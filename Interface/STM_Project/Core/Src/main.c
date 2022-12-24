#include <main.h>
#include <CAN.h>
#include <IO.h>
#include <Clock.h>
#include <Timers.h>
#include <Interface.h>
#include <DMA.h>

#define DEVICE_ADDRESS 0x01

int main(void){
  HAL_Init();
  DMA_Init();
  Clock_Init();
  GPIO_Init();
  CAN_Init(DEVICE_ADDRESS);
  CAN_Start();
  TIMER2_Init();
  TIMER3_Init();

  uint32_t timeLastADCConversion = HAL_GetTick();

  while (1){
	  if(getHeartbeatFlag()){
		  Interface_SendHeartbeat();
		  unsetHeartbeatFlag();
	  }
	  if(getInputFlag()){
		  Interface_SendInput();
		  unsetInputFlag();
	  }

	  if(HAL_GetTick() - timeLastADCConversion > 5){
		  ADC_StartConversion();
		  timeLastADCConversion = HAL_GetTick();
	  }
	  if(CAN_getRxFlag()){
		  // Process input
	  }
  }
}
