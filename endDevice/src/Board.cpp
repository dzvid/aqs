#include "Board.h"

#include <SPI.h>
#include <Wire.h>
#include <axp20x.h>

AXP20X_Class PMU;

Board::Board() {}

bool Board::initPMU() {
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
   *
   *   PMU.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
   *   PMU.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
   *   PMU.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
   *   PMU.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);
   *
   * **/

  PMU.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  PMU.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  PMU.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  PMU.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  PMU.setPowerOutPut(AXP192_DCDC1, AXP202_ON);

  /*
   * Set the power of LoRa and GPS module to 3.3V
   **/
  PMU.setDCDC1Voltage(3300);  // 3.3V pin
  PMU.setLDO2Voltage(3300);   // LoRa VDD
  PMU.setLDO3Voltage(3300);   // GPS  VDD

  return true;
}

void Board::init() {
  Serial.begin(115200);
  Serial.println(F("initBoard"));
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_CS_PIN);
  Serial.println("AXP192 initialized...");
  bool energyStartup = Board::initPMU();

  if (energyStartup) {
    Serial.println("AXP192 initialized...OK");
  } else {
    Serial.println("AXP192 initialized...FAIL");
    while (1)
      ;
  }

  // When the power is turned on, a delay is required.
  delay(1500);
}