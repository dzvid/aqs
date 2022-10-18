from rest_framework import viewsets
from rest_framework.decorators import action

from sensordata.filters import SensorDataFilter
from sensordata.models import SensorData
from sensordata.serializers import SensorDataSerializer


class SensorDataViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = SensorData.objects.all()
    serializer_class = SensorDataSerializer
    filterset_class = SensorDataFilter
    permission_classes = []

    @action(methods=['get'], detail=False)
    def list_sensors(self, request, *args,  **kwargs):
        return self.get_queryset().values('device_name').distinct()
