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

uint8_t getInputFlag(){
	return sendInput_flag;
}

void setInputFlag(){
	sendInput_flag = 0xFF;
}

void Interface_SendHeartbeat(){
	uint8_t data = 0x00;
	CAN_SendMsg(TYPE_HEARTBEAT, &data, 1);
}

void Interface_SendInput(){
	GPIO_SamplePins();

	uint8_t data[6];

	for(int i=0; i < 6; i++){
		data[i] = 0;
	}

	for(int i=0; i < 4; i++){
		data[0] |= ((GPIO_GetPin(DIGITAL, INPUT, i) & 0x01) << i);
	}

	CAN_SendMsg(TYPE_DIGITAL, data, 1);

	data[0] = GPIO_GetPin(ANALOG, INPUT, 0) & 0xFF;
	data[1] = GPIO_GetPin(ANALOG, INPUT, 0) >> 4;
	data[1] |= (GPIO_GetPin(ANALOG, INPUT, 1) & 0x0F) << 4;
	data[2] = GPIO_GetPin(ANALOG, INPUT, 1) >> 4;
	data[3] = GPIO_GetPin(ANALOG, INPUT, 2) & 0xFF;
	data[4] = GPIO_GetPin(ANALOG, INPUT, 2) >> 4;
	data[4] |= (GPIO_GetPin(ANALOG, INPUT, 3) & 0x0F) << 4;
	data[5] = GPIO_GetPin(ANALOG, INPUT, 3) >> 4;

	CAN_SendMsg(TYPE_ANALOG, data, 6);
}
