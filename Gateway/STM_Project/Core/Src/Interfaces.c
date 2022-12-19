#include <Interfaces.h>

static volatile Interface Interfaces[INTERFACES_NUMBER];

void Interfaces_Init(){
	for(uint8_t i=0; i < INTERFACES_NUMBER; i++){
		for(uint8_t ii=0; ii < DIGITAL_INPUTS; i++){
			Interfaces[i].digitalInputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < DIGITAL_OUTPUTS; i++){
			Interfaces[i].digitalOutputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < ANALOG_INPUTS; i++){
			Interfaces[i].analogInputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < ANALOG_OUTPUTS; i++){
			Interfaces[i].analogOutputRegisters[ii] = 0;
		}
	}
}

uint8_t getDigitalInput(uint8_t deviceAddress, uint8_t inputNumber){
	return Interfaces[deviceAddress].digitalInputRegisters[inputNumber];
}

void setDigitalOutput(uint8_t deviceAddress, uint8_t inputNumber, uint8_t value){
	Interfaces[deviceAddress].digitalOutputRegisters[inputNumber] = value;
}

uint16_t getAnalogInput(uint8_t deviceAddress, uint8_t inputNumber){
	return Interfaces[deviceAddress].analogInputRegisters[inputNumber];
}

void getAnalogOutput(uint8_t deviceAddress, uint8_t inputNumber, uint16_t value){
	Interfaces[deviceAddress].analogOutputRegisters[inputNumber] = value;
}
