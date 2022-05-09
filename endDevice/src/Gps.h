#ifndef _GPS_H_
#define _GPS_H_

#include <Arduino.h>
#include <SPI.h>

#define GPS_BAUDRATE_SERIAL 115200
// GPS IS CONNECTED TO TTGO TBEAM SERIAL "1"
#define SERIAL_GPS 1
#define GPS_RX_PIN 34
#define GPS_TX_PIN 12
#define BUTTON_PIN 38
#define BUTTON_PIN_MASK GPIO_SEL_38

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
} TPOSITION_GPS;

class Gps {
 public:
  Gps();
  void init();
  static void gps_loop();
  TPOSITION_GPS getGpsData();
  void displayInfo(TPOSITION_GPS gps_data);
};

#endif /* _GPS_H_ */
