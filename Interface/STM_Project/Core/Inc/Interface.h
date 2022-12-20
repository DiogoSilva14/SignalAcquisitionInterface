#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include <main.h>
#include <stdint.h>

#define TYPE_HEARTBEAT 0x00
#define TYPE_DIGITAL 0x01
#define TYPE_ANALOG 0x02

void Interface_SendHeartbeat();
void Interface_SendInput();
void setInputFlag();
void unsetInputFlag();
uint8_t getInputFlag();
void setHeartbeatFlag();
void unsetHeartbeatFlag();
uint8_t getHeartbeatFlag();

#endif /* INC_INTERFACE_H_ */
