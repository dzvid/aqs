from rest_framework import routers

from sensordata.viewsets import SensorDataViewSet

router_api = routers.DefaultRouter()
router_api.register(r'sensordata', SensorDataViewSet)
