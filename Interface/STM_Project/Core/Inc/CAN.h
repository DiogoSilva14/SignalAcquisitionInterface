#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <main.h>
#include <Interface.h>

uint8_t CAN_Init(uint8_t address);
uint8_t CAN_Start();
uint8_t CAN_SendMsg(uint8_t typeIdentifier, uint8_t* data, uint8_t length);
uint8_t CAN_getRxFlag();
void CAN_setRxFlag();
void CAN_unsetRxFlag();

#endif /* INC_CAN_H_ */
