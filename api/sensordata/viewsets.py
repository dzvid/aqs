from rest_framework import viewsets
from rest_framework.decorators import action

from sensordata.filters import SensorDataFilter
from sensordata.models import SensorData
from sensordata.serializers import SensorDataSerializer, SensorsSerializer


class SensorDataViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = SensorData.objects.all()
    serializer_class = SensorDataSerializer
    filterset_class = SensorDataFilter
    permission_classes = []

    @action(methods=['get'], detail=False)
    def list_sensors(self, request, *args,  **kwargs):
        sensors = self.get_queryset().values('device_name').distinct()
        page = self.paginate_queryset(sensors)
        serializer = SensorsSerializer(page, many=True)
        return self.get_paginated_response(serializer.data)
