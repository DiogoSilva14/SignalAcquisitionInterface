#include <CAN.h>

CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef TxHeader;

uint32_t TxMailbox;
uint8_t TxData[8];

uint8_t deviceAddress;

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
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);


	TxHeader.ExtId = 0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = deviceAddress;
	TxHeader.TransmitGlobalTime = DISABLE;
}

uint8_t CAN_SendMsg(uint8_t typeIdentifier, uint8_t* data, uint8_t length){
	TxHeader.DLC = length;
	TxHeader.StdId = ((typeIdentifier & 0x03) << 8) | deviceAddress;

	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, data, &TxMailbox) != HAL_OK){
		return 1;
	}

	return 0;
}
