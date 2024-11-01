# Basic Sketch Description

The robot is controlled via the MQTT protocol. To understand the system's operation, we recommend familiarizing yourself with this protocol.

## Topic Formats

Assume we have set `MQTT_SYSTEM_TOPIC`= `test/system` and `MQTT_USER_TOPIC` = `test/verification` in the EEPROM settings. The topic formats will be as follows:

- `test/system/input` - system input where commands are sent
- `test/system/output` - system output where the board sends response messages
- `test/verification` - user output, which students can use to send messages from the robot

In the future, `test/verification` may also be divided into `test/verification/input` and `test/verification/output`.

## Command List

1. **Restart the Board**
    ```json
    {
        "command":"restart"
    }
    ```

    Response from ESP

    ```json
    {
        "type":"hello",
        "msg":"Esp32 connected!",
        "version-control-token":"aae8361a-4dad-4434-8357-7dceb7ca6d66"
    }
    ```

2. **Check Battery Status**
    ```json
    {
        "command": "battery-status"
    }
    ```

    Response from ESP
    ```json
    {
        "type":"battery-status",
        "voltage": "27.93", 
        "charging": "true"
    }
    ```

    - `charging` - true - robot is charging / false - robot is not charging
    - `voltage` - battery charge level

3. **Control Robot Movement**
    ```json
    {
        "command":"move",
        "commands":"",
        "values":[]
    }
    ```

    - `commands`: a text field containing a sequence of commands. Allowed symbols: **‘f’, ’b’, ’r’, ’l’**.
        - **‘f’** - move forward
        - **‘b’** - move backward
        - **‘r’** - turn right
        - **‘l’** - turn left
    - `values`: an array of numbers representing the values of the corresponding commands in the sequence of the `commands` string.

    - Example - move forward 20 cm and turn right 45 degrees:
        ```json
        {
            "command":"move",
            "commands":"fr",
            "values":[20,45]
        }
        ```

4. **Start Executing User Code**

    ```json
    {
        "command":"start",
        "set_block":false
    }
    ```

    - `set_block` - boolean, true if you need to block standard library functions. IMPORTANT! If a text “false” is sent, it will not work, as the board will interpret this as a string and assume the value is true.

    Response from ESP
    ```json
    {
        "type":"reset",
        "msg":"Reset completed!"
    }
    ```

    **IMPORTANT!** This command cannot be executed twice consecutively, as the robot requires a restart to run the user code again. Therefore, to re-run the user code, you must first send a command to restart the board.

5. **Move to the Charging Station** (simply moves backward until it detects a signal on the charging pin)

    ```json
    {
        "command":"charge"
    }
    ```

6. **Update Firmware**

    ```json
    {
        "command":"update",
        "version":"token",
        "url":"http://server.compiler/download..."
    }
    ```

    - `url` - link to download the new firmware
    - `version` - version token matching the new code; if the board-side token matches the sent token, the board will simply restart. This token is embedded in the compiled code.

    Response from ESP

    - If the board updates successfully, you will receive a message similar to the restart response, but with the new version token.
    - If something goes wrong, you will receive the following message:

        ```json
        {
            "type":"error",
            "msg":"Error downloading sketch!"
        }
        ```