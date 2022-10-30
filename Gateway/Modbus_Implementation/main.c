#include <stdio.h>
#include "modbus.h"

int main(){
    printf("Hello world!\n");

    uint8_t buffer[] = {0x02,0x10,0x00,0x64,0x00,0x05};

    uint16_t crc = crc16(buffer,6);

    printf("%X\n", crc);

    return 0;
}
