#ifndef _BME_SENSOR_H_
#define _BME_SENSOR_H_

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

#define BME280_I2C_ADDR 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

class BmeSensor {
 public:
  BmeSensor();
  void init();
  void getReading();

 private:
  Adafruit_BME280 _bme;
};

#endif /* _BME_SENSOR_H_ */
