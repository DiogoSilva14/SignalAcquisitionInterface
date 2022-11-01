#include "device_functions.h"

sem_t registers_semaphore;

void delay_us(int microseconds){
    usleep(microseconds);
}

int init_register_semaphore(){
    return sem_init(&registers_semaphore, 0, 1);
}

void register_semaphore_down(){
    sem_wait(&registers_semaphore);
}

void register_semaphore_up(){
    sem_post(&registers_semaphore);
}