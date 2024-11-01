#include <EEPROM.h>
#include <IPAddress.h>

#define START_ADDRESS 0 // start byte address in eeprom
#define EEPROM_SIZE 512

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


void setup() {
    Serial.begin(115200);
    if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("failed to initialize EEPROM");
      delay(1000000);
    }
    // DEFINE SETTINGS HERE
    NetworkSettings myNetworkSettings = {
        IPAddress(192, 168, 1, 101),    // IP Address in local network NOT USED
        IPAddress(255, 255, 255, 0),    // Netmask in local network NOT USED
        IPAddress(192, 168, 1, 1),      // Gateway in local network NOT USED        
        "WIFI_PASSWORD",                // WiFi Password     
        "WIFI_SSID",                    // WiFi SSID
        IPAddress(0, 0, 0, 0),    // MQTT Broker IP
        0,                           // MQTT Broker port
        "MQTT_PASSWORD",                // MQTT Password
        "MQTT_LOGIN",                   // MQTT login
        "MQTT_SYSTEM_TOPIC",            // MQTT Topic 1
        "MQTT_USER_TOPIC"               // MQTT Topic 2
    };

    // Write settings to EEPROM
    saveNetworkSettingsToEEPROM(myNetworkSettings);
    Serial.println("Network settings saved to EEPROM.");


    // Read settings from EEPROM
    NetworkSettings myNetworkSettingsRead = readNetworkSettingsFromEEPROM();

    // Display the network settings
    Serial.print("IP Address: ");
    Serial.println(myNetworkSettingsRead.ipAddress);
    Serial.print("Netmask: ");
    Serial.println(myNetworkSettingsRead.netmask);
    Serial.print("Gateway: ");
    Serial.println(myNetworkSettingsRead.gateway);
    Serial.print("WiFi SSID: ");
    Serial.println(myNetworkSettingsRead.wifiSSID);
    Serial.print("WiFi Password: ");
    Serial.println(myNetworkSettingsRead.wifiPassword);
    Serial.print("MQTT Broker IP: ");
    Serial.println(myNetworkSettingsRead.mqttBrokerIP);
    Serial.print("MQTT Broker Port: ");
    Serial.println(myNetworkSettingsRead.mqttPort);
    Serial.print("MQTT Username: ");
    Serial.println(myNetworkSettingsRead.mqttUsername);
    Serial.print("MQTT Password: ");
    Serial.println(myNetworkSettingsRead.mqttPassword);
    Serial.print("MQTT Topic 1: ");
    Serial.println(myNetworkSettingsRead.mqttTopic1);
    Serial.print("MQTT Topic 2: ");
    Serial.println(myNetworkSettingsRead.mqttTopic2);
}

void loop() {
    // Do nothing in loop
}

void writeStringToEEPROM(int &address, const String &data) {
    int length = data.length();
    EEPROM.write(address, length);  // Write string length to EEPROM
    address += 1;
    
    for (int i = 0; i < length; i++) {
        EEPROM.write(address + i, data[i]);
    }
    address += length;
}

void writeIPAddressToEEPROM(int &address, const IPAddress &ip) {
    for (int i = 0; i < 4; i++) {
        EEPROM.write(address + i, ip[i]);
    }
    address += 4;
}

// Function to write an integer to EEPROM
void writeIntToEEPROM(int &address, int value) {
    Serial.println(value);
    EEPROM.write(address, value & 0xFF);       // Write lower byte
    EEPROM.write(address + 1, (value >> 8) & 0xFF); // Write upper byte
    address += 2;
}

void saveNetworkSettingsToEEPROM(const NetworkSettings &settings) {
    int address = START_ADDRESS;

    // Write IP Address, Netmask, Gateway
    Serial.println(settings.ipAddress);
    writeIPAddressToEEPROM(address, settings.ipAddress);
    writeIPAddressToEEPROM(address, settings.netmask);
    writeIPAddressToEEPROM(address, settings.gateway);

    // Write WiFi SSID and Password
    writeStringToEEPROM(address, settings.wifiSSID);
    writeStringToEEPROM(address, settings.wifiPassword);

    // Write MQTT Broker IP Address, Username, Password, Topics
    writeIPAddressToEEPROM(address, settings.mqttBrokerIP);
    writeIntToEEPROM(address, settings.mqttPort);
    writeStringToEEPROM(address, settings.mqttUsername);
    writeStringToEEPROM(address, settings.mqttPassword);
    writeStringToEEPROM(address, settings.mqttTopic1);
    writeStringToEEPROM(address, settings.mqttTopic2);
    EEPROM.commit();
}


String readStringFromEEPROM(int &address) {
    int length = EEPROM.read(address);  // Read the length of the string
    address += 1;

    char data[length + 1];  // Create a buffer for the string, plus null terminator
    for (int i = 0; i < length; i++) {
        data[i] = EEPROM.read(address + i);
    }
    data[length] = '\0';  // Null terminate the string
    address += length;

    return String(data);
}

IPAddress readIPAddressFromEEPROM(int &address) {
    byte ip[4];
    for (int i = 0; i < 4; i++) {
        ip[i] = EEPROM.read(address + i);
    }
    address += 4;
    return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}

// Function to read an integer from EEPROM
int readIntFromEEPROM(int &address) {
    int value = 0;
    value = EEPROM.read(address) | (EEPROM.read(address + 1) << 8);
    address += 2;
    return value;
}


NetworkSettings readNetworkSettingsFromEEPROM() {
    int address = START_ADDRESS;
    NetworkSettings settings;
    // Read IP Address, Netmask, Gateway
    settings.ipAddress = readIPAddressFromEEPROM(address);
    settings.netmask = readIPAddressFromEEPROM(address);
    settings.gateway = readIPAddressFromEEPROM(address);

    // Read WiFi SSID and Password
    settings.wifiSSID = readStringFromEEPROM(address);
    settings.wifiPassword = readStringFromEEPROM(address);

    // Read MQTT Broker IP Address, Username, Password, Topics
    settings.mqttBrokerIP = readIPAddressFromEEPROM(address);
    settings.mqttPort = readIntFromEEPROM(address);
    settings.mqttUsername = readStringFromEEPROM(address);
    settings.mqttPassword = readStringFromEEPROM(address);
    settings.mqttTopic1 = readStringFromEEPROM(address);
    settings.mqttTopic2 = readStringFromEEPROM(address);
    return settings;
}
