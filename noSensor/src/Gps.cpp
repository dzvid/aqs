/*
  This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
  Base on TinyGPSPlus //https://github.com/mikalhart/TinyGPSPlus
*/

#include "Gps.h"

#include <SparkFun_Ublox_Arduino_Library.h>

HardwareSerial _serial_gps(HARDWARE_SERIAL_GPS);

SFE_UBLOX_GPS _gps;

Gps::Gps() {}

void Gps::init() {
  _serial_gps.begin(GPS_BAUDRATE_SERIAL, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(1000);

  do {
    if (_gps.begin(_serial_gps)) {
      Serial.println("Connected to GPS");
      _gps.setUART1Output(COM_TYPE_NMEA);  // Set the UART port to output NMEA only
      _gps.saveConfiguration();            // Save the current settings to flash and BBR
      Serial.println("GPS serial connected, output set to NMEA");
      _gps.disableNMEAMessage(UBX_NMEA_GLL, COM_PORT_UART1);
      _gps.disableNMEAMessage(UBX_NMEA_GSA, COM_PORT_UART1);
      _gps.disableNMEAMessage(UBX_NMEA_GSV, COM_PORT_UART1);
      _gps.disableNMEAMessage(UBX_NMEA_VTG, COM_PORT_UART1);
      _gps.disableNMEAMessage(UBX_NMEA_RMC, COM_PORT_UART1);
      _gps.enableNMEAMessage(UBX_NMEA_GGA, COM_PORT_UART1);
      _gps.saveConfiguration();  // Save the current settings to flash and BBR
      Serial.println("Enabled/disabled NMEA sentences");
      break;
    }
    delay(1000);
  } while (1);

  Serial.println("Finished GPS setup.");
}

TGPS_DATA Gps::getCurrentDate() {
  TGPS_DATA currentGpsData;

  currentGpsData.readSuccess = _gps.getTimeValid() && _gps.getDateValid();
  currentGpsData.year = _gps.getYear();
  currentGpsData.month = _gps.getMonth();
  currentGpsData.day = _gps.getDay();
  currentGpsData.hours = _gps.getHour();
  currentGpsData.minutes = _gps.getMinute();
  currentGpsData.seconds = _gps.getSecond();

  return currentGpsData;
}

void Gps::displayData(TGPS_DATA gpsData) {
  if (gpsData.readSuccess) {
    Serial.println("GPS:: Valid data.");
  } else {
    Serial.println("GPS:: Invalid data.");
  }

  char currentDate[21] = {0};

  sprintf(currentDate, "%04d-%02d-%02dT%02d:%02d:%02dZ",
          gpsData.year,
          gpsData.month,
          gpsData.day,
          gpsData.hours,
          gpsData.minutes,
          gpsData.seconds);
  Serial.print(F("Current Date: "));
  Serial.print(currentDate);
  Serial.println();
}
