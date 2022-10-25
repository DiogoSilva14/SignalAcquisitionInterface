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
#include <time.h>
#include "rs485.h"
#include "device_functions.h"

// Error definition
#define FAILED_TO_INITIALIZE -1
#define NOT_INITIALIZED -1
#define ERROR_SENDING_FRAME -2

// Parameters
#define CIRCULAR_BUFFER_SIZE 200

// Struct to store a byte with its receive timestamp
typedef struct Buffer_Byte{
    long timestamp; // Timestamp in nanoseconds
    uint8_t byte; // Byte 
} Buffer_Byte;

// Struct for the circular buffer
typedef struct Circular_Buffer{
    Buffer_Byte buffer[CIRCULAR_BUFFER_SIZE];
    int head;
    int tail;
} Circular_Buffer;

/** @brief Initialize modbus module
 *
 *  @param baud_rate Baud rate desired for RS-485 module
 *  @param address Address intended for this device on Modbus line
 *  @return 0 Modbus module was initialized properly
 *          -1 Failed to initialize Modbus module
 */
int init_modbus(int baud_rate, uint8_t address);


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
int send_frame(uint8_t destination_address, uint8_t function, uint8_t* data_pointer, uint8_t data_length);

/** @brief Calculates the 16-bit CRC
 *
 *  @param data_pointer Pointer to the data buffer to calculate the crc
 *  @param data_length Length of data buffer
 *  @return Calculated CRC
 */
uint16_t crc16(const uint16_t* data_pointer, int length);

#endif