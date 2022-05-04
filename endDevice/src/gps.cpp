/*
  This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
  Base on TinyGPSPlus //https://github.com/mikalhart/TinyGPSPlus
*/

#include "gps.h"

#include <TinyGPS++.h>
#include <axp20x.h>

HardwareSerial _serial_gps(SERIAL_GPS);

AXP20X_Class PMU;
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
  TPOSITION_GPS current_gps_data;

  while (_serial_gps.available() > 0) {
    if (_gps.encode(_serial_gps.read())) {
      if (_gps.location.isValid() && _gps.date.isValid() && _gps.time.isValid()) {
        current_gps_data.latitude = _gps.location.lat();
        current_gps_data.longitude = _gps.location.lng();
        current_gps_data.year = _gps.date.year();
        current_gps_data.month = _gps.date.month();
        current_gps_data.day = _gps.date.day();
        current_gps_data.hours = _gps.time.hour();
        current_gps_data.minutes = _gps.time.minute();
        current_gps_data.seconds = _gps.time.second();
        current_gps_data.readSuccess = true;
      } else {
        current_gps_data.readSuccess = false;
      }

      return current_gps_data;
    }
  }

  if (millis() > 5000 && _gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    current_gps_data.readSuccess = false;
  }

  return current_gps_data;
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

bool initPMU() {
  Wire.begin(I2C_SDA, I2C_SCL);

  if (PMU.begin(Wire, AXP192_SLAVE_ADDRESS) == AXP_FAIL) {
    return false;
  }
  /*
   * The charging indicator can be turned on or off
   * * * */
  // PMU.setChgLEDMode(LED_BLINK_4HZ);

  /*
   * The default ESP32 power supply has been turned on,
   * no need to set, please do not set it, if it is turned off,
   * it will not be able to program
   *
   *   PMU.setDCDC1Voltage(3300);
   *   PMU.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
   *
   * * * */

  /*
   *   Turn off unused power sources to save power
   * **/
  PMU.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
  PMU.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
  PMU.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
  PMU.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);

  /*
   * Set the power of LoRa and GPS module to 3.3V
   **/
  PMU.setLDO2Voltage(3300);  // LoRa VDD
  PMU.setLDO3Voltage(3300);  // GPS  VDD

  PMU.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  PMU.setPowerOutPut(AXP192_LDO3, AXP202_ON);

  return true;
}

void initBoard() {
  Serial.println(F("initBoard"));
  //   Serial1.begin(GPS_BAUDRATE_SERIAL, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_CS_PIN);
  Serial.println("AXP192 initialized...");
  bool energyStartup = initPMU();

  if (energyStartup) {
    Serial.println("AXP192 initialized...OK");
  } else {
    Serial.println("AXP192 initialized...FAIL");
  }

  // When the power is turned on, a delay is required.
  delay(1500);
}