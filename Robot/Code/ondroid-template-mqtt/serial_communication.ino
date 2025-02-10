void printConfig() {
    Serial.println("==================== Network Parameters ====================");
    // Serial.print("IP Address: ");
    // Serial.println(networkSettings.ipAddress);
    // Serial.print("Netmask: ");
    // Serial.println(networkSettings.netmask);
    // Serial.print("Gateway: ");
    // Serial.println(networkSettings.gateway);
    Serial.print("WiFi SSID(wssid): ");
    Serial.println(networkSettings.wifiSSID);
    Serial.print("WiFi Password(wpas): ");
    Serial.println(networkSettings.wifiPassword);
    Serial.print("MQTT Broker IP(mip): ");
    Serial.println(networkSettings.mqttBrokerIP);
    Serial.print("MQTT Broker Port(mport): ");
    Serial.println(networkSettings.mqttPort);
    Serial.print("MQTT Username(mu): ");
    Serial.println(networkSettings.mqttUsername);
    Serial.print("MQTT Password(mpas): ");
    Serial.println(networkSettings.mqttPassword);
    Serial.print("MQTT Topic 1(User)(tu): ");
    Serial.println(networkSettings.mqttTopic1);
    Serial.print("MQTT Topic 2(System)(ts): ");
    Serial.println(networkSettings.mqttTopic2);
    Serial.println("==================== Robot Parameters ====================");
    Serial.print("Robot pin motor left 1(pml1): ");
    Serial.println(robotSettings.pinMotorLeft1);
    Serial.print("Robot pin motor left 2(pml2): ");
    Serial.println(robotSettings.pinMotorLeft2);
    Serial.print("Robot pin motor right 1(pmr1): ");
    Serial.println(robotSettings.pinMotorRight1);
    Serial.print("Robot pin motor right 2(pmr2): ");
    Serial.println(robotSettings.pinMotorRight2);
    Serial.print("Robot pin encoder left 1(pel1): ");
    Serial.println(robotSettings.pinEncoderLeft1);
    Serial.print("Robot pin encoder left 2(pel2): ");
    Serial.println(robotSettings.pinEncoderLeft2);
    Serial.print("Robot pin encoder right 1(per1): ");
    Serial.println(robotSettings.pinEncoderRight1);
    Serial.print("Robot pin encoder right 2(per2): ");
    Serial.println(robotSettings.pinEncoderRight2);
    Serial.print("Robot radius wheel(wrad): ");
    Serial.println(robotSettings.wheelRadius);
    Serial.print("Robot distance between wheels(wdist): ");
    Serial.println(robotSettings.distanceBetweenWheels);
    Serial.print("Robot Encoder resolution(er): ");
    Serial.println(robotSettings.encoderResolution);
    Serial.print("Robot max speed in rad/s(maxs): ");
    Serial.println(robotSettings.maxSpeedRadians);
    Serial.println("==================== Robot coefficients ====================");
    Serial.print("Robot kpAng(kpa): ");
    Serial.println(robotSettings.kpAng);
    Serial.print("Robot kiAng(kia): ");
    Serial.println(robotSettings.kiAng);
    Serial.print("Robot kdAng(kda): ");
    Serial.println(robotSettings.kdAng);
    Serial.print("Robot kpSpeedLeft(kpsl): ");
    Serial.println(robotSettings.kpSpeedLeft);
    Serial.print("Robot kpSpeedRight(kpsr): ");
    Serial.println(robotSettings.kpSpeedRight);
    Serial.print("Robot kiSpeed(kis): ");
    Serial.println(robotSettings.kiSpeed);
    Serial.print("Robot kdSpeedLeft(kdsl): ");
    Serial.println(robotSettings.kdSpeedLeft);
    Serial.print("Robot kdSpeedRight(kdsr): ");
    Serial.println(robotSettings.kdSpeedRight);
    Serial.print("Robot kStraight(ks): ");
    Serial.println(robotSettings.kStraight);
    Serial.println("==================== Additional peripherals ====================");
    Serial.print("Pin led 1(pled1): ");
    Serial.println(robotSettings.pinLED1);
    Serial.print("Pin led 2(pled2): ");
    Serial.println(robotSettings.pinLED2);
    Serial.print("Pin CLK(pclk): ");
    Serial.println(robotSettings.pinCLK);
    Serial.print("Pin SDA(psda): ");
    Serial.println(robotSettings.pinSDA);
    Serial.print("Pin measure battery voltage(pbat): ");
    Serial.println(robotSettings.pinVoltage);
    Serial.print("Pin measure charging voltage(pch): ");
    Serial.println(robotSettings.pinCharging);
}


String setConfig(String name, String StringValue){
  if (name == "mip") {
      // WiFi and MQTT config
      // mqtt broker ip
      networkSettings.mqttBrokerIP.fromString(StringValue);
      return "MQTT broker ip = "+networkSettings.mqttBrokerIP.toString()+". To apply network and broker settings please save settings and reboot";
  } else if (name == "mport") { // mqtt broker port
      int value = StringValue.toInt();
      networkSettings.mqttPort = value;
      return "MQTT Port = "+String(networkSettings.mqttPort)+". To apply network and broker settings please save settings and reboot";
  } else if (name == "muser") { // mqtt username
      networkSettings.mqttUsername = StringValue;
      return "MQTT username = "+networkSettings.mqttUsername+". To apply network and broker settings please save settings and reboot";
  } else if (name == "mpas") { // mqtt password
      networkSettings.mqttPassword = StringValue;
      return "MQTT Password = "+networkSettings.mqttPassword+". To apply network and broker settings please save settings and reboot";
  } else if (name == "wssid") { // wifi ssid
      networkSettings.wifiSSID = StringValue;
      return "WiFi SSID = "+networkSettings.wifiSSID;
  } else if (name == "wpas") { // wifi password
      networkSettings.wifiPassword = StringValue;
      return "WiFi password = "+networkSettings.wifiPassword;
  } else if (name == "tu") { // topic user
      networkSettings.mqttTopic1 = StringValue;
      return "User topic = "+networkSettings.mqttTopic1;
  } else if (name == "ts") { // topic system
      networkSettings.mqttTopic2 = StringValue;
      return "System topic = "+ networkSettings.mqttTopic2;
  } else if (name == "pml1") { // pin motor left 1
      int value = StringValue.toInt();
      //Serial.println(value);
      robotSettings.pinMotorLeft1 = value;
      initRobot();
      return "Pin motor left 1 = "+String(robotSettings.pinMotorLeft1);
  } else if (name == "pml2") { // pin motor left 2
      
      int value = StringValue.toInt();
      robotSettings.pinMotorLeft2 = value;
      initRobot();
      return "Pin motor left 2 = "+String(robotSettings.pinMotorLeft2);
  } else if (name == "pmr1") { // pin motor right 1
      int value = StringValue.toInt();
      robotSettings.pinMotorRight1 = value;
      initRobot();
      return "Pin motor right 1 = "+String(robotSettings.pinMotorRight1);;
  } else if (name == "pmr2") { // pin motor right 2
      int value = StringValue.toInt();
      robotSettings.pinMotorRight2 = value;
      initRobot();
      return "Pin motor right 2 = "+String(robotSettings.pinMotorRight2);
  } else if (name == "pel1") { // pin encoder left 1
      int value = StringValue.toInt();
      robotSettings.pinEncoderLeft1 = value;
      initRobot();
      return "Pin encoder left 1 = "+String(robotSettings.pinEncoderLeft1);
  } else if (name == "per1") { // pin encoder right 1
      int value = StringValue.toInt();
      robotSettings.pinEncoderRight1 = value;
      //initRobot();
      return "Pin encoder right 1 = "+String(robotSettings.pinEncoderRight1);
  } else if (name == "pel2") { // pin encoder left 2
      int value = StringValue.toInt();
      robotSettings.pinEncoderLeft2 = value;
      //initRobot();
      return "Pin encoder left 2 = "+String(robotSettings.pinEncoderLeft2);;
  } else if (name == "per2") { // pin encoder right 2
      int value = StringValue.toInt();
      robotSettings.pinEncoderRight2 = value;
      //initRobot();
      return "Pin encoder right 2 = "+String(robotSettings.pinEncoderRight2);
  } else if (name == "er") {
      float value = StringValue.toFloat();
      robotSettings.encoderResolution = value;
      //initRobot();
      return "encoder resolution = "+String(robotSettings.encoderResolution);
  } else if (name == "wrad") {
      float value = StringValue.toFloat();
      robotSettings.wheelRadius = value;
      initRobot();
      return "wheel radius = "+String(robotSettings.wheelRadius);
  } else if (name == "wdist") {
      float value = StringValue.toFloat();
      robotSettings.distanceBetweenWheels = value;
      initRobot();
      return "distance between wheels = "+String(robotSettings.distanceBetweenWheels);
  } else if (name == "kpa") { // Coefficients of regulators
      float value = StringValue.toFloat();
      robot.kpAng = value;
      robotSettings.kpAng = value;
      return "kpAng = "+String(robot.kpAng);
  } else if (name == "kia") {
      float value = StringValue.toFloat();
      robot.kiAng = value;
      robotSettings.kiAng = value;
      return "kiAng = "+String(robot.kiAng);
  } else if (name == "kda") {
      float value = StringValue.toFloat();
      robot.kdAng = value;
      robotSettings.kdAng = value;
      return "kdAng = "+String(robot.kdAng);
  } else if (name == "kpsl") {
      float value = StringValue.toFloat();
      robot.kpSpeedLeft = value;
      robotSettings.kpSpeedLeft = value;
      return "kpSpeedLeft = "+String(robot.kpSpeedLeft);
  } else if (name == "kpsr") {
      float value = StringValue.toFloat();
      robotSettings.kpSpeedRight = value;
      robot.kpSpeedRight = value;
      return "kpSpeedRight = "+String(robot.kpSpeedRight);
  } else if (name == "kis") {
      float value = StringValue.toFloat();
      robotSettings.kiSpeed = value;
      robot.kiSpeed = value;
      return "kiSpeed = "+String(robot.kiSpeed);
  } else if (name == "kdsl") {
      float value = StringValue.toFloat();
      robotSettings.kdSpeedLeft = value;
      robot.kdSpeedLeft = value;
      return "kdSpeedLeft = "+String(robot.kdSpeedLeft);
  } else if (name == "kdsr") {
      float value = StringValue.toFloat();
      robotSettings.kdSpeedRight = value;
      robot.kdSpeedRight = value;
      return "kdSpeedRight = "+String(robot.kdSpeedRight);
  } else if (name == "ks") {
    float value = StringValue.toFloat();
    robotSettings.kStraight = value;
    robot.kStraight = value;
    return "kdSpeedRight = "+String(robot.kStraight);
  } else if (name == "maxs") {
    float value = StringValue.toFloat();
    robotSettings.maxSpeedRadians = value;
    //robot.maxSpeedRadians = value;
    return "max speed in rad/s = "+String(robotSettings.maxSpeedRadians);
  } else if (name == "pled1") {
      int value = StringValue.toInt();
      robotSettings.pinLED1 = value;
      ledPin1 = value;
      return "pin led 1 = " + String(robotSettings.pinLED1);
  } else if (name == "pled2") {
      int value = StringValue.toInt();
      robotSettings.pinLED2 = value;
      ledPin2 = value;
      return "pin led 2 = " + String(robotSettings.pinLED2);
  } else if (name == "pclk") {
      int value = StringValue.toInt();
      robotSettings.pinCLK = value;
      pinCLK = value;
      return "pin clk = " + String(robotSettings.pinCLK);
  } else if (name == "psda") {
      int value = StringValue.toInt();
      robotSettings.pinSDA = value;
      pinSDA = value;
      return "pin sda = " + String(robotSettings.pinSDA);
  } else if (name == "pch") {
    int value = StringValue.toInt();
    robotSettings.pinCharging = value;
    return "pin charging = " + String(robotSettings.pinCharging);
  } else if (name == "pbat") {
    int value = StringValue.toInt();
    robotSettings.pinVoltage = value;
    return "pin battery = " + String(robotSettings.pinVoltage);
  } else {
      return "Unknown parameter name!";
  }
}


String getConfig(String name) {
  if (name == "mip") {
      // WiFi and MQTT config
      // mqtt broker ip
      return "MQTT broker ip = "+networkSettings.mqttBrokerIP.toString();
  } else if (name == "mport") { // mqtt broker port
      return "MQTT broker port = "+String(networkSettings.mqttPort);
  } else if (name == "muser") { // mqtt username
      return "MQTT username = "+networkSettings.mqttUsername;
  } else if (name == "mpas") { // mqtt password
      return "MQTT password = " + networkSettings.mqttPassword;
  } else if (name == "wssid") { // wifi ssid
      return "WiFi SSID = "+networkSettings.wifiSSID;
  } else if (name == "wpas") { // wifi password
      return "WiFi password = "+networkSettings.wifiPassword;
  } else if (name == "tu") { // topic user
      return "User mqtt topic = "+networkSettings.mqttTopic1;
  } else if (name == "ts") { // topic system
      return "System mqtt topic = "+networkSettings.mqttTopic2;
  } else if (name == "pml1") { // pin motor left 1
      return "pin motor left 1 = "+String(robotSettings.pinMotorLeft1);
  } else if (name == "pml2") { // pin motor left 2
      return "pin motor left 2 = "+String(robotSettings.pinMotorLeft2);
  } else if (name == "pmr1") { // pin motor right 1
      return "pin motor right 1 = "+String(robotSettings.pinMotorRight1);
  } else if (name == "pmr2") { // pin motor right 2
      return "pin motor right 2 = "+String(robotSettings.pinMotorRight2);
  } else if (name == "pel1") { // pin encoder left 1
      return "pin encoder left 1 = "+String(robotSettings.pinEncoderLeft1);
  } else if (name == "per1") { // pin encoder right 1
      return "pin encoder right 1 = "+String(robotSettings.pinEncoderRight1);
  } else if (name == "pel2") { // pin encoder left 2
      return "pin encoder left 2 = "+String(robotSettings.pinEncoderLeft2);
  } else if (name == "per2") { // pin encoder right 2
      return "pin encoder right 2 = "+String(robotSettings.pinEncoderRight2);
  } else if (name == "er") {
      return "encoder resolution = " + String(robotSettings.encoderResolution);
  } else if (name == "wrad") {
      return "wheel radius = "+String(robotSettings.wheelRadius);
  } else if (name == "wdist") {
      return "distance between wheels = "+String(robotSettings.distanceBetweenWheels);
  } else if (name == "kpa") { // Coefficients of regulators
      return "kpAng = " + String(robot.kpAng);
  } else if (name == "kia") {
      return "kiAng = " + String(robot.kiAng);
  } else if (name == "kda") {
      return "kdAng = " + String(robot.kdAng);
  } else if (name == "kpsl") {
      return "kpSpeedLeft = " + String(robot.kpSpeedLeft);
  } else if (name == "kpsr") {
      return "kpSpeedRight = " + String(robot.kpSpeedRight);
  } else if (name == "kis") {
      return "kiSpeed = " + String(robot.kiSpeed);
  } else if (name == "kdsl") {
      return "kdSpeedLeft = " + String(robot.kdSpeedLeft);
  } else if (name == "kdsr") {
      return "kdSpeedRight = " + String(robot.kdSpeedRight);
  } else if (name == "pled1") {
      return "pin led 1 = " + String(robotSettings.pinLED1);
  } else if (name == "pled2") {
      return "pin led 2 = " + String(robotSettings.pinLED2);
  } else if (name == "pclk") {
      return "pin clk = " + String(robotSettings.pinCLK);
  } else if (name == "psda") {
      return "pin sda = " + String(robotSettings.pinSDA);
  } else if (name="ks") {
    return "pin kStraight = " + String(robotSettings.kStraight);
  } else if (name="maxs") {
    return "pin max speed in rad/s = " + String(robotSettings.maxSpeedRadians);
  } else if (name="pch") {
    return "pin charging module = " + String(robotSettings.pinCharging);
  } else if (name="pbat") {
    return "pin barrery = " + String(robotSettings.pinVoltage);
  } else {
      return "Unknown parameter name!";
  }
}

void ReadSerial(){
  if (Serial.available() > 0) {
    // read until ;
    String input = Serial.readStringUntil(';'); 
    if (input.startsWith("set ")){
        if (input.indexOf("=") != -1) { 
            String coefficient = input.substring(4, input.indexOf("=")); 
            String value = input.substring(input.indexOf("=")+1, -1); 
            Serial.println(setConfig(coefficient, value));
        }

    } else if (input == "get all"){
        printConfig();   
    }
    else if (input == "help"){
        String msg = R"rawliteral(Description: 
        - command format: command; 
        - to get all available parameters and it's values use: "get all;" 
        - to set value use: "set varaible_name=value;". Example: "wssid=MyWifi;" will set wifi ssid to "MyWifi" 
        - IMPORTANT! Remember that your settings will not be saved while you do not save them with specified commands!
        - to save robot settings(robot parameters and coefficients): "save_robot;"
        - to save network settings(wifi and mqtt parameters): "save_network;"
        - to reboot: "reboot;". Also reboot is required for applying network parameters changes!
        )rawliteral";
        Serial.println(msg);
    } else if (input=="reboot") {
        restartESP32();
    } else if (input=="save_network"){
        saveNetworkSettingsToEEPROM(networkSettings);
        Serial.println("Successfully saved network parameters!");
    }
    else if (input=="save_robot"){
        saveRobotSettingsToEEPROM(robotSettings);
        Serial.println("Successfully saved robot parameters!");
    }
    else if (input.length()>1) {
        Serial.print("Your input: ");
        Serial.println(input);
        Serial.println("Unknown command! print \"help;\" to get more info");
    }
  }
}