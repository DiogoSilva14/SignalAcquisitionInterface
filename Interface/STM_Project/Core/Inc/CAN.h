#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <main.h>
#include <Interface.h>

#define MESSAGE_MAX_SIZE 20
#define BUFFER_MAX_SIZE 10

typedef struct{
	uint16_t header;
	uint8_t data[MESSAGE_MAX_SIZE];
	uint8_t length;
} CAN_Message;

typedef struct{
	uint8_t head;
	uint8_t tail;
	CAN_Message data[BUFFER_MAX_SIZE];
	uint8_t size;
} CircularBuffer;

uint8_t CAN_Init(uint8_t address);
uint8_t CAN_Start();
uint8_t CAN_SendMsg(uint8_t typeIdentifier, uint8_t* data, uint8_t length);
uint8_t CAN_getRxFlag();
void CAN_setRxFlag();
void CAN_unsetRxFlag();
uint8_t CAN_popMessage(CAN_Message* message);
uint8_t CAN_putMessage(uint16_t header, uint8_t* data, uint8_t length);

#endif /* INC_CAN_H_ */
