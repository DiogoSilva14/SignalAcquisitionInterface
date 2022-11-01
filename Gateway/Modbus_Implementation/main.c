#include <stdio.h>
#include <pthread.h>
#include "modbus.h"

pthread_t print_thread;

void *print_registers(void* varg){  
    while(1){
        printf("======== DIGITAL REGISTERS ========\n");
        for(int i=0; i < 4; i++){
            printf("Register %d: %X\n", i, get_digital_register(i));
        }
        printf("======== ANALOG REGISTERS ========\n");
        for(int i=0; i < 4; i++){
            printf("Register %d: %X\n", i, get_analog_register(i));
        }

        usleep(1000000);
    }
}

int main(int argc, char *argv[]){

    if(argc < 4){
        printf("Usage: ./modbus_implementation_DEBUG serial-port baud-rate device_address\n");

        return -1;
    }

    if(init_modbus(argv[1], atoi(argv[2]), atoi(argv[3]))){
        printf("Error initializing Modbus driver\n");

        return -1;
    }

    pthread_create(&print_thread, NULL, print_registers, NULL);
    pthread_detach(print_thread);

    uint8_t register_n;
    uint8_t new_val;

    while(1){
        scanf("%d", &register_n);
        scanf("%d", &new_val);

        set_digital_register(register_n, new_val);
    }

    return 0;
}