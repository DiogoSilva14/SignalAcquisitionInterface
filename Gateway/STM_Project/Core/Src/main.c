#include <main.h>
#include <MODBUS.h>
#include <RS485.h>
#include <IO.h>
#include <CAN.h>
#include <Clock.h>
#include <Interfaces.h>

int main(void){
  HAL_Init();
  Clock_Init();
  Interfaces_Init();
  CAN_Init();
  GPIO_Init();
  DWT_Init();

  CAN_Start();
  MODBUS_Init(115200, 0x00);

  while (1){
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  DWT_Delay(100000);
  }
}
