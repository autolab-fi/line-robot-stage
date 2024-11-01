// function to send user's messages to mqtt channel
template <typename T>
void print(T value){
  char* charArray = convertToChar(value);
  client.publish(userTopicOutput.c_str(), charArray);
}
// function to send system's messages to mqtt channel
template <typename T>
void sendMessageSystem(T value){
 char* charArray = convertToChar(value);
 client.publish(systemTopicOutput.c_str(), charArray);
}

// function callback for messages in mqtt channel
void callback(char *topic, byte *payload, unsigned int length) {
  // display message
  Serial.print("Topic: ");
  Serial.println(topic);
  if (topic==systemTopicInput){
    Serial.print("Message: ");
    String data = "";
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);  // Convert each byte to a char and append to the String
      data+=(char)payload[i];
    }
    Serial.println();
    Serial.println("------------");
     // Create a StaticJsonDocument
    StaticJsonDocument<300> doc;
    // Parse the JSON string
    DeserializationError error = deserializeJson(doc, data);
    // Check if parsing succeeded
    if (error) {
      String msg = "{\"type\":\"error\",\"msg\":\""+String(error.c_str())+"\"}";
      Serial.println(msg);
      sendMessageSystem(msg);
      return;
    }
    // get battery info: battery voltage and charging
    if (strcmp(doc["command"], "battery-status") == 0) {
      String ch = "false";
      if (charging()>0){
        ch="true";
      }
      String resp = "{\"type\":\"battery-status\", \"voltage\": \""+String(getVoltage())+"\", \"charging\": \""+ch+"\"}";
      sendMessageSystem(resp);
      return;
    }
    // move to specified point
    if (strcmp(doc["command"], "move") == 0) {
      // write commands to global variable for the second thread
      commands = String(doc["commands"]);
      // write values for commands to global array 
      for (int i=0; i<commands.length();i++){
        moveValues[i] = doc["values"][i];
      }
      // start robot move func on the second thread
      robotMove = true;
      return;
    }
    // start user's code
    if (strcmp(doc["command"], "start") == 0) {
      if (doc["set_block"]){
        robot.setBlockTrue();
      }
      robotStart = true;
      return;
    }
    // restart esp32
    if (strcmp(doc["command"], "restart") == 0) {
      restartESP32();
      return;
    }
    // move to charging station
    if (strcmp(doc["command"], "charge") == 0) {
      robotCharging = true;
      return;
    }
    // download new firmware
    if (strcmp(doc["command"], "update") == 0) {
      // if token of sketch is same as current it is mean code does not required to update!
      if (strcmp(doc["version"], getVersionToken().c_str()) == 0){
        ESP.restart();
        return;
      }
      //HTTPClient netclient;
      updateInProgress = true;
      const char* url = doc["url"];
      Serial.println(url);
      t_httpUpdate_return ret = httpUpdate.update(espClient, url);
      switch (ret) {
          case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            break;
     
          case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;
     
          case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            //delay(1000); // Wait a second and restart
            ESP.restart();
            break;
        }
      updateInProgress = false;
      // Read settings from EEPROM
      if (!client.connected()){
        Serial.println("Reconnect to the broker!");
        NetworkSettings networkSettings = readNetworkSettingsFromEEPROM();
        connectBroker(networkSettings);
      }
      sendMessageSystem("{\"type\":\"error\", \"msg\":\"Error downloading sketch!\"}");
      return;
    }
  }
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}


void update_progress(int cur, int total) {
  uint8_t current_percentage = (cur * 100) / total;
  Serial.printf("CALLBACK: HTTP update process at %d%% (%d of %d bytes)...\n", current_percentage, cur, total);
}


void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}



