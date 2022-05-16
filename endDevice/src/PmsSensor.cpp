#include "PmsSensor.h"

HardwareSerial _serial(2);
PMS _pms(_serial);
PMS::DATA _pmsData;

PmsSensor::PmsSensor() {}

void PmsSensor::init() {
  _serial.begin(9600, SERIAL_8N1, PMS_PIN_RX, PMS_PIN_TX);
  _pms.passiveMode();  // Switch to passive mode
}

TPMS_DATA PmsSensor::getReading() {
  Serial.println("Waking up, wait 30 seconds for stable readings...");
  _pms.wakeUp();
  delay(30000);
  Serial.println("Send read request...");
  _pms.requestRead();

  TPMS_DATA reading;

  if (_pms.readUntil(_pmsData)) {
    // Serial.print("PM 1.0 (ug/m3): ");
    // Serial.println(_pmsData.PM_AE_UG_1_0);

    // Serial.print("PM 2.5 (ug/m3): ");
    // Serial.println(_pmsData.PM_AE_UG_2_5);

    // Serial.print("PM 10.0 (ug/m3): ");
    // Serial.println(_pmsData.PM_AE_UG_10_0);

    reading.readSuccess = true;
    reading.pm2_5 = _pmsData.PM_AE_UG_2_5;
    reading.pm10 = _pmsData.PM_AE_UG_10_0;
  } else {
    // Serial.println("No data.");
    reading.readSuccess = false;
  }
  _pms.sleep();
  delay(2000);
  Serial.println("PMS:: Sleep mode.");
  return reading;
}