#include "rs485.h"

// RS-485 line baud rate
int baud_rate = 0;

int init_rs485(int baud_rate){
    baud_rate = baud_rate;

    return 0;
}

int sendByte(uint8_t byte_to_send){
    if(baud_rate == 0){
        return RS485_NOT_INITIALIZED;
    }

    printf("%i\n", byte_to_send);

    return 0;
}

int getByte(uint8_t block){
    return 0x01;
}
