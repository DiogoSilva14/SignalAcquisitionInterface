#include <IO.h>

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

uint8_t digital_in_pins[DIGITAL_IN_PINS];
uint8_t digital_out_pins[DIGITAL_OUT_PINS];
uint32_t analog_out_pins[ANALOG_OUT_PINS];
uint32_t analog_in_pins[ANALOG_IN_PINS];

uint8_t GPIO_Init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

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

    /*Configure GPIO pin : PB11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK){
    	return 1;
    }

    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
    	return 1;
    }

    HAL_ADCEx_Calibration_Start(&hadc1);

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

void LED_OFF(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void LED_ON(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

uint16_t GPIO_GetPin(uint8_t type, uint8_t inout, uint8_t number){
	if(type == ANALOG){
		if(inout == INPUT){
			return analog_in_pins[number] & 0x0FFF;
		}else if(inout == OUTPUT){
			return analog_out_pins[number] & 0x0FFF;
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

void GPIO_SetPin(uint8_t type, uint8_t inout, uint8_t number, uint16_t value){
	if(type == ANALOG){
		if(inout == OUTPUT){
			analog_out_pins[number] = value;
		}
	}else if(type == DIGITAL){
		if(inout == OUTPUT){
			if(value){
				digital_out_pins[number] = 0xFF;
			}else{
				digital_out_pins[number] = 0x00;
			}
		}
	}
}

void ADC_StartConversion(){
	HAL_ADC_Start_DMA(&hadc1, &analog_in_pins[0], 1);
}

void GPIO_ApplyOutput(){
	// Inverted logic
	if(digital_out_pins[0]){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
	}

	PWM1_changeDutyCycle(analog_out_pins[0]*100/4095);
}
