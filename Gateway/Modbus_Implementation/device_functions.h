/** @file device_functions.h
 *  @brief Device drivers for generic functions
 *
 *  This contains the device drivers for generic functions
 *  like delay, etc..
 *
 *  @author Diogo Silva
 *  @bug No known bugs.
 */

#ifndef _DEV_FUNC_H
#define _DEV_FUNC_H

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

/** @brief Delays the code execution
 *
 *  @param microseconds Amount of microseconds to delay
 */

void delay_us(int microseconds);

/** @brief Initializes the Circular Buffer Semaphore
 *
 *  @return 0 if successful
 *          Error code if not successful
 */
int init_buffer_semaphore();

/** @brief Take Circular Buffer semaphore
 */
void buffer_semaphore_down();

/** @brief Releases Circular buffer semaphore
 */
void buffer_semaphore_up();

#endif