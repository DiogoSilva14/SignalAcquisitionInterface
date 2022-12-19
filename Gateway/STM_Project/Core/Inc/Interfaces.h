#ifndef INC_INTERFACES_H_
#define INC_INTERFACES_H_

#include <main.h>
#include <stdint.h>

#define DIGITAL_INPUTS 4
#define DIGITAL_OUTPUTS 4
#define ANALOG_INPUTS 4
#define ANALOG_OUTPUTS 2

#define INTERFACES_NUMBER 255

typedef struct Interface{
	uint8_t digitalInputRegisters[DIGITAL_INPUTS];
	uint8_t digitalOutputRegisters[DIGITAL_OUTPUTS];
	uint16_t analogInputRegisters[ANALOG_INPUTS];
	uint16_t analogOutputRegisters[ANALOG_OUTPUTS];
} Interface;

void Interfaces_Init();
uint8_t getDigitalInput(uint8_t deviceAddress, uint8_t inputNumber);
void setDigitalOutput(uint8_t deviceAddress, uint8_t inputNumber, uint8_t value);
uint16_t getAnalogInput(uint8_t deviceAddress, uint8_t inputNumber);
void getAnalogOutput(uint8_t deviceAddress, uint8_t inputNumber, uint16_t value);

#endif /* INC_INTERFACES_H_ */
