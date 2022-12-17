#include "main.h"
#include <CAN.h>
#include <IO.h>
#include <Clock.h>

uint8_t data;

int main(void){
  HAL_Init();
  Clock_Init();
  GPIO_Init();
  CAN_Init();

  CAN_Start();

  while (1){
	  GPIO_SamplePins();

	  if(GPIO_GetPin(DIGITAL, INPUT, 0)){
		  LED_OFF();
		  data = 0x00;

		  CAN_SendMsg(&data, 1);
	  }else{
		  LED_ON();
		  data = 0xFF;

		  CAN_SendMsg(&data, 1);
	  }

	  HAL_Delay(10);
  }
}
