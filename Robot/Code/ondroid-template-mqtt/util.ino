// convert any type tp char*
template <typename T>
char* convertToChar(T value) {
    // Create a String object from the value
    String strValue = String(value);

    // Allocate memory for the char array
    char* charArray = new char[strValue.length() + 1];

    // Copy the String to the char array
    strValue.toCharArray(charArray, strValue.length() + 1);

    return charArray;
}

// function for moving robot to any point
void func_move_robot(){
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 1);
  for (int i=0; i<commands.length();i++){
    Serial.print(commands[i]);
    Serial.println(moveValues[i]);
    switch(commands[i]){
      // move forward
      case 'f':
        robot.moveForwardDistance(moveValues[i]);
        break;
      // move backward 
      case 'b':
        robot.moveBackwardDistance(moveValues[i]);
        break;
      // turn right
      case 'r':
        robot.turnRightAngle(moveValues[i]);
        break;
      // turn left
      case 'l':
        robot.turnLeftAngle(moveValues[i]);
        break;
      default:
        break;
    }
  }
  // turn off leds
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
  // send message that reset complted
  sendMessageSystem("{\"type\":\"reset\", \"msg\":\"Reset completed!\"}");
}

void initRobot(){
  robot = lineRobot(robotSettings.pinMotorLeft1,
    robotSettings.pinMotorLeft2,
    robotSettings.pinMotorRight1,
    robotSettings.pinMotorRight2, 
    robotSettings.pinEncoderLeft1, 
    robotSettings.pinEncoderLeft2,
    robotSettings.pinEncoderRight1,
    robotSettings.pinEncoderRight2,
    robotSettings.wheelRadius, 
    robotSettings.distanceBetweenWheels, 
    robotSettings.encoderResolution,
    robotSettings.kpAng,
    robotSettings.kiAng,
    robotSettings.kdAng,
    robotSettings.kpSpeedLeft ,
    robotSettings.kpSpeedRight,
    robotSettings.kdSpeedLeft,
    robotSettings.kdSpeedRight,
    robotSettings.kiSpeed,
    robotSettings.kStraight,
    robotSettings.maxSpeedRadians
  );
  robot.begin();
}


void readVoltage(){
  int16_t adc0, adc1;
  float voltage_charging;
  int64_t Adc0_temp = 0;
  int64_t Adc1_temp = 0;
  for(uint8_t i = 0;i<32; i++){
    adc0 = adc.readADC_SingleEnded(robotSettings.pinVoltage);
    adc1 = adc.readADC_SingleEnded(robotSettings.pinCharging);
    Adc0_temp += adc0;
    Adc1_temp += adc1;
  }
  Serial.print("Adc0_temp: ");
  Serial.println(Adc0_temp);
  Serial.print("Adc1_temp: ");
  Serial.println(Adc1_temp);
  adc0 = Adc0_temp / 32;
  adc1 = Adc1_temp / 32;
  Serial.print("adc0: ");
  Serial.println(adc0);
  Serial.print("adc1: ");
  Serial.println(adc1);
  voltage_charging = adc.computeVolts(adc1);
  voltage = adc.computeVolts(adc0)*k_voltage;
  charging = voltage_charging > 1;
  Serial.print("voltage: ");
  Serial.println(voltage);
  Serial.print("charging: ");
  Serial.println(charging);
  // Serial.println("-----------------------------------------------------------");
  // Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V");
  // Serial.print("AIN1: "); Serial.print(adc1); Serial.print("  "); Serial.print(volts1); Serial.println("V");

}


// func for moving backward while not charing with timeout 10 seconds
void moveCharging(){
  long endTime = millis()+10000;
  while (!charging && millis()<endTime){ 
    robot.moveBackwardDistance(7);
    readVoltage();
  }
  String resp = "{\"type\":\"battery-status\", \"voltage\": \""+String(voltage)+"\", \"charging\": \""+ch+"\"}";
  sendMessageSystem(resp);
}


// func to restart esp32
void restartESP32(){
  //digitalWrite(enPin, 1);
  //delay(1);
  //digitalWrite(enPin, 0);
  ESP.restart();
}