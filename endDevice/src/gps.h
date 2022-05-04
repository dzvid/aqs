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

#define I2C_SDA 21
#define I2C_SCL 22
#define PMU_IRQ 35

#define RADIO_SCLK_PIN 5
#define RADIO_MISO_PIN 19
#define RADIO_MOSI_PIN 27
#define RADIO_CS_PIN 18
#define RADIO_DI0_PIN 26
#define RADIO_RST_PIN 23
#define RADIO_DIO1_PIN 33
#define RADIO_BUSY_PIN 32

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

bool initPMU();
void initBoard();

static void gps_loop();
void gpsSetup();
TPOSITION_GPS getGpsData();
void displayInfo(TPOSITION_GPS gps_data);

#endif /* _GPS_H_ */
