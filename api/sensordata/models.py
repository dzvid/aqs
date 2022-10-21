from django.contrib.postgres.fields import JSONField
from django.db import models


class SensorData(models.Model):
    id = models.UUIDField(primary_key=True)
    device_name = models.CharField(max_length=255)
    object = JSONField()

    def __str__(self):
        return self.device_name

    class Meta:
        db_table = 'device_up'
        managed = False
