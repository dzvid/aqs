#ifndef _BME_SENSOR_H_
#define _BME_SENSOR_H_

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

#define BME280_I2C_ADDR 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

typedef struct {
  bool readSuccess = false;
  float temperature;
  float pressure;
  float humidity;
} TBME_DATA;

class BmeSensor {
 public:
  BmeSensor();
  void init();
  TBME_DATA getReading();
  void displayData(TBME_DATA bmeData);

 private:
  Adafruit_BME280 _bme;
};

#endif /* _BME_SENSOR_H_ */
