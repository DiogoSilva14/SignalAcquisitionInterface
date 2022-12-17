#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <main.h>

#define DEVICE_ADDRESS 0x01

uint8_t CAN_Init();
uint8_t CAN_Start();
uint8_t CAN_SendMsg(uint8_t* data, uint8_t length);

#endif /* INC_CAN_H_ */