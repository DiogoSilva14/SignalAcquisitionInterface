#include <main.h>
#include <MODBUS.h>
#include <RS485.h>
#include <IO.h>
#include <CAN.h>
#include <Clock.h>

int main(void){
  HAL_Init();
  Clock_Init();
  CAN_Init();
  GPIO_Init();
  DWT_Init();

  CAN_Start();
  MODBUS_Init("dummy", 115200, 0x03);

  while (1){
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  DWT_Delay(100000);
	  //HAL_Delay(100);
  }
}
