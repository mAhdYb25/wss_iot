import json
import os

import paho.mqtt.client as mqtt
from dotenv import load_dotenv
from rich import print
from typer import Typer

load_dotenv()

app = Typer()


def connect(on_connect=None, on_message=None) -> mqtt.Client:
    """Create a client and connect to MQTT broker."""
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    if on_connect:
        client.on_connect = on_connect
    if on_message:
        client.on_message = on_message

    broker_host = os.getenv("MQTT_BROKER_HOST", "127.0.0.1")
    client.connect(broker_host)

    return client


@app.command()
def publish(topic: str, key: str, value: str):
    """Publish a single key and value JSON to MQTT broker on the topic."""
    data = json.dumps({key: value})
    client = connect()
    client.loop_start()
    client.publish(topic, data)
    client.loop_stop()


@app.command()
def subscribe(topic: str):
    """Subscribe to MQTT broker topic and print all the messages."""

    def on_connect(client, data, flags, reason, properties):
        client.subscribe(topic)

    def on_message(client, data, message):
        payload = json.loads(message.payload)
        print(payload)

    client = connect(on_connect=on_connect, on_message=on_message)
    client.loop_forever()


if __name__ == "__main__":
    app()
