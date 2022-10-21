/** @file rs485.h
 *  @brief RS485 functions
 *
 *  This library should contain functions
 *  related to the RS-485 communication line
 *
 *  @author Diogo Silva
 *  @bug No known bugs.
 */

#ifndef _RS485_H
#define _RS485_H

#include <stdio.h>
#include <stdint.h>

// Error definition
#define INITIALIZATION_NOT_SUCCESFULL -1
#define RS485_NOT_INITIALIZED -1
#define FAILED_SENDING_BYTE -2
#define FAILED_GETTING_BYTE -1

/** @brief Initializes the RS-485 Module
 *
 *  @param baud_rate Baud rate chosen for the RS-485 line
 *  @return 0 Initalization was sucessful
 *          -1 Device was not initialized sucessfuly
 */
int init_rs485(int baud_rate);

/** @brief Send a byte over RS-485
 *
 *  @param byte_to_send Byte to send
 *  @return 0 Byte was sent
 *          -1 RS-485 module is not initialized
 *          -2 Failed sending byte
 */
int sendByte(uint8_t byte_to_send);

/** @brief Get a byte from RS-485 line
 *
 *  @param block If != 0 and there isn't a byte on the buffer, wait for one to arrive
 *  @param received_byte Pointer to the memory location in which to place the byte
 *  @return 0 Valid byte gotten
 *          -1 Failed to get a byte
 */
int getByte(uint8_t block, uint8_t* received_byte);

#endif