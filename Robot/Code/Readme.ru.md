# Настройка esp32 для курса Line-Robot 

[![en](https://img.shields.io/badge/lang-en-red.svg)](Readme.md)
[![ru](https://img.shields.io/badge/lang-ru-red.svg)](Readme.ru.md)

## Содержание

- [**Описание папки**](#описание-папки)
- [**Инструкция по шагам**](#инструкция-по-шагам)
- [**Запись данных в eeprom**](#запись-данных-в-eeprom)
- [**Загрузка основного исполняемого скетча**](#загрузка-основного-исполняемого-скетча)
- [**Python script с проверкой**](#python-script-с-проверкой)
- [**WEB интерфейс для управления**](#web-интерфейс-для-управления)
- [**Альтернативные варианты управления роботом**](#альтернативные-варианты-управления-роботом)


## Описание директории

- `ondroid-write-eeprom-settings` - скетч для записи настроек в память eeprom
- `ondroid-template-mqtt` - базовый скетч для курса  
- `docs/template.md` - описание базового скетча
- `python_scripts` - директория с python скриптами, где `test_mqtt.py` - скрипт для проверки работы esp32, `web_app.py` - веб приложение для управления роботом в режиме реального времени


## Инструция по шагам

1. Вписать конфигурационные данные в скетч `ondroid-write-eeprom-settings` и загрузить на esp32. [Подробнее](#Запись-данных-в-eeprom)
2. Загрузить скетч `ondroid-template-mqtt`. [Подробнее](#Запись-данных-в-eeprom)
3. Протестировать с помощью python скрипта `test_mqtt.py`. [Подробнее](#Запись-данных-в-eeprom)


### Запись данных в eeprom

1. Запустите 

2. Запросите следующие данные:

	- `MQTT_IP` - IP адрес брокера в формате ipv4(4 однобайтных числа)
	- `MQTT_PORT` - порт для подключения брокера сообщений
	- `MQTT_LOGIN` - Логин для подключения к брокеру 
	- `MQTT_PASSWORD` - Пароль для подключения к брокеру
	- `MQTT_SYSTEM_TOPIC` - Системный топик esp32
	- `MQTT_USER_TOPIC` - Пользовательский топик esp32 для сообщений 

3. Впишите полученные данные и данные для подключения к WIFI в `ondroid-write-eeprom-settings.ino`: 

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

4. Загрузите скетч на esp32.
	
	В serial будет выведена информация о записанных в память данных


### Загрузка основного исполняемого скетча

1. Скачайте библиотеки: 
	- [ArduinoJson](https://docs.arduino.cc/libraries/arduinojson/https://github.com/bblanchon/ArduinoJson) - работа с json
	- [PubSubClient](https://docs.arduino.cc/libraries/pubsubclient) - mqtt клиент
	- [Adafruit ADS1X15](https://docs.arduino.cc/libraries/adafruit-ads1x15) - для чтения данных с АЦП
	- [LineRobot](https://github.com/autolab-fi/LineRobotLibrary) - библиотека ondroid для робота линии и доступна только на github
2. Загрузите скетч `ondroid-template-mqtt` на esp32


### Python script с проверкой

1. Внесите данные для подключения к брокеру в python script `test_mqtt.py`, который находится в папке `python_scripts`. 
2. Создайте виртуальное окружение для питона и запустите(опционально)
3. Установите requirements `pip install -r python_scripts/requirements.txt`.
4. Запустите скрипт `test_mqtt.py`, если вы увидели в выводе: **All works as expected.** - значит всё работает правильно


## WEB интерфейс для управления

В папке python также находится простое web приложение для управления роботом в реальном времени. Для этого:

1. Внесите данные для подключения к брокеру в python script `web_app.py`, который находится в папке `python_scripts`. Также вы можете туда внести ip адрес камеры, таким образом вы сможете видеть стрим в реальном времени на веб странице.
2. Создайте виртуальное окружение для питона и запустите(опционально)
3. Установите requirements `pip install -r python_scripts/requirements.txt`.
4. Запустите скрипт `web_app.py`
5. В браузере, зайдите на страницу `http://localhost:5000/` и вы увидите веб интерфейс

Перед использованием рекомендуем прочитать документацию по template, которая находится в `docs/template.md`, там вы сможете больше узнать о командах, которые могут быть отправлены на робота.

![alt text](<docs/web_interface.png>)


## Альтернативные варианты управления роботом

Вы можете использовать любой mqtt клиент для управления роботом, но соответственно вам необходимо будет самостоятельно изучить работу с ним. А документация по командам для робота находится [здесь](docs/template.ru.md)

