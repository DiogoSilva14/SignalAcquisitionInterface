#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "rs485.h"

int main(int argc, char *argv[]){
    
    if(argc < 3){
        printf("Usage: ./rs485-test serial-port baud-rate\n");

        return -1;
    }

    if(init_rs485(argv[1], atoi(argv[2]), 8, 0, 1, 0)){
        printf("Error initializing RS-485 driver\n");

        return -1;
    }

    uint8_t byte = 0;

    while(1){
        if(getByte(&byte)){
            printf("Received: %2X \n", byte);
        }

        usleep(10);
    }

    return 0;
}