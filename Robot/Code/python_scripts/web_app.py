from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, emit
import json
import paho.mqtt.client as mqtt

# Define Flask app and SocketIO
app = Flask(__name__)
socketio = SocketIO(app)


# MQTT topics
topic = ""
# Define MQTT broker settings
broker = ""  # broker address
port = 1883   # broke port
broker_password = ""
broker_user = ""
# Define camera ip addtess
camera_ip = ""




video_url = f"{camera_ip}/cam/stream.m3u8"
publish_topic = f"{topic}/input"
output_topic = f"{topic}/output"
subscribe_topics = [publish_topic, output_topic]

# Message history storage
message_history = []

# Initialize MQTT client
mqtt_client = mqtt.Client()
mqtt_client.username_pw_set(broker_user, broker_password)

# MQTT message received callback
def on_message(client, userdata, msg):
    message = {
        "topic": msg.topic,
        "payload": msg.payload.decode()
    }
    # Save message to history
    message_history.append(message)
    # Send the message to frontend
    socketio.emit("mqtt_message", message)

# Connect and subscribe to topics
def setup_mqtt():
    mqtt_client.on_message = on_message
    mqtt_client.connect(broker, port, 60)
    for topic in subscribe_topics:
        mqtt_client.subscribe(topic)
    mqtt_client.loop_start()

# Send command to topic1
@app.route('/send_command', methods=['POST'])
def send_command():
    command = request.json
    mqtt_client.publish(publish_topic, json.dumps(command))
    return jsonify({"status": "Command sent", "command": command})

# Get message history
@app.route('/history', methods=['GET'])
def get_history():
    return jsonify(message_history)

# Serve frontend
@app.route('/')
def index():
    return render_template('index.html', video_url=video_url, system_input=publish_topic,system_output=output_topic)

# Run the app and initialize MQTT client
if __name__ == '__main__':
    setup_mqtt()
    socketio.run(app, host="0.0.0.0", port=5000)
