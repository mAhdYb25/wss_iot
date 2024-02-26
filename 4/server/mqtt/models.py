from django.core.exceptions import ObjectDoesNotExist
from django.db import models


class Data(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    light = models.IntegerField()
    wifi = models.CharField(max_length=256)
    timestamp = models.DateTimeField(auto_now=True)

    class Meta:
        indexes = [models.Index(fields=["timestamp"])]
        ordering = ["-timestamp"]

    def __str__(self):
        return f"@{self.timestamp!s}"


class RelayManager(models.Manager):
    def get_current_value(self):
        try:
            return super().latest().value
        except ObjectDoesNotExist:
            return None


class Relay(models.Model):
    value = models.BooleanField()
    timestamp = models.DateTimeField(auto_now=True)

    objects = RelayManager()

    class Meta:
        indexes = [models.Index(fields=["timestamp"])]
        get_latest_by = "timestamp"

    def __str__(self):
        return f"@{self.timestamp!s}"
