# Instructions for Setting up ESP32 for Line-Robot Course

[![en](https://img.shields.io/badge/lang-en-red.svg)](Readme.md)
[![ru](https://img.shields.io/badge/lang-ru-red.svg)](Readme.ru.md)

## Contents

- [**Folder Description**](#folder-description)
- [**Step-by-Step Instructions**](#step-by-step-instructions)
- [**Writing Data to EEPROM**](#writing-data-to-eeprom)
- [**Uploading the Main Sketch**](#uploading-the-main-sketch)
- [**Python Script for Testing**](#python-script-for-testing)
- [**Web Interface for Control**](#web-interface-for-control)
- [**Alternative Robot Control Options**](#alternative-robot-control-options)

## Folder Description

- `ondroid-write-eeprom-settings` - sketch for writing settings to EEPROM memory
- `ondroid-template-mqtt` - basic sketch for the course
- `docs/template.md` - description of the basic sketch
- `python_scripts` - folder containing Python scripts, where `test_mqtt.py` is used to test ESP32 functionality, and `web_app.py` is a web app for real-time robot control

## Step-by-Step Instructions

1. Enter configuration data in the `ondroid-write-eeprom-settings` sketch and upload it to the ESP32. [More details](#writing-data-to-eeprom)
2. Upload the `ondroid-template-mqtt` sketch. [More details](#uploading-the-main-sketch)
3. Test using the Python script `test_mqtt.py`. [More details](#python-script-for-testing)

### Writing Data to EEPROM

1. Run the sketch.
2. Provide the following data:

	- `MQTT_IP` - IP address of the broker in IPv4 format (four 1-byte numbers)
	- `MQTT_PORT` - port for broker connection
	- `MQTT_LOGIN` - login for broker connection
	- `MQTT_PASSWORD` - password for broker connection
	- `MQTT_SYSTEM_TOPIC` - system topic for ESP32
	- `MQTT_USER_TOPIC` - user topic for ESP32 messages

3. Enter the obtained data, along with the Wi-Fi connection information, in `ondroid-write-eeprom-settings.ino`:

	```cpp
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
	```

4. Upload the sketch to ESP32. The serial monitor will display information about the data written to memory.

### Uploading the Main Sketch

1. Download the libraries:
	- [ArduinoJson](https://docs.arduino.cc/libraries/arduinojson/https://github.com/bblanchon/ArduinoJson) - for working with JSON
	- [PubSubClient](https://docs.arduino.cc/libraries/pubsubclient) - MQTT client
	- [Adafruit ADS1X15](https://docs.arduino.cc/libraries/adafruit-ads1x15) - for reading data from the ADC
	- [LineRobot](https://github.com/autolab-fi/LineRobotLibrary) - Ondroid library for line robot, available only on GitHub
2. Upload the `ondroid-template-mqtt` sketch to the ESP32.

### Python Script for Testing

1. Enter the broker connection details in the Python script `test_mqtt.py` located in the `python_scripts` folder.
2. Optionally, create and activate a Python virtual environment.
3. Install the requirements with `pip install -r python_scripts/requirements.txt`.
4. Run the script `test_mqtt.py`. If you see the output: **All works as expected.** - everything is functioning correctly.

## Web Interface for Control

In the Python folder, there is a simple web app for real-time robot control. To use it:

1. Enter the broker connection details in the Python script `web_app.py`, located in `python_scripts`. You can also add the camera's IP address here to enable real-time streaming on the web page.
2. Optionally, create and activate a Python virtual environment.
3. Install the requirements with `pip install -r python_scripts/requirements.txt`.
4. Run the script `web_app.py`.
5. In your browser, go to `http://localhost:5000/` to access the web interface.

Before using, we recommend reading the template documentation located in `docs/template.md` to learn more about commands that can be sent to the robot.

![alt text](<docs/web_interface.png>)

## Alternative Robot Control Options

You can use any MQTT client to control the robot, but you will need to study how to work with it on your own. The documentation on commands for the robot is available [here](docs/template.md).