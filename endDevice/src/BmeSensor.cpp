#include "BmeSensor.h"

BmeSensor::BmeSensor() {}

void BmeSensor::init() {
  // BME
  if (!_bme.begin(BME280_I2C_ADDR)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }
}
void BmeSensor::getReading() {
  Serial.print("Temperature = ");
  Serial.print(_bme.readTemperature());
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(_bme.readPressure() / 100.0F);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(_bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(_bme.readHumidity());
  Serial.println("%");

  Serial.println();
  delay(1000);
}