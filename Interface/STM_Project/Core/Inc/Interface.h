#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#define TYPE_HEARTBEAT 0x00
#define TYPE_DIGITAL 0x01
#define TYPE_ANALOG 0x02

void Interface_SendHeartbeat();
void Interface_SendInput();

#endif /* INC_INTERFACE_H_ */
