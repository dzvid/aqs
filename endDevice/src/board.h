#ifndef _BOARD_H_
#define _BOARD_H_

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

bool initPMU();
void initBoard();

#endif /* _BOARD_H_ */
