#ifndef _PMS_SENSOR_H_
#define _PMS_SENSOR_H_

#include <Arduino.h>

#include "PMS.h"

#define PMS_HARDWARE_SERIAL 2
#define PMS_PIN_RX 2
#define PMS_PIN_TX 13

typedef struct {
  bool readSuccess = false;
  uint16_t pm2_5;
  uint16_t pm10;
} TPMS_DATA;

class PmsSensor {
 public:
  PmsSensor();
  void init();
  TPMS_DATA getReading();
  void displayData(TPMS_DATA pmsData);
};

#endif /* _PMS_SENSOR_H_ */
