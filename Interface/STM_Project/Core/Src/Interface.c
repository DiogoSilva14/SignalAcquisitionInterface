#include <Interface.h>
#include <CAN.h>
#include <IO.h>

static volatile uint8_t heartbeat_flag = 0x00;
static volatile uint8_t sendInput_flag = 0x00;

uint8_t getHeartbeatFlag(){
	return heartbeat_flag;
}

void setHeartbeatFlag(){
	heartbeat_flag = 0xFF;
}

void unsetHeartbeatFlag(){
	heartbeat_flag = 0x00;
}

uint8_t getInputFlag(){
	return sendInput_flag;
}

void setInputFlag(){
	sendInput_flag = 0xFF;
}

void unsetInputFlag(){
	sendInput_flag = 0x00;
}

void Interface_SendHeartbeat(){
	uint8_t data = 0x00;
	CAN_SendMsg(TYPE_HEARTBEAT, &data, 1);
}

void Interface_SendInput(){
	GPIO_SamplePins();

	uint8_t data[8];

	for(int i=0; i < 8; i++){
		data[i] = 0;
	}

	for(int i=0; i < 4; i++){
		data[0] |= ((GPIO_GetPin(DIGITAL, INPUT, i) & 0x01) << i);
	}

	CAN_SendMsg(TYPE_DIGITAL_INPUT, data, 1);

	data[0] = GPIO_GetPin(ANALOG, INPUT, 0) & 0xFF;
	data[1] = GPIO_GetPin(ANALOG, INPUT, 0) >> 8;
	data[2] = GPIO_GetPin(ANALOG, INPUT, 1) & 0xFF;
	data[3] = GPIO_GetPin(ANALOG, INPUT, 1) >> 8;
	data[4] = GPIO_GetPin(ANALOG, INPUT, 2) & 0xFF;
	data[5] = GPIO_GetPin(ANALOG, INPUT, 2) >> 8;
	data[6] = GPIO_GetPin(ANALOG, INPUT, 3) & 0xFF;
	data[7] = GPIO_GetPin(ANALOG, INPUT, 3) >> 8;

	CAN_SendMsg(TYPE_ANALOG_INPUT, data, 8);
}

void Interface_processCANMessages(){
	CAN_Message message;

	while(!CAN_popMessage(&message)){
		uint8_t messageType = (message.header >> 8) & 0x07;

		if(messageType == TYPE_ANALOG_OUTPUT){
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			for(int i=0; i < ANALOG_OUT_PINS; i++){
				uint16_t value = (message.data[i] | (message.data[i+1] << 8)) & 0xFFF;
				GPIO_SetPin(ANALOG, OUTPUT, i, value);
			}
		}
		if(messageType == TYPE_DIGITAL_OUTPUT){
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			for(int i=0; i < DIGITAL_OUT_PINS; i++){
				GPIO_SetPin(DIGITAL, OUTPUT, i, message.data[0] & (1 << i));
			}
		}
	}

	GPIO_ApplyOutput();
}
