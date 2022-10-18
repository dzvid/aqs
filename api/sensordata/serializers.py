from rest_framework import serializers

from sensordata.models import SensorData


class SensorDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = SensorData
        fields = '__all__'

class SensorsSerializer(serializers.ModelSerializer):
    class Meta:
        model = SensorData
        fields = ['device_name']