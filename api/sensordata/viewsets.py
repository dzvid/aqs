from datetime import datetime, timedelta

from django.http import JsonResponse
from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.pagination import PageNumberPagination
from rest_framework.status import HTTP_200_OK

# from sensordata.filters import SensorDataFilter
from sensordata.models import SensorData
from sensordata.serializers import SensorDataSerializer, SensorsSerializer
from sensordata.utils import get_average, get_pm25_interval_and_concentration, get_pm25_aiq, get_pm10_aiq


class SensorDataViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = SensorData.objects.all()
    serializer_class = SensorDataSerializer
    # filterset_class = SensorDataFilter
    permission_classes = []

    @action(methods=['get'], detail=False)
    def list_sensors(self, request, *args, **kwargs):
        sensors = self.get_queryset().values('device_name').distinct()
        page = self.paginate_queryset(sensors)
        serializer = SensorsSerializer(page, many=True)
        return self.get_paginated_response(serializer.data)

    @action(methods=['get'], detail=False)
    def get_readings(self, request, *args, **kwargs):
        # medidas de um dia escolhido
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

    @action(methods=['get'], detail=False)
    def get_aqi_last24hrs(self, request, *args, **kwargs):
        # ultimas 24 horas
        id_sensor = request.query_params.get('id_sensor', None)
        date = request.query_params.get('date', None)

        start_date = datetime.strptime(date, '%Y-%m-%dT%H:%M:%S.%fZ')
        end_date = start_date + timedelta(days=-1)

        readings = self.get_queryset() \
            .filter(device_name__iexact=id_sensor) \
            .order_by('object__data__dt_collected_at')

        # 1 - Pegar 24 horas de leitura para pm25 e pm10
        readings_in_interval = []
        for reading in readings:
            if reading.object is not None:
                data = reading.object.get('data', None)
                reading_date = datetime.strptime(data['dt_collected_at'], '%Y-%m-%dT%H:%M:%SZ')
                if start_date > reading_date > end_date:
                    readings_in_interval.append(data)

        # 2 - Calcular media do periodo para cada poluente (pm25_avg_measured_pollution)
        pm25_24hrs_avg = get_average(readings=readings_in_interval, pollutant='pm2_5')

        # 3 - Calcular media do periodo para cada poluente (pm10_avg_measured_pollution)
        pm10_24hrs_avg = get_average(readings=readings_in_interval, pollutant='pm10')

        # 4 - Calculo AIQ para poluentes
        pm25_calculated_aiq = get_pm25_aiq(pm25_average_measured_conc=pm25_24hrs_avg)
        pm10_calculated_aiq = get_pm10_aiq(pm10_average_measured_conc=pm10_24hrs_avg)
        aiq = pm25_calculated_aiq \
            if pm25_calculated_aiq['aiq'] >= pm10_calculated_aiq['aiq'] \
            else pm10_calculated_aiq

        data = {
            'aiq': aiq,
            'pm25': pm25_calculated_aiq,
            'pm10': pm10_calculated_aiq,
            'interval': {'start_date': start_date, 'end_date': end_date}
        }

        return JsonResponse(data, status=HTTP_200_OK, safe=False)
