#include <main.h>
#include <CAN.h>
#include <IO.h>
#include <Clock.h>
#include <Timers.h>
#include <Interface.h>

#define DEVICE_ADDRESS 0x01

int main(void){
  HAL_Init();
  Clock_Init();
  GPIO_Init();
  CAN_Init(DEVICE_ADDRESS);
  CAN_Start();
  TIMER2_Init();
  TIMER3_Init();

  while (1){
	  if(getHeartbeatFlag()){
		  Interface_SendHeartbeat();
		  unsetHeartbeatFlag();
	  }
	  if(getInputFlag()){
		  Interface_SendInput();
		  unsetInputFlag();
	  }
  }
}
