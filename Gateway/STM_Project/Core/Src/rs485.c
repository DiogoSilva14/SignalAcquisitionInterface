#include <RS485.h>

// RS-485 line baud rate
int baud_rate = 0;
static volatile uint8_t uart_RxData[1];
static volatile uint8_t rx_flag = 0;
UART_HandleTypeDef huart1;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  rx_flag = 1;
}

int init_rs485(char* serial_port_device, int _baud_rate, uint8_t bits_per_frame, uint8_t parity_bit, uint8_t stop_bits, uint8_t block){
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK){
	  return 1;
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

	HAL_UART_Receive_IT(&huart1, uart_RxData, 1);

	baud_rate = _baud_rate;

    return 0;
}

int sendByte(uint8_t byte_to_send){
    if(baud_rate == 0){
        return RS485_NOT_INITIALIZED;
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // Pull DE high to enable TX operation
    HAL_UART_Transmit(&huart1, &byte_to_send, 1, 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  // Pull RE Low to enable RX operation

    return 0;
}

int sendBuffer(uint8_t* buffer, uint16_t length){
    if(baud_rate == 0){
        return RS485_NOT_INITIALIZED;
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // Pull DE high to enable TX operation
    HAL_UART_Transmit(&huart1, buffer, length, 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  // Pull RE Low to enable RX operation

    return 0;
}

int getByte(uint8_t* received_byte){
	if(rx_flag){
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        *received_byte = uart_RxData[0];
        rx_flag = 0;

        //HAL_Delay(100);

        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // Pull DE high to enable TX operation
        //HAL_UART_Transmit(&huart1, uart_RxData, 1, 1000);
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  // Pull RE Low to enable RX operation

        HAL_UART_Receive_IT(&huart1, uart_RxData, 1);
        return 1;
	}else{
		return FAILED_GETTING_BYTE;
	}
}

int getBaudRate(){
    return baud_rate;
}
