import json

from django.http import HttpResponse, HttpResponseNotAllowed, JsonResponse
from django.core.exceptions import ObjectDoesNotExist
from django.views.decorators.csrf import csrf_exempt

from .models import Data, Relay
from .client import client


def data(request):
    data_queryset = list(reversed(Data.objects.all()[:50]))
    relay = Relay.objects.get_current_value()
    data = {
        "relay": relay,
        "wifi": data_queryset[0].wifi if data_queryset else None,
        "temperature": [instance.temperature for instance in data_queryset],
        "humidity": [instance.humidity for instance in data_queryset],
        "light": [instance.light for instance in data_queryset],
    }

    return JsonResponse(data)


@csrf_exempt
def relay(request):
    if request.method == "POST":
        body = json.loads(request.body)
        relay = body["relay"]
        client.publish_relay(relay)

        return HttpResponse()

    return HttpResponseNotAllowed(["POST"])
