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


// get battery status. calculate voltage on resistor
float getVoltage(){
  if (robotMove){
    return voltage;
  }
  int A0Value = analogRead(voltagePin);
  voltage = (float)A0Value * (3.3 / 4096.0) * ( 1 + 10.0)*k_voltage;
  return voltage;
}
// get status of charing
bool charging(){
  return (analogRead(chargingPin))>5;
}
// func for moving backward while not charing with timeout 10 seconds
void moveCharging(){
  long endTime = millis()+10000;
  while (!charging() && millis()<endTime){ 
    robot.moveBackwardDistance(7);
  }
}

// func to restart esp32
void restartESP32(){
  //digitalWrite(enPin, 1);
  //delay(1);
  //digitalWrite(enPin, 0);
  ESP.restart();
}