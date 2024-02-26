import json
from django.conf import settings
from django.db import transaction
import paho.mqtt.client as mqtt

from .models import Data, Relay


class Client(mqtt.Client):
    def on_connect(self, client, data, flags, reason, properties):
        client.subscribe(settings.MQTT_DATA_TOPIC)

    def on_message(self, client, data, message):
        payload = json.loads(message.payload)
        temperature = round(payload["temperature"], 2)
        humidity = round(payload["humidity"], 2)
        light = payload["light"]
        wifi = payload["wifi"]
        relay = payload["relay"]
        with transaction.atomic():
            Data.objects.create(
                temperature=temperature, humidity=humidity, light=light, wifi=wifi
            )
            Relay.objects.create(value=relay)

    def publish_relay(self, relay):
        payload = json.dumps({"relay": relay})
        message_info = self.publish(settings.MQTT_COMMAND_TOPIC, payload)
        message_info.wait_for_publish()
        Relay.objects.create(value=relay)


def connect() -> Client:
    """Create a client and connect to MQTT broker."""
    print("MQTT_BROKER_HOST", settings.MQTT_BROKER_HOST)
    client = Client(mqtt.CallbackAPIVersion.VERSION2)

    client.connect(settings.MQTT_BROKER_HOST)

    return client


client = connect()
