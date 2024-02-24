import paho.mqtt.client as mqtt
import json

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    client.subscribe("wss/iot/data")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload)
        result = str(data)                           
    except: 
        result = str(msg.payload)
    print("\n\nNew Message!\n_______________")
    print("topic:: " + msg.topic)
    print("message::\n" + result)

mqttClient = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttClient.on_connect = on_connect
mqttClient.on_message = on_message
# mqttClient.username_pw_set("username", "password")
mqttClient.connect("broker.emqx.io", 1883, 60)
mqttClient.loop_start()
try:
    while True:
        topic = input("enter topic: ")
        message = input("enter message: ")
        mqttClient.publish(topic, message)
        print("\n\n")
except:
    print("exiting")
    mqttClient.disconnect()
    mqttClient.loop_stop()