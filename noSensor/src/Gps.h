#ifndef _GPS_H_
#define _GPS_H_

#include <Arduino.h>
#include <SPI.h>

#define GPS_BAUDRATE_SERIAL 9600
// GPS IS CONNECTED TO TTGO TBEAM SERIAL "1"
#define HARDWARE_SERIAL_GPS 1
#define GPS_RX_PIN 34
#define GPS_TX_PIN 12

typedef struct
{
  bool readSuccess = false;
  float latitude = 0;
  float longitude = 0;
  int year = 0;
  int month = 0;
  int day = 0;
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
} TGPS_DATA;

class Gps {
 public:
  Gps();
  void init();
  TGPS_DATA getCurrentDate();
  void displayData(TGPS_DATA gpsData);
};

#endif /* _GPS_H_ */
