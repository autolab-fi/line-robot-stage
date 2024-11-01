import json
import time
import paho.mqtt.client as mqtt


topic = ""    # system topic

# Define MQTT broker settings
broker = ""  # broker address
port = 1883   # broke port
broker_password = ""
broker_user = ""


#  JSON message
message = {
    "command": "restart"
}

# create system input topic
input_topic = f"{topic}/input"
# create system output topic
output_topic = f"{topic}/output"

# Create a flag for message received
message_received = False

# Callback for when a message is received
def on_message(client, userdata, msg):
    global message_received
    print("Message received on", msg.topic, ":", msg.payload.decode())
    message_received = True

# Initialize MQTT client
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# set username and password
client.username_pw_set(broker_user,broker_password)

# Set up message handling
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker, port, 60, )

# Subscribe to topic2
client.subscribe(output_topic)

# Start the MQTT client
client.loop_start()

# Publish JSON message to topic1
client.publish(input_topic, json.dumps(message))
print(f"Message sent to {input_topic}: {message}")

end_time = time.time()+10
# Wait for a response on topic2
while not message_received and time.time() < end_time:
    time.sleep(0.1)
if message_received:
    # Confirm that everything works and end program
    print("All works as expected.")
else:
    print("Something went wrong! Message from esp32 was not received!")

client.loop_stop()
client.disconnect()
