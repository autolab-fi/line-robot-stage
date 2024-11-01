#include <WiFi.h>
#include <PubSubClient.h>
#include <lineRobot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <EEPROM.h>
#include "esp_pm.h"
//#define versionToken "version-control-token"
#define EEPROM_SIZE 512
#define START_ADDRESS 0

int moveValues[10];

const uint8_t ledPin1 = 22;
const uint8_t ledPin2  = 23;
const uint8_t voltagePin = 36;
const uint8_t chargingPin = 39;


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
float k_voltage = 1.48;
float voltage = 0;
// timer for detecting start of the user's code executing
unsigned long user_code_timer = 0;
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
// commands - is variable to store sequance of the commands 
String commands;
// mqtt topic for system messages like commands
String systemTopicInput;
// mqtt topic for system messages like commands
String systemTopicOutput;
// mqtt topic for user messages user's output
String userTopicOutput;
// wifi client
WiFiClient espClient;
// mqtt client
PubSubClient client(espClient);

void setup() {
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

  // zero signal to esp32 pins
  analogWrite(robot.in1, 0);
  analogWrite(robot.in2, 0);
  analogWrite(robot.in3, 0);
  analogWrite(robot.in4, 0);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
  // pinMode for charging and voltage pins
  pinMode(chargingPin, INPUT);
  pinMode(voltagePin, INPUT);
  // Devide threads on 1 core and 2 core of esp32
  xTaskCreatePinnedToCore(Task1Core, "Core 0", 10000, NULL, 1, NULL,  0); //first core for connection to wifi and server
  xTaskCreatePinnedToCore(Task2Core, "Core 1", 10000, NULL, 1, NULL,  1); //second core for user's code and robot movement
}

void Task1Core( void * parameter) {  
   // Set software serial baud to 115200;
  Serial.begin(115200);
  if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("failed to initialize EEPROM");
      delay(1000000);
    }
  // Read settings from EEPROM
  NetworkSettings networkSettings = readNetworkSettingsFromEEPROM();
  // Display the network settings
  Serial.print("IP Address: ");
  Serial.println(networkSettings.ipAddress);
  Serial.print("Netmask: ");
  Serial.println(networkSettings.netmask);
  Serial.print("Gateway: ");
  Serial.println(networkSettings.gateway);
  Serial.print("WiFi SSID: ");
  Serial.println(networkSettings.wifiSSID);
  Serial.print("WiFi Password: ");
  Serial.println(networkSettings.wifiPassword);
  Serial.print("MQTT Broker IP: ");
  Serial.println(networkSettings.mqttBrokerIP);
  Serial.print("MQTT Broker Port: ");
  Serial.println(networkSettings.mqttPort);
  Serial.print("MQTT Username: ");
  Serial.println(networkSettings.mqttUsername);
  Serial.print("MQTT Password: ");
  Serial.println(networkSettings.mqttPassword);
  Serial.print("MQTT Topic 1(System): ");
  Serial.println(networkSettings.mqttTopic1);
  Serial.print("MQTT Topic 2(User): ");
  Serial.println(networkSettings.mqttTopic2);

  // config network
  // if (!WiFi.config( networkSettings.ipAddress,  networkSettings.gateway,  networkSettings.netmask)) {
  //   Serial.println("STA Failed to configure");
  // }
  WiFi.setSleep(false); // Disable Wi-Fi power saving
  // Connecting to a WiFi network
  WiFi.begin(networkSettings.wifiSSID, networkSettings.wifiPassword);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

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
  // start robot lib
  robot.begin();
  // timer to restart robot sometimes
  long timer = millis();
  long blink_timer = millis();
  int8_t signal = 0;
  // it is infinite loop on the first core
  for(;;){    
    if (blink_timer<millis()){
      blink_timer = millis()+1000;
      if (signal==0)
        signal = 1;
      else {
        signal = 0;
      }
      digitalWrite(ledPin1, signal);
      digitalWrite(ledPin2, signal);
    }
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
    if (WiFi.status() != WL_CONNECTED) {
      //reconnect to wifi if lost conenction for 15 seconds if not connected - restart
      WiFi.reconnect();
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts<100) {
        attempts++;
        Serial.print(".");
        delay(150);
      }
      if (WiFi.status() != WL_CONNECTED){
        restartESP32();
      }
    }
    if (!client.connected() && !updateInProgress){
      Serial.println("Reconnect to the broker!");
      NetworkSettings networkSettings = readNetworkSettingsFromEEPROM();
      connectBroker(networkSettings);
    }
    // handle messages mqtt
    client.loop();
    //handle requests to esp32 OTA
    //ArduinoOTA.handle();
    // to prevent core panic
    delay(1);
  }
}

//This core is used exclusively for student code for educational purposes and for control motors
void Task2Core( void * parameter) {       
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
    // to prevent core panic
    delay(1);
  }
  // sendMessage to chaennel
  sendMessageSystem("{\"type\":\"start\", \"msg\":\"Started!\"}");
  // pause before starting user's code
  delay(500);
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
  while (!client.connected()) {
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
  client.subscribe(systemTopicInput.c_str());
}

void loop(){}