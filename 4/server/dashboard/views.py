from django.shortcuts import render

from mqtt.models import Relay


def dashboard(request):
    relay = Relay.objects.get_current_value()
    context = {"relay": relay}
    return render(request, "dashboard.html", context)
