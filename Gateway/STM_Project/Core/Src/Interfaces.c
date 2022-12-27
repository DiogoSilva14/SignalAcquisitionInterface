#include <Interfaces.h>

static volatile Interface Interfaces[INTERFACES_NUMBER];

void Interfaces_Init(){
	for(uint16_t i=0; i < INTERFACES_NUMBER; i++){
		for(uint8_t ii=0; ii < DIGITAL_INPUTS; ii++){
			Interfaces[i].digitalInputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < DIGITAL_OUTPUTS; ii++){
			Interfaces[i].digitalOutputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < ANALOG_INPUTS; ii++){
			Interfaces[i].analogInputRegisters[ii] = 0;
		}

		for(uint8_t ii=0; ii < ANALOG_OUTPUTS; ii++){
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

void setAnalogOutput(uint8_t deviceAddress, uint8_t inputNumber, uint16_t value){
	Interfaces[deviceAddress].analogOutputRegisters[inputNumber] = value;
}

void Interfaces_processCANMessages(){
	CAN_Message message;

	while(!CAN_popMessage(&message)){
		uint8_t messageType = (message.header & 0x300) >> 8;
		uint8_t deviceAddress = message.header & 0xFF;

		if(messageType == TYPE_ANALOG_INPUT){
			for(int i=0; i < ANALOG_INPUTS; i++){
				Interfaces[deviceAddress].analogInputRegisters[i] = (message.data[i] | (message.data[i+1] << 8)) & 0x4FF;
			}
		}
		if(messageType == TYPE_DIGITAL_INPUT){
			for(int i=0; i < DIGITAL_INPUTS; i++){
				if(message.data[0] & (1 << i)){
					Interfaces[deviceAddress].digitalInputRegisters[i] = 0xFF;
				}else{
					Interfaces[deviceAddress].digitalInputRegisters[i] = 0x00;
				}
			}
		}
	}
}
