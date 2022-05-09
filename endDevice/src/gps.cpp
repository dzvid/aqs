/*
  This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
  Base on TinyGPSPlus //https://github.com/mikalhart/TinyGPSPlus
*/

#include "gps.h"

#include <TinyGPS++.h>

HardwareSerial _serial_gps(SERIAL_GPS);

TinyGPSPlus _gps;

static void gps_loop() {
  while (_serial_gps.available()) {
    _gps.encode(_serial_gps.read());
  }

  if (millis() > 5000 && _gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }
}

void gpsSetup() {
  _serial_gps.begin(GPS_BAUDRATE_SERIAL, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

TPOSITION_GPS getGpsData() {
  TPOSITION_GPS currentGpsData;

  while (_serial_gps.available() > 0) {
    if (_gps.encode(_serial_gps.read())) {
      if (_gps.location.isValid() && _gps.date.isValid() && _gps.time.isValid()) {
        currentGpsData.latitude = _gps.location.lat();
        currentGpsData.longitude = _gps.location.lng();
        currentGpsData.year = _gps.date.year();
        currentGpsData.month = _gps.date.month();
        currentGpsData.day = _gps.date.day();
        currentGpsData.hours = _gps.time.hour();
        currentGpsData.minutes = _gps.time.minute();
        currentGpsData.seconds = _gps.time.second();
        currentGpsData.readSuccess = true;
      } else {
        currentGpsData.readSuccess = false;
      }

      return currentGpsData;
    }
  }

  if (millis() > 5000 && _gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    currentGpsData.readSuccess = false;
  }

  return currentGpsData;
}

void displayInfo(TPOSITION_GPS gps_data) {
  if (!gps_data.readSuccess) {
    Serial.print(F("INVALID "));
  }

  Serial.print(F("Location: "));
  Serial.print(gps_data.latitude, 6);
  Serial.print(F(","));
  Serial.print(gps_data.latitude, 6);

  Serial.print(F("  Date/Time: "));
  Serial.print(gps_data.month);
  Serial.print(F("/"));
  Serial.print(gps_data.day);
  Serial.print(F("/"));
  Serial.print(gps_data.year);

  Serial.print(F(" "));
  Serial.print(gps_data.hours);
  Serial.print(F(":"));
  Serial.print(gps_data.minutes);
  Serial.print(F(":"));
  Serial.print(gps_data.seconds);
  // if (gps_data.readSuccess) {
  //   Serial.print(F("Location: "));
  //   Serial.print(gps_data.latitude, 6);
  //   Serial.print(F(","));
  //   Serial.print(gps_data.latitude, 6);

  //   Serial.print(F("  Date/Time: "));
  //   Serial.print(gps_data.month);
  //   Serial.print(F("/"));
  //   Serial.print(gps_data.day);
  //   Serial.print(F("/"));
  //   Serial.print(gps_data.year);

  //   Serial.print(F(" "));
  //   Serial.print(gps_data.hours);
  //   Serial.print(F(":"));
  //   Serial.print(gps_data.minutes);
  //   Serial.print(F(":"));
  //   Serial.print(gps_data.seconds);
  //   Serial.print(F("."));
  // } else {
  //   Serial.print(F("INVALID"));
  // }

  Serial.println();
}
