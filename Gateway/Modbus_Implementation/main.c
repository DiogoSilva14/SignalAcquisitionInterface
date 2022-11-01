#include <stdio.h>
#include "modbus.h"

int main(int argc, char *argv[]){

    if(argc < 4){
        printf("Usage: ./modbus_implementation_DEBUG serial-port baud-rate device_address\n");

        return -1;
    }

    if(init_modbus(argv[1], atoi(argv[2]), atoi(argv[3]))){
        printf("Error initializing Modbus driver\n");

        return -1;
    }

    while(1){
        usleep(1000);
    }

    return 0;
}
