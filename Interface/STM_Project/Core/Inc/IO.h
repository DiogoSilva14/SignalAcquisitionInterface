#ifndef INC_IO_H_
#define INC_IO_H_

#include <main.h>

#define DIGITAL_IN_PINS 1
#define DIGITAL_OUT_PINS 0
#define ANALOG_IN_PINS 0
#define ANALOG_OUT_PINS 0

#define ANALOG 0
#define DIGITAL 1
#define OUTPUT 0
#define INPUT 1

uint8_t GPIO_Init();
void GPIO_SamplePins();
uint16_t GPIO_GetPin(uint8_t type, uint8_t inout, uint8_t number);
void LED_ON();
void LED_OFF();

#endif /* INC_IO_H_ */
