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

uint32_t getCurrentMicros(void){
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (LL_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

static inline uint32_t LL_SYSTICK_IsActiveCounterFlag(void){
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}
