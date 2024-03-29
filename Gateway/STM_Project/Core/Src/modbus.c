#include <MODBUS.h>

uint8_t device_address;
uint8_t initialized = 0;

int MODBUS_Init(int _baud_rate, uint8_t address){
    if(init_rs485(_baud_rate, 8, 0, 1, 0)){
        return FAILED_TO_INITIALIZE;
    }

    device_address = address;

    initialized = 1;

    MODBUS_RxThread();

    return 0;
}

int MODBUS_SendFrame(uint8_t destination_address, uint8_t function, uint8_t* data_pointer, uint8_t data_length){
    if(!initialized){
        return NOT_INITIALIZED;
    }

    uint8_t byte_buffer[data_length + 3];
    uint8_t buffer_it = 0;

    byte_buffer[buffer_it++] = destination_address;
    byte_buffer[buffer_it++] = function;

    for(int i=0; i < data_length; i++){
        byte_buffer[buffer_it++] = data_pointer[i];
    }

    uint16_t crc = CRC16(byte_buffer, buffer_it);

    byte_buffer[buffer_it++] = crc & 0xFF;
    byte_buffer[buffer_it++] = (crc >> 8) & 0xFF;

	return sendBuffer(byte_buffer, buffer_it);
}

uint16_t CRC16(uint8_t* data_pointer, int length){
    // Using a table approach because it is much faster, and that is important for the
    // microcontroller implementation
    static const uint16_t table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };

	uint8_t xor = 0;
	uint16_t crc = 0xFFFF;

	while(length--){
		xor = (*data_pointer++) ^ crc;
		crc >>= 8;
		crc ^= table[xor];
	}

    return crc;
}

void MODBUS_RxThread(){
    uint8_t rx_byte = 0;
    Frame frame;
    long time_last = 0;

    frame.length = 0;

    time_last = getCurrentMicros();

    while(1){
        while((getCurrentMicros() - time_last) < 1000){
        	if(getByte(&rx_byte) > 0){
        		frame.data[frame.length++] = rx_byte;
        		time_last = getCurrentMicros();
        	}
        	if(CAN_getRxFlag()){
        		Interfaces_processCANMessages();
				CAN_unsetRxFlag();
			}
        }

        if(frame.length != 0){
            MODBUS_HandleFrame(frame);

            memset(frame.data, 0, sizeof(frame.data));
            frame.length = 0;
        }

        time_last = getCurrentMicros();
    }
}

void MODBUS_HandleFrame(Frame frame){
    static uint8_t buffer[MAX_MODBUS_DATA];
    uint16_t buffer_len = 0;
    uint8_t deviceAddress = frame.data[0];

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

    switch(frame.data[1]){
        case WRITE_MULTIPLE_HOLDING_REGISTERS: ;
            uint16_t analog_first_register = (frame.data[2] << 8 | frame.data[3]);
            uint16_t analog_number_registers = (frame.data[4] << 8 | frame.data[5]);

            uint16_t registerNumber = analog_first_register;

            for(int i=0; i < analog_number_registers; i++){
            	MODBUS_SetDeviceRegister(deviceAddress, registerNumber++, (frame.data[7 + i*2] << 8) | (frame.data[8 + i*2]));
            }

            buffer_len = 4;

            buffer[0] = frame.data[2];
            buffer[1] = frame.data[3];
            buffer[3] = frame.data[4];
            buffer[4] = frame.data[5];

            Interfaces_updateOutput(deviceAddress);

            MODBUS_SendFrame(deviceAddress, WRITE_MULTIPLE_HOLDING_REGISTERS, buffer, buffer_len);
            break;
        case WRITE_HOLDING_REGISTER: ;
        	uint16_t registerToWrite = (frame.data[2] << 8 | frame.data[3]);
        	uint16_t valueToWrite = (frame.data[4] << 8 | frame.data[5]);

        	MODBUS_SetDeviceRegister(deviceAddress, registerToWrite, valueToWrite);

        	buffer_len = 4;

			buffer[0] = frame.data[2];
			buffer[1] = frame.data[3];
			buffer[2] = frame.data[4];
			buffer[3] = frame.data[5];

			Interfaces_updateOutput(deviceAddress);

			MODBUS_SendFrame(deviceAddress, WRITE_HOLDING_REGISTER, buffer, buffer_len);
        	break;
        case READ_HOLDING_REGISTERS: ;
        	uint16_t firstRegisterToRead = (frame.data[2] << 8 | frame.data[3]);
        	uint16_t numberOfRegistersToRead = (frame.data[4] << 8 | frame.data[5]);

        	buffer_len = numberOfRegistersToRead*2 + 1;
        	buffer[0] = numberOfRegistersToRead*2;

        	uint16_t readRegisterNumber = firstRegisterToRead;

        	for(int i=0; i < numberOfRegistersToRead; i++){
        		uint16_t registerValue = MODBUS_GetDeviceRegister(deviceAddress, readRegisterNumber++);
        		buffer[i*2+1] = (registerValue >> 8) & 0xFF;
        		buffer[i*2+2] = registerValue & 0xFF;
        	}

        	MODBUS_SendFrame(deviceAddress, READ_HOLDING_REGISTERS, buffer, buffer_len);
        default:
            break;
    }
}

uint16_t MODBUS_GetDeviceRegister(uint8_t deviceAddress, uint16_t registerAddress){
	switch(registerAddress){
		case 40001: ;
			uint16_t value = 0;

			for(int i=0; i < DIGITAL_INPUTS; i++){
				value |= getDigitalInput(deviceAddress, i) & 0x01;
				value = value << 1;
			}

			value = value >> 1;

			return value;
			break;
		case 40002:
			return getAnalogInput(deviceAddress,0);
			break;
		case 40003:
			return getAnalogInput(deviceAddress,1);
			break;
		case 40004:
			return getAnalogInput(deviceAddress,2);
			break;
		case 40005:
			return getAnalogInput(deviceAddress,3);
			break;
	}

	return 0;
}

void MODBUS_SetDeviceRegister(uint8_t deviceAddress, uint16_t registerAddress, uint16_t value){
	switch(registerAddress){
		case 40006:
			for(int i=0; i < DIGITAL_OUTPUTS; i++){
				setDigitalOutput(deviceAddress, i, value & 0x01);
				value = value >> 1;
			}
			break;
		case 40007:
			setAnalogOutput(deviceAddress,0, value);
			break;
		case 40008:
			setAnalogOutput(deviceAddress,1, value);
			break;
	}
}
