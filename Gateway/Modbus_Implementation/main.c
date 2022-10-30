#include <stdio.h>
#include "modbus.h"

int main(){
    printf("Hello world!\n");

    uint8_t buffer[] = {0x03,0x01,0x00,0x00,0x00,0x05,0xFD};

    uint16_t crc = crc16(buffer,7);

    printf("%X\n", crc);

    return 0;
}
