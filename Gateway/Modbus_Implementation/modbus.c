#include "modbus.h"

uint8_t device_address = 0;
uint8_t initialized = 0;
int baud_rate = 0;

int init_modbus(int baud_rate, uint8_t address){
    if(init_rs485(baud_rate)){
        return FAILED_TO_INITIALIZE;
    }

    device_address = address;
    baud_rate = baud_rate;

    initialized = 1;

    return 0;
}

int send_frame(uint8_t destination_address, uint8_t function, uint8_t* data_pointer, uint8_t data_length){
    if(!initialized){
        return NOT_INITIALIZED;
    }

    uint8_t frame_buffer[data_length + 3];
    uint8_t frame_it = 0;

    frame_buffer[frame_it++] = destination_address;
    frame_buffer[frame_it++] = function;

    for(int i=0; i < data_length; i++){
        frame_buffer[frame_it++] = data_pointer[i];
    }

    uint16_t crc = crc16(frame_buffer, frame_it);

    frame_buffer[frame_it++] = crc & 0xFF;
    frame_buffer[frame_it++] = crc >> 16;

    delay_us(ceil(28000/baud_rate));

    for(int i=0; i < frame_it; i++){
        sendByte(frame_buffer[i]);
    }

    delay_us(ceil(28000/baud_rate));
}

uint16_t crc16(const uint16_t* data_pointer, int length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }

    return crc;
}