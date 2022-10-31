#include "device_functions.h"

sem_t circular_buffer_semaphore;

void delay_us(int microseconds){
    usleep(microseconds);
}

int init_buffer_semaphore(){
    return sem_init(&circular_buffer_semaphore, 0, 1);
}

void buffer_semaphore_down(){
    sem_wait(&circular_buffer_semaphore);
}

void buffer_semaphore_up(){
    sem_post(&circular_buffer_semaphore);
}