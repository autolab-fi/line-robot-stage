#include <WiFi.h>
#include <PubSubClient.h>
#include <lineRobot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <EEPROM.h>
#include "esp_pm.h"
#include <Adafruit_ADS1X15.h>

//#define versionToken "version-control-token"
#define EEPROM_SIZE 512
#define START_ADDRESS_NETWORK_SETTINGS 0
#define START_ADDRESS_ROBOT_SETTINGS 300

int moveValues[10];

uint8_t ledPin1 = 2;
uint8_t ledPin2  = 4;

uint8_t pinCLK = 16;
uint8_t pinSDA = 17;

// adc to read battery voltage
Adafruit_ADS1115 adc;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 adc;     /* Use this for the 12-bit version */
// percentage for upload
uint8_t previous_percentage = 0;
// downloading new code flag
bool updateInProgress = false;
// to activate student's code
bool robotStart = false;
// flag for reset and charging
bool robotMove = false;
bool robotCharging = false;
// measuring voltage
float voltage = 0;
float k_voltage = 10.0;
bool charging = false;
// flag for set coeffs mode
bool robot_set_coeffs_mode = false;
// timer for detecting start of the user's code executing
unsigned long user_code_timer = 0;
unsigned long timer_started_upload = 0;
// struct for store setting in EEPROM
struct NetworkSettings {
    IPAddress ipAddress; // esp local ip address
    IPAddress netmask; // netmask in local network
    IPAddress gateway; // gateway in local network
    String wifiPassword; // wifi password
    String wifiSSID; // wifi ssid
    IPAddress mqttBrokerIP; // ip of the mqtt broker
    int mqttPort;  // port of mqtt borker
    String mqttPassword; // mqtt password
    String mqttUsername; // mqtt username
    String mqttTopic1; // user's mqtt topic 
    String mqttTopic2; // system mqtt topic
};
struct RobotSettings {
  float wheelRadius;
  float distanceBetweenWheels;
  int encoderResolution;
  int pinMotorLeft1;
  int pinMotorLeft2;
  int pinMotorRight1;
  int pinMotorRight2;
  int pinEncoderLeft1;
  int pinEncoderLeft2;
  int pinEncoderRight1;
  int pinEncoderRight2;
  float kpAng;
  float kiAng;
  float kdAng;
  float kpSpeedLeft;
  float kpSpeedRight;
  float kiSpeed;
  float kdSpeedLeft;
  float kdSpeedRight;
  int pinCLK;
  int pinSDA;
  int pinLED1;
  int pinLED2;
  float kStraight;
  float maxSpeedRadians;
  int pinCharging;
  int pinVoltage;
};
// commands - is variable to store sequance of the commands 
String commands;
// mqtt topic for system messages like commands
String systemTopicInput;
// mqtt topic for system messages like commands
String systemTopicOutput;
// mqtt topic for user messages user's output
String userTopicOutput;
// network settings
NetworkSettings networkSettings;
// robot settings
RobotSettings robotSettings;
// wifi client
WiFiClient espClient;
// line robot object declaration
lineRobot robot;
// mqtt client
PubSubClient client(espClient);

void setup() {
  // config for update by http
  httpUpdate.rebootOnUpdate(false); // remove automatic update
  httpUpdate.onStart(update_started);
  httpUpdate.onEnd(update_finished);
  httpUpdate.onProgress(update_progress);
  httpUpdate.onError(update_error);

  // Lock CPU frequency to 240 MHz
  esp_pm_config_esp32_t pm_config;
  pm_config.max_freq_mhz = 240;
  pm_config.min_freq_mhz = 240;
  pm_config.light_sleep_enable = false;
  esp_pm_configure(&pm_config);

  // Devide threads on 1 core and 2 core of esp32
  xTaskCreatePinnedToCore(Task1Core, "Core 0", 10000, NULL, 1, NULL,  0); //first core for connection to wifi and server
  xTaskCreatePinnedToCore(Task2Core, "Core 1", 10000, NULL, 1, NULL,  1); //second core for user's code and robot movement
}

void Task1Core( void * parameter) {  
   // Set software serial baud to 115200;
  Serial.begin(115200);
  if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("failed to initialize EEPROM");
      delay(300);
    }

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
   //ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   adc.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!adc.begin()) {
    Serial.println("Failed to initialize ADS.");
  }
  // Read settings from EEPROM
  networkSettings = readNetworkSettingsFromEEPROM();
  
  // Read robot settings 
  robotSettings = readRobotSettingsFromEEPROM();
  
  // Display the network settings
  printConfig();
  ledPin1 = robotSettings.pinLED1;
  ledPin2 = robotSettings.pinLED2;
  pinCLK = robotSettings.pinCLK;
  pinSDA = robotSettings.pinSDA;
  initRobot();
  // zero signal to esp32 pins
  analogWrite(robot.in1, 0);
  analogWrite(robot.in2, 0);
  analogWrite(robot.in3, 0);
  analogWrite(robot.in4, 0);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
  // config network
  // if (!WiFi.config( networkSettings.ipAddress,  networkSettings.gateway,  networkSettings.netmask)) {
  //   Serial.println("STA Failed to configure");
  // }
  WiFi.setSleep(false); // Disable Wi-Fi power saving
  // Connecting to a WiFi network
  WiFi.begin(networkSettings.wifiSSID, networkSettings.wifiPassword);
  Serial.println("To skip wifi connection enter: \"y\"");
  Serial.println("Connecting to WiFi..");
  bool skip = false;
  unsigned long timer = millis();
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (Serial.available() > 0){
        if (Serial.read()=='y'){
          skip = true;
          Serial.println("Skipped WiFi connection. To get more info use \"help\" command");
          break;
        }
      }
      if (millis()-timer>7000){
        restartESP32();
      }
  }
  if (!skip){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(networkSettings.wifiSSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // mqtt topics
    userTopicOutput = networkSettings.mqttTopic1;
    systemTopicOutput = networkSettings.mqttTopic2+"/output";
    systemTopicInput = networkSettings.mqttTopic2+"/input";
    //connecting to a mqtt broker
    client.setServer( networkSettings.mqttBrokerIP, networkSettings.mqttPort);
    client.setCallback(callback);
    connectBroker(networkSettings);
    // robot should only publish and not reading it's own messages
    // client.subscribe(userTopic.c_str());
    String helloMessage = "{\"type\":\"hello\", \"msg\":\"Esp32 connected!\", \"version-control-token\":\""+getVersionToken()+"\"}";
    sendMessageSystem(helloMessage);
  }
  // start robot lib
  //robot.begin();
  // timer to restart robot sometimes
  timer = millis();
  // long blink_timer = millis();
  // int8_t signal = 0;
  // it is infinite loop on the first core
  for(;;){    
    // timer wait for uploading new code
    if (updateInProgress && millis()-timer_started_upload>30000){
      restartESP32();
    }
    // blink
    // if (blink_timer<millis()){
    //   blink_timer = millis()+1000;
    //   if (signal==0)
    //     signal = 1;
    //   else {
    //     signal = 0;
    //   }
    //   digitalWrite(ledPin1, signal);
    //   digitalWrite(ledPin2, signal);
    // }
    //restart board every 20 minutes if user's code not executing and robot not in moving to reset point
    if (millis()-timer>1200000){
      if (!(robotStart || robotMove)){
        restartESP32();
      } 
    }
    // restart board from the first core if user's code executing more then 3 minutes
    if (robotStart && millis()-user_code_timer>180000){
      restartESP32();
    }
    // check if WIFI not connected
    if (WiFi.status() != WL_CONNECTED && !skip) {
      //reconnect to wifi if lost conenction for 15 seconds if not connected - restart
      WiFi.reconnect();
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts<50) {
        attempts++;
        Serial.print(".");
        delay(150);
      }
      if (WiFi.status() != WL_CONNECTED){
        restartESP32();
      }
    }
    if (!client.connected() && !updateInProgress && !skip){
      Serial.println("Reconnect to the broker!");
      NetworkSettings networkSettings = readNetworkSettingsFromEEPROM();
      connectBroker(networkSettings);
    }
    ReadSerial();
    // handle messages mqtt
    if (!skip){
      client.loop();
    }
    //handle requests to esp32 OTA
    //ArduinoOTA.handle();
    // to prevent core panic
    delay(1);
  }
}

//This core is used exclusively for student code for educational purposes and for control motors
void Task2Core( void * parameter) {       
  // second timer for restart 
  user_code_timer = millis(); 
  // infinite loop while user's code not started
  while(!robotStart){
    // robot recieved command for moving to specified point
    if (robotMove){
      func_move_robot();
      robotMove = false;
    }
    // robot recieved command to move backward
    if (robotCharging){
      moveCharging();
      robotCharging=false;
    }
    if (millis()-user_code_timer > 1300000) {
      if (!(robotStart || robotMove)){
        restartESP32();
      } 
    }
    // timer wait for uploading new code
    if (updateInProgress && millis()-timer_started_upload>30000){
      restartESP32();
    }
    // to prevent core panic
    delay(1);
  }
  // sendMessage to chaennel
  sendMessageSystem("{\"type\":\"start\", \"msg\":\"Started!\"}");
  // pause before starting user's code
  delay(5200);
  // timer for user's code
  user_code_timer = millis(); 
  //Setup code for the second core 
  setup_0();
  for(;;){        //The main control code for electric motors
    loop_0();
    //restart board from the second core if user's code executing more then 3 minutes
    if (millis()-user_code_timer>180000){
      restartESP32();
    }
  }
}

void connectBroker(NetworkSettings networkSettings){
  unsigned long timer = millis();
  while (!client.connected()) {
      if (millis()-timer>6500){
        restartESP32();
      }
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), networkSettings.mqttUsername.c_str(), networkSettings.mqttPassword.c_str())) {
          Serial.println("MQTT broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // subscribe
  client.subscribe(systemTopicInput.c_str(), 1);
}

void loop(){}