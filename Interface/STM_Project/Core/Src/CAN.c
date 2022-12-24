#include <CAN.h>

CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint32_t TxMailbox;
uint8_t TxData[8];
uint8_t RxData[8];

uint8_t deviceAddress;

uint8_t rxFlag = 0x00;

static volatile CircularBuffer circularBuffer = {0,0,0,0};

uint8_t CAN_Init(uint8_t address){
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 18;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = DISABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;

	deviceAddress = address;

	if (HAL_CAN_Init(&hcan) != HAL_OK){
	  return 1;
	}

	return 0;
}

uint8_t CAN_Start(){
	CAN_FilterTypeDef canfilterconfig;

	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 10;  // anything between 0 to SlaveStartFilterBank
	canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig.FilterIdHigh = deviceAddress;
	canfilterconfig.FilterIdLow = 0x0000;
	canfilterconfig.FilterMaskIdHigh = 0xFF;
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canfilterconfig.SlaveStartFilterBank = 13;  // 13 to 27 are assigned to slave CAN (CAN 2) OR 0 to 12 are assgned to CAN1

	TxHeader.ExtId = 0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = deviceAddress;
	TxHeader.TransmitGlobalTime = DISABLE;

	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	return 0;
}

uint8_t CAN_SendMsg(uint8_t typeIdentifier, uint8_t* data, uint8_t length){
	TxHeader.DLC = length;
	TxHeader.StdId = ((typeIdentifier & 0x03) << 8) | deviceAddress;

	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, data, &TxMailbox) != HAL_OK){
		return 1;
	}

	return 0;
}

uint8_t CAN_getRxFlag(){
	return rxFlag;
}

void CAN_setRxFlag(){
	rxFlag = 0xFF;
}

void CAN_unsetRxFlag(){
	rxFlag = 0x00;
}

uint8_t CAN_popMessage(CAN_Message* message){
	if(circularBuffer.size == 0)
		return 1;

	*message = circularBuffer.data[circularBuffer.tail++];

	if(circularBuffer.tail == BUFFER_MAX_SIZE){
		circularBuffer.tail = 0;
	}

	circularBuffer.size--;

	return 0;
}

static uint8_t CAN_putMessage(uint16_t header, uint8_t* data, uint8_t length){
	CAN_Message message;

	message.header = header;
	message.length = length;

	for(int i=0; i < length; i++){
		message.data[i] = data[i];
	}

	circularBuffer.data[circularBuffer.head++] = message;
	circularBuffer.size++;

	if(circularBuffer.head == BUFFER_MAX_SIZE){
		circularBuffer.head = 0;
	}

	if(circularBuffer.size > BUFFER_MAX_SIZE){
		circularBuffer.size = BUFFER_MAX_SIZE;
		circularBuffer.tail++;

		if(circularBuffer.tail == BUFFER_MAX_SIZE){
			circularBuffer.tail = 0;
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

    if((RxHeader.StdId & 0xFF) == deviceAddress){
    	setRxFlag();
    }
}
