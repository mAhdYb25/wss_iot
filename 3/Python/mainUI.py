import paho.mqtt.client as mqtt
import json
from tkinter import *
import time

def turnOnRelay():
    x = {"relay": True}
    y = json.dumps(x)
    mqttClient.publish("wss/iot/command", y)
def turnOffRelay():
    x = {"relay": False}
    y = json.dumps(x)
    mqttClient.publish("wss/iot/command", y)

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    client.subscribe("wss/iot/data")

def on_subscribe(client, userdata, mid, reason_code_list, properties):
    if reason_code_list[0].is_failure:
        print(f"Broker rejected you subscription: {reason_code_list[0]}")
    else:
        print(f"Broker granted the following QoS: {reason_code_list[0].value}")
        
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload)
        tmpVal['text'] = str(round(data['temperature'],2)) + "°C"
        humVal['text'] = str(round(data['humidity'],2)) + "%"
        lightVal['text'] = str(data['light']) + " out of 4095"
        relayVal['text'] = "on" if data['relay'] else "off"
        wifiVal['text'] = data['wifi'] 
        named_tuple = time.localtime() # get struct_time
        time_string = time.strftime("%m/%d/%Y, %H:%M:%S", named_tuple)
        t['text'] = "Last updated time: " + time_string
        result = str(data)                           
    except: 
        result = str(msg.payload)
    print("\n\nNew Message!\n_______________")
    print("topic:: " + msg.topic)
    print("message::\n" + result)

mqttClient = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttClient.on_connect = on_connect
mqttClient.on_message = on_message
mqttClient.on_subscribe = on_subscribe
# mqttClient.username_pw_set("username", "password")

mqttClient.connect("broker.emqx.io", 1883, 60)
mqttClient.loop_start()

root =Tk()
p=Label(root, text="WSS ESP32 Through MQTT", font=("Arial", 15))
p.grid(row=0,column=0)
tmpLabel=Label(root, text="Temperature", fg="white", bg="red", height=4, font=("Arial", 15) , width=20)
tmpVal=Label(root, text="0°C", fg="white", bg="red", height=4, font=("Arial", 15) , width=20)
tmpLabel.grid(row=1,column=0)
tmpVal.grid(row=1,column=1)
humLabel=Label(root, text="Humidity", fg="white", bg="blue", height=4, font=("Arial", 15) , width=20)
humVal=Label(root, text="0%", fg="white", bg="blue", height=4, font=("Arial", 15) , width=20)
humLabel.grid(row=2,column=0)
humVal.grid(row=2,column=1)
lightLabel=Label(root, text="Light", fg="white", bg="orange", height=4, font=("Arial", 15) , width=20)
lightVal=Label(root, text="0", fg="white", bg="orange", height=4, font=("Arial", 15) , width=20)
lightLabel.grid(row=3,column=0)
lightVal.grid(row=3,column=1)
wifiLabel=Label(root, text="WiFi Network", fg="white", bg="green", height=4, font=("Arial", 15) , width=20)
wifiVal=Label(root, text="Name", fg="white", bg="green", height=4, font=("Arial", 15) , width=20)
wifiLabel.grid(row=4,column=0)
wifiVal.grid(row=4,column=1)
relayLabel=Label(root, text="Relay", fg="white", bg="magenta", height=4, font=("Arial", 15) , width=20)
relayVal=Label(root, text="n/a", fg="white", bg="magenta", height=4, font=("Arial", 15) , width=20)
relayLabel.grid(row=5,column=0)
relayVal.grid(row=5,column=1)
relayOn=Button(root, text="Turn On",relief=RAISED, height=4, font=("Arial", 15) , width=20, command=turnOnRelay)
relayOff=Button(root, text="Turn Off",relief=RAISED, height=4, font=("Arial", 15) , width=20, command=turnOffRelay)
relayOn.grid(row=6,column=0)
relayOff.grid(row=6,column=1)
t=Label(root, text="Last updated time: null", font=("Arial", 15))
t.grid(row=7,column=0,columnspan=2)
root.mainloop()

try:
    while True:
        command = input()
        mqttClient.publish("wss/iot/command", command)
except:
    print("exiting")
    mqttClient.disconnect()
    mqttClient.loop_stop()