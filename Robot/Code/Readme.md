# Setting up ESP32 for the Line-Robot Course

[![en](https://img.shields.io/badge/lang-en-red.svg)](Readme.md)
[![ru](https://img.shields.io/badge/lang-ru-red.svg)](Readme.ru.md)

## Table of Contents

- [**Folder Description**](#folder-description)
- [**Step-by-Step Instructions**](#step-by-step-instructions)
- [**Uploading the Main Executable Sketch**](#uploading-the-sketch)
- [**Writing Data to EEPROM**](#writing-data-to-eeprom)
- [**Python Script with Verification**](#python-script-with-verification)
- [**WEB Interface for Control**](#web-interface-for-control)
- [**Alternative Robot Control Options**](#alternative-robot-control-options)


## Directory Description

- `ondroid-template-mqtt` - base sketch for the course  
- `docs/template.md` - description of the base sketch
- `python_scripts` - directory with Python scripts, where `test_mqtt.py` is a script for testing the ESP32, and `web_app.py` is a web application for real-time robot control


## Step-by-Step Instructions

1. Upload the `ondroid-template-mqtt` sketch. [More details](#uploading-the-sketch)
2. Write configuration data to the device's memory using UART. [More details](#writing-data-to-eeprom)
3. Test using the Python script `test_mqtt.py`. [More details](#writing-data-to-eeprom)


### Uploading the Sketch

1. Download the libraries: 
	- [ArduinoJson](https://docs.arduino.cc/libraries/arduinojson/https://github.com/bblanchon/ArduinoJson) - for working with JSON
	- [PubSubClient](https://docs.arduino.cc/libraries/pubsubclient) - MQTT client
	- [Adafruit ADS1X15](https://docs.arduino.cc/libraries/adafruit-ads1x15) - for reading data from the ADC
	- [LineRobot](https://github.com/autolab-fi/LineRobotLibrary) - the Ondroid library for the line robot, available only on GitHub
2. Upload the `ondroid-template-mqtt` sketch to the ESP32

### Writing Data to EEPROM

When uploading the sketch, there will be an attempt to connect to a WiFi network. The connection attempt will last for 7 seconds. After a failed attempt, the board will reboot to skip the WiFi connection and enter offline mode. **Send "y" to Serial**. If the ESP32 successfully connects to WiFi, **serial commands will also be available**.

Command format: each command is written in Serial and ends with a ";" character.

List of commands:

- "help" - display a list of commands with descriptions
- "get all" - display a list of all parameters and their values
- "set *name*=*value*" - set the value *value* for the parameter with the name *name*. The *name* should be the short name of the parameter, which is indicated in parentheses when calling the "get all" function.
- "save_robot" - save robot parameters.
- "save_network" - save network settings.
- "reboot" - reboot the board

Important! To apply the settings, you must save the changes with the "save_network" and/or "save_robot" commands, and then reboot the board.

Example of WiFi setup:

1. Connect the board via USB
2. Open the serial monitor
3. If you see an attempt to connect to a WiFi network, send "y".
4. Set the WiFi SSID using the command "set wssid=MyWiFi;", where MyWiFi is the name of your WiFi network.
5. Set the WiFi password using the command "set wpas=password;", where password is the password for your WiFi network.
6. Save the settings with the command "save_network;"
7. Reboot the microcontroller with the command "reboot;"
8. Wait for the network connection during boot. If the connection fails, double-check the entered data.

### Python Script with Verification

1. Enter the broker connection data in the Python script `test_mqtt.py`, located in the `python_scripts` folder. 
2. Create a Python virtual environment and activate it (optional)
3. Install the requirements `pip install -r python_scripts/requirements.txt`.
4. Run the script `test_mqtt.py`. If you see the output: **All works as expected.** - everything is working correctly.


## WEB Interface for Control

In the Python folder, there is also a simple web application for real-time robot control. To use it:

1. Enter the broker connection data in the Python script `web_app.py`, located in the `python_scripts` folder. You can also enter the IP address of the camera there, so you can see the live stream on the web page.
2. Create a Python virtual environment and activate it (optional)
3. Install the requirements `pip install -r python_scripts/requirements.txt`.
4. Run the script `web_app.py`
5. In your browser, go to `http://localhost:5000/` and you will see the web interface

Before using it, we recommend reading the documentation on the template, located in `docs/template.md`, where you can learn more about the commands that can be sent to the robot.

![alt text](<docs/web_interface.png>)


## Alternative Robot Control Options

You can use any MQTT client to control the robot, but you will need to study how to work with it yourself. The documentation on the commands for the robot is available [here](docs/template.ru.md).