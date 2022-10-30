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
#include <stdlib.h>
#include <string.h>     
#include <unistd.h>     
#include <fcntl.h>      
#include <errno.h>      
#include <termios.h>    
#include <stdint.h>


// Error definition
#define INITIALIZATION_NOT_SUCCESSFUL -1
#define RS485_NOT_INITIALIZED -1
#define FAILED_SENDING_BYTE -2
#define FAILED_GETTING_BYTE -1

/** @brief Initializes the RS-485 Module
 *
 *  @param serial_port_device Serial port for RS-485 device
 *  @param baud_rate Baud rate chosen for the RS-485 line
 *  @param bits_per_frame Quantity of bits sent per frame
 *  @param parity_bit Quantity of parity bits, either 1 or 0
 *  @param stop_bits Quantity of stop bits, either 1 or 2
 *  @param block Choose whether read() blocks. 0 means no blocking, other value means block.
 *  @return 0 Initalization was sucessful
 *          -1 Device was not initialized sucessfuly
 */
int init_rs485(char* serial_port_device, int _baud_rate, uint8_t bits_per_frame, uint8_t parity_bit, uint8_t stop_bits, uint8_t block);

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
 *  @return  1 Valid byte gotten
 *           0 No byte available
 *          -1 Error while getting a byte
 */
int getByte(uint8_t* received_byte);

/** @brief Returns the current baud rate
 * 
 *  @return Baud Rate
 */
int getBaudRate();

#endif