from datetime import datetime, timedelta

from django.http import JsonResponse
from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.pagination import PageNumberPagination
from rest_framework.status import HTTP_200_OK

# from sensordata.filters import SensorDataFilter
from sensordata.models import SensorData
from sensordata.serializers import SensorDataSerializer, SensorsSerializer


class PaginationNone(PageNumberPagination):
    page_size = None


class SensorDataViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = SensorData.objects.all()
    serializer_class = SensorDataSerializer
    # filterset_class = SensorDataFilter
    permission_classes = []

    @action(methods=['get'], detail=False)
    def list_sensors(self, request, *args, **kwargs):
        sensors = self.get_queryset().values('device_name').distinct()
        serializer = SensorsSerializer(sensors, many=True)
        return self.get_paginated_response(serializer.data)

    @action(methods=['get'], detail=False)
    def get_readings(self, request, *args, **kwargs):
        id_sensor = request.query_params.get('id_sensor', None)
        date = request.query_params.get('date', None)

        start_date = datetime.strptime(date, '%Y-%m-%dT%H:%M:%S.%fZ')
        end_date = start_date + timedelta(days=1)

        readings = self.get_queryset() \
            .filter(device_name__iexact=id_sensor) \
            .order_by('object__data__dt_collected_at')

        readings_in_date = []
        for reading in readings:
            if reading.object is not None:
                data = reading.object.get('data', None)
                reading_date = datetime.strptime(data['dt_collected_at'], '%Y-%m-%dT%H:%M:%SZ')
                if start_date < reading_date < end_date:
                    readings_in_date.append(reading)

        serializer = SensorDataSerializer(readings_in_date, many=True)
        return JsonResponse(serializer.data, status=HTTP_200_OK, safe=False)
