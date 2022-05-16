/*
  This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
  Base on TinyGPSPlus //https://github.com/mikalhart/TinyGPSPlus
*/

#include "Gps.h"

#include <TinyGPS++.h>

HardwareSerial _serial_gps(SERIAL_GPS);

TinyGPSPlus _gps;

Gps::Gps() {}

void Gps::init() {
  _serial_gps.begin(GPS_BAUDRATE_SERIAL, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

TGPS_DATA Gps::getReading() {
  TGPS_DATA currentGpsData;

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

void Gps::displayData(TGPS_DATA gpsData) {
  if (gpsData.readSuccess) {
    Serial.print(F("Location: "));
    Serial.print(gpsData.latitude, 6);
    Serial.print(F(","));
    Serial.print(gpsData.latitude, 6);

    Serial.print(F("  Date/Time: "));
    Serial.print(gpsData.month);
    Serial.print(F("/"));
    Serial.print(gpsData.day);
    Serial.print(F("/"));
    Serial.print(gpsData.year);

    Serial.print(F(" "));
    Serial.print(gpsData.hours);
    Serial.print(F(":"));
    Serial.print(gpsData.minutes);
    Serial.print(F(":"));
    Serial.print(gpsData.seconds);
    Serial.println();
  } else {
    Serial.println("GPS:: No data.");
    Serial.print(F("Location: "));
    Serial.print(gpsData.latitude, 6);
    Serial.print(F(","));
    Serial.print(gpsData.latitude, 6);

    Serial.print(F("  Date/Time: "));
    Serial.print(gpsData.month);
    Serial.print(F("/"));
    Serial.print(gpsData.day);
    Serial.print(F("/"));
    Serial.print(gpsData.year);

    Serial.print(F(" "));
    Serial.print(gpsData.hours);
    Serial.print(F(":"));
    Serial.print(gpsData.minutes);
    Serial.print(F(":"));
    Serial.print(gpsData.seconds);
    Serial.println();
  }
}
