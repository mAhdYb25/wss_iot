from django.core.serializers.json import DjangoJSONEncoder

from .models import Data

class DataJSONEncoder(DjangoJSONEncoder):
    def default(self, obj):
        if isinstance(obj, Data):
            return {
                "temperature": obj.temperature
            }
        
        return super().default(obj)