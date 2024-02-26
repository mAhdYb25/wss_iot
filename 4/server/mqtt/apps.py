from django.apps import AppConfig


class MqttConfig(AppConfig):
    default_auto_field = "django.db.models.BigAutoField"
    name = "mqtt"

    def ready(self) -> None:
        from .client import client
        client.loop_start()
