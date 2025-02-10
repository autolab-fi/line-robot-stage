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

float readFloatFromEEPROM(int &address) {
    byte bytes[4];
    for (int i = 0; i < 4; i++) {
        bytes[i] = EEPROM.read(address + i);  // Читаем каждый байт из EEPROM
    }
    address += 4;  // Увеличиваем адрес на 4 байта

    float value;
    memcpy(&value, bytes, sizeof(float));  // Преобразуем массив байтов обратно в float
    return value;
}

NetworkSettings readNetworkSettingsFromEEPROM() {
    int address = START_ADDRESS_NETWORK_SETTINGS;
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

RobotSettings readRobotSettingsFromEEPROM() {
    int address = START_ADDRESS_ROBOT_SETTINGS;
    RobotSettings settings;
    // physical parameters
    settings.wheelRadius = readFloatFromEEPROM(address);
    settings.distanceBetweenWheels = readFloatFromEEPROM(address);
    settings.encoderResolution = readIntFromEEPROM(address);
    // motors
    settings.pinMotorLeft1 = readIntFromEEPROM(address);
    settings.pinMotorLeft2 = readIntFromEEPROM(address);
    settings.pinMotorRight1 = readIntFromEEPROM(address);
    settings.pinMotorRight2 = readIntFromEEPROM(address);
    // encoders
    settings.pinEncoderLeft1 = readIntFromEEPROM(address);
    settings.pinEncoderLeft2 = readIntFromEEPROM(address);
    settings.pinEncoderRight1 = readIntFromEEPROM(address);
    settings.pinEncoderRight2 = readIntFromEEPROM(address);
    // coefficients
    settings.kpAng = readFloatFromEEPROM(address);
    settings.kiAng = readFloatFromEEPROM(address);
    settings.kdAng = readFloatFromEEPROM(address);
    settings.kpSpeedLeft = readFloatFromEEPROM(address);
    settings.kpSpeedRight = readFloatFromEEPROM(address);
    settings.kiSpeed = readFloatFromEEPROM(address);
    settings.kdSpeedLeft = readFloatFromEEPROM(address);
    settings.kdSpeedRight = readFloatFromEEPROM(address);
    // i2c and leds
    settings.pinCLK = readIntFromEEPROM(address);
    settings.pinSDA = readIntFromEEPROM(address);
    settings.pinLED1 = readIntFromEEPROM(address);
    settings.pinLED2 = readIntFromEEPROM(address);

    settings.kStraight = readFloatFromEEPROM(address);
    settings.maxSpeedRadians = readFloatFromEEPROM(address);

    settings.pinCharging = readIntFromEEPROM(address);
    settings.pinVoltage = readIntFromEEPROM(address);
    return settings;
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
    //Serial.println(value);
    EEPROM.write(address, value & 0xFF);       // Write lower byte
    EEPROM.write(address + 1, (value >> 8) & 0xFF); // Write upper byte
    address += 2;
}

void writeFloatToEEPROM(int &address, float value) {
    byte *bytes = (byte *)&value;  // Преобразуем float в массив байтов
    for (int i = 0; i < 4; i++) {
        EEPROM.write(address + i, bytes[i]);  // Записываем каждый байт в EEPROM
    }
    address += 4;  // Увеличиваем адрес на 4 байта
}

void saveNetworkSettingsToEEPROM(const NetworkSettings &settings) {
    int address = START_ADDRESS_NETWORK_SETTINGS;

    // Write IP Address, Netmask, Gateway
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


void saveRobotSettingsToEEPROM(const RobotSettings &settings) {
    int address = START_ADDRESS_ROBOT_SETTINGS;

    writeFloatToEEPROM(address, settings.wheelRadius);
    writeFloatToEEPROM(address, settings.distanceBetweenWheels);
    writeIntToEEPROM(address, settings.encoderResolution);
    // motors
    writeIntToEEPROM(address, settings.pinMotorLeft1);
    writeIntToEEPROM(address, settings.pinMotorLeft2);
    writeIntToEEPROM(address, settings.pinMotorRight1);
    writeIntToEEPROM(address, settings.pinMotorRight2);
    // encoders
    writeIntToEEPROM(address, settings.pinEncoderLeft1);
    writeIntToEEPROM(address, settings.pinEncoderLeft2);
    writeIntToEEPROM(address, settings.pinEncoderRight1);
    writeIntToEEPROM(address, settings.pinEncoderRight2);
    // coefficients
    writeFloatToEEPROM(address, settings.kpAng);
    writeFloatToEEPROM(address, settings.kiAng );
    writeFloatToEEPROM(address, settings.kdAng);
    writeFloatToEEPROM(address, settings.kpSpeedLeft);
    writeFloatToEEPROM(address, settings.kpSpeedRight);
    writeFloatToEEPROM(address, settings.kiSpeed);
    writeFloatToEEPROM(address, settings.kdSpeedLeft);
    writeFloatToEEPROM(address, settings.kdSpeedRight);
    // i2c and leds
    writeIntToEEPROM(address, settings.pinCLK);
    writeIntToEEPROM(address, settings.pinSDA);
    writeIntToEEPROM(address, settings.pinLED1);
    writeIntToEEPROM(address, settings.pinLED2);

    writeFloatToEEPROM(address,  settings.kStraight);
    writeFloatToEEPROM(address, settings.maxSpeedRadians);

    writeIntToEEPROM(address, settings.pinCharging);
    writeIntToEEPROM(address, settings.pinVoltage);
    EEPROM.commit();
}