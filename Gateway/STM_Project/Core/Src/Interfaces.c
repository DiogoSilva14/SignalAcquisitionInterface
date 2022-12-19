#include <Interfaces.h>

static volatile Interface[256];

uint8_t getDigitalInput(uint8_t deviceAddress, uint8_t inputNumber){
	return Interface[deviceAddress]->digitalInputRegisters[inputNumber];
}

void setDigitalOutput(uint8_t deviceAddress, uint8_t inputNumber, uint8_t value){
	Interface[deviceAddress]->digitalOutputRegisters[inputNumber] = value;
}

uint16_t getAnalogInput(uint8_t deviceAddress, uint8_t inputNumber){
	return Interface[deviceAddress]->analogInputRegisters[inputNumber];
}

void getAnalogOutput(uint8_t deviceAddress, uint8_t inputNumber, uint16_t value){
	Interface[deviceAddress]->analogOutputRegisters[inputNumber] = value;
}
