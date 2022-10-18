from django.http import JsonResponse
from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.status import HTTP_200_OK

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
        data = SensorsSerializer(sensors, many=True).data
        return JsonResponse(data=data, status=HTTP_200_OK)
