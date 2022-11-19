#include "device_functions.h"

//sem_t registers_semaphore;

uint8_t first_delay = 0;

void delay_us(int microseconds){
	if(first_delay){
		DWT_Init();
		first_delay = 0;
	}

	DWT_Delay(microseconds);
}

int init_register_semaphore(){
	return 0;
    //return sem_init(&registers_semaphore, 0, 1);
}

void register_semaphore_down(){
    #ifdef DEBUG
        printf("Taking semaphore\n");
    #endif
    //sem_wait(&registers_semaphore);
}

void register_semaphore_up(){
    #ifdef DEBUG
        printf("Releasing semaphore\n");
    #endif
    //sem_post(&registers_semaphore);
}
