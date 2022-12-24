#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include <main.h>
#include <stdint.h>

// Mudar de define para enum
#define TYPE_HEARTBEAT 0x00
#define TYPE_DIGITAL_INPUT 0x01
#define TYPE_DIGITAL_OUTPUT 0x02
#define TYPE_ANALOG_INPUT 0x03
#define TYPE_ANALOG_OUTPUT 0x04

void Interface_SendHeartbeat();
void Interface_SendInput();
void setInputFlag();
void unsetInputFlag();
uint8_t getInputFlag();
void setHeartbeatFlag();
void unsetHeartbeatFlag();
uint8_t getHeartbeatFlag();
void Interface_processCANMessages();

#endif /* INC_INTERFACE_H_ */
