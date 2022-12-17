#include <IO.h>

uint8_t digital_in_pins[DIGITAL_IN_PINS];
uint8_t digital_out_pins[DIGITAL_OUT_PINS];
uint16_t analog_out_pins[ANALOG_IN_PINS];
uint16_t analog_in_pins[ANALOG_OUT_PINS];

uint8_t GPIO_Init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PB14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	for(int i=0; i<DIGITAL_IN_PINS; i++){
		digital_in_pins[i] = 0;
	}

	for(int i=0; i<DIGITAL_OUT_PINS; i++){
		digital_out_pins[i] = 0;
	}

	for(int i=0; i<ANALOG_IN_PINS; i++){
		analog_in_pins[i] = 0;
	}

	for(int i=0; i<ANALOG_OUT_PINS; i++){
		analog_out_pins[i] = 0;
	}

	return 0;
}

void GPIO_SamplePins(){
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)){
		digital_in_pins[0] = 0x00;
	}else{
		digital_in_pins[0] = 0xFF;
	}
}

void LED_ON(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void LED_OFF(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

uint16_t GPIO_GetPin(uint8_t type, uint8_t inout, uint8_t number){
	if(type == ANALOG){
		if(inout == INPUT){
			return analog_in_pins[number];
		}else if(inout == OUTPUT){
			return analog_out_pins[number];
		}
	}else if(type == DIGITAL){
		if(inout == INPUT){
			if(digital_in_pins[number]){
				return 0xFFFF;
			}else{
				return 0x0000;
			}
		}else if(inout == OUTPUT){
			if(digital_out_pins[number]){
				return 0xFFFF;
			}else{
				return 0x0000;
			}
		}
	}

	return 0;
}
