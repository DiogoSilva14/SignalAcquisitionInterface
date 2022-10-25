#include "modbus.h"

uint8_t device_address = 0;
uint8_t initialized = 0;
int baud_rate = 0;
Circular_Buffer byte_buffer;
pthread_t rx_thread;

int init_modbus(int baud_rate, uint8_t address){
    if(init_rs485(baud_rate)){
        return FAILED_TO_INITIALIZE;
    }

    device_address = address;
    baud_rate = baud_rate;

    byte_buffer.head = 0;
    byte_buffer.tail = 0;
    byte_buffer.size = 0;
    memset(byte_buffer.buffer, 0, sizeof(byte_buffer.buffer));

    pthread_create(&rx_thread, NULL, rx_bytes, NULL);
    pthread_detach(rx_thread, NULL);

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

void buffer_insert(Circular_Buffer byte){
    byte_buffer.buffer[byte_buffer.head++] = byte;
    
    if(byte_buffer.size == CIRCULAR_BUFFER_SIZE){
        byte_buffer.tail++;
    }else{
        byte_buffer.size++;
    }

    if(byte_buffer.head >= CIRCULAR_BUFFER_SIZE){
        byte_buffer.head = 0;
    }
}

int buffer_pop(Circular_Buffer* byte){
    if(byte_buffer.size == 0){
        return BUFFER_FULL;
    }

    *byte = byte_buffer.buffer[byte_buffer.tail++];

    byte_buffer.size--;

    if(byte_buffer.tail >= CIRCULAR_BUFFER_SIZE){
        byte_buffer.tail = 0;
    }
}

void *rx_bytes(void* varg){
    uint8_t rx_byte = 0;
    struct timespec ts;
    Buffer_Byte byte_struct;

    while(1){
        if(getByte(&rx_byte)){
            timespec_get(&ts, TIME_UTC);
            
            byte_struct.byte = rx_byte;
            byte_struct.timestamp = ts.tv_nsec;

            buffer_insert(byte_struct);
        }
        
        delay_us(1);
    }
}