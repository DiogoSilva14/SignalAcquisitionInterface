/** @file modbus.h
 *  @brief Modbus implementation
 *
 *  This library is a standard implementation of
 *  Modbus RTU
 *
 *  @author Diogo Silva
 *  @bug No known bugs.
 */

#ifndef _MODBUS_H
#define _MODBUS_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <RS485.h>
#include <CAN.h>
#include <Interfaces.h>
#include "device_functions.h"

// Error definition
#define FAILED_TO_INITIALIZE -1
#define NOT_INITIALIZED -1
#define ERROR_SENDING_FRAME -2
#define BUFFER_FULL -1

// Modbus Function definition
#define READ_COILS 0x01
#define WRITE_MULTIPLE_HOLDING_REGISTERS 0x10
#define WRITE_HOLDING_REGISTER 0x06
#define READ_HOLDING_REGISTERS 0x03

// Parameters
#define CIRCULAR_BUFFER_SIZE 200
#define MAX_MODBUS_DATA 100
#define SILENCE_DURATION_BITS 5

// Struct to store a byte with its receive timestamp
typedef struct Frame{
    uint8_t data[MAX_MODBUS_DATA+4];
    uint16_t length;
} Frame;

typedef struct Modbus_Frame{
    uint8_t address;
    uint8_t function;
    uint8_t data[MAX_MODBUS_DATA];
    uint16_t crc;
} Modbus_Frame;

/** @brief Initialize modbus module
 *
 *  @param baud_rate Baud rate desired for RS-485 module
 *  @param address Address intended for this device on Modbus line
 *  @return 0 Modbus module was initialized properly
 *          -1 Failed to initialize Modbus module
 *          -2 Failed to initialize Circular Buffer Semaphore
 */
int MODBUS_Init(int _baud_rate, uint8_t address);


/** @brief Send a frame over Modbus
 *
 *  @param destination_address Destination device's address
 *  @param function Function intended for the frame
 *  @param data_pointer Pointer to the data buffer
 *  @param data_length Data Buffer length
 *  @return 0 Frame sent
 *          -1 Modbus module not initialized
 *          -2 Modbus frame not sent
 */
int MODBUS_SendFrame(uint8_t destination_address, uint8_t function, uint8_t* data_pointer, uint8_t data_length);

/** @brief Calculates the 16-bit CRC
 *
 *  @param data_pointer Pointer to the data buffer to calculate the crc
 *  @param data_length Length of data buffer
 *  @return Calculated CRC
 */
uint16_t CRC16(uint8_t* data_pointer, int length);

/** @brief Function to receive frames and answer the requests
 */
void MODBUS_RxThread();

/** @brief Handles a received Modbus frame
 *
 *  @param frame Frame to process
 */
void MODBUS_HandleFrame(Frame frame);

uint16_t MODBUS_GetDeviceRegister(uint8_t deviceAddress, uint16_t registerAddress);

void MODBUS_SetDeviceRegister(uint8_t deviceAddress, uint16_t registerAddress, uint16_t value);

#endif
