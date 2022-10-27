#include "modbus.h"

uint8_t device_address = 0;
uint8_t initialized = 0;
Circular_Buffer byte_buffer;
pthread_t rx_thread;

int init_modbus(int _baud_rate, uint8_t address){
    if(init_rs485("/dev/ttyUSB0", _baud_rate, 8, 0, 1, 0)){
        return FAILED_TO_INITIALIZE;
    }

    device_address = address;

    byte_buffer.head = 0;
    byte_buffer.tail = 0;
    byte_buffer.size = 0;
    memset(byte_buffer.buffer, 0, sizeof(byte_buffer.buffer));

    pthread_create(&rx_thread, NULL, rx_bytes, NULL);
    pthread_detach(rx_thread);

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

    delay_us(ceil(28000/getBaudRate()));

    for(int i=0; i < frame_it; i++){
        sendByte(frame_buffer[i]);
    }

    delay_us(ceil(28000/getBaudRate()));
}

uint16_t crc16(uint8_t* data_pointer, int length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    uint8_t bit = 0;

    while (length--){
        /*
        x = crc >> 8 ^ *data_pointer++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
        */
        
        crc = crc ^ *data_pointer++;

        printf("After XOR with data: %X\n",crc);

        int shift_num = 0;

        while(shift_num<8){

            bit = crc & 0x01;

            crc = crc >> 1;
            shift_num++;

            printf("Shift : %X \n",crc);

            if(shift_num<8){
                while(!bit){
                    bit = crc & 0x01;

                    crc = crc >> 1;
                    shift_num++;

                    printf("Extra Shift: %X\n",crc);
                }

                if(shift_num<8){
                    crc = crc ^ 0xA001;
                    printf("XOR with 0xA001: %X\n",crc);
                }
            }
            
/*
            if(bit){
                crc = crc ^ 0xA001;
                printf("XOR with 0xA001: %X\n",crc);
            }else{
                crc = crc >> 1;
                shift_num++;
                printf("Extra Shift: %X\n",crc);
            }
*/
        }
    }

    return crc;
}

void buffer_insert(Buffer_Byte byte){
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

int buffer_pop(Buffer_Byte* byte){
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