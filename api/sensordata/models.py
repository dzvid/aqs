from django.db import models


class SensorData(models.Model):
    id = models.UUIDField(primary_key=True)
    device_name = models.CharField(max_length=255)
    object = models.JSONField(null=True)

    def __str__(self):
        return self.id

    class Meta:
        db_table = 'device_up'
        managed = False
