#ifndef INC_INTERFACES_H_
#define INC_INTERFACES_H_

#include <main.h>
#include <stdint.h>
#include <CAN.h>

#define DIGITAL_INPUTS 4
#define DIGITAL_OUTPUTS 4
#define ANALOG_INPUTS 4
#define ANALOG_OUTPUTS 2

// Mudar de define para enum
#define TYPE_HEARTBEAT 0x00
#define TYPE_DIGITAL_INPUT 0x01
#define TYPE_DIGITAL_OUTPUT 0x02
#define TYPE_ANALOG_INPUT 0x03
#define TYPE_ANALOG_OUTPUT 0x04

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
void setAnalogOutput(uint8_t deviceAddress, uint8_t inputNumber, uint16_t value);
void Interfaces_updateOutput(uint8_t deviceAddress);
void Interfaces_processCANMessages();

#endif /* INC_INTERFACES_H_ */
