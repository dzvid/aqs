from django_filters import FilterSet

from sensordata.models import SensorData


class SensorDataFilter(FilterSet):
    class Meta:
        model = SensorData
        fields = '__all__'
