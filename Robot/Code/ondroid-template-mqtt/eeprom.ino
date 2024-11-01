// read string from eeprom
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
// read int from eeprom
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