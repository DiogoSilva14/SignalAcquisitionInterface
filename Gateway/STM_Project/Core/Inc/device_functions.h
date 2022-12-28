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
#include "stm32f1xx_hal.h"
#include "DWT_Delay.h"

/** @brief Delays the code execution
 *
 *  @param microseconds Amount of microseconds to delay
 */

void delay_us(int microseconds);

uint32_t LL_SYSTICK_IsActiveCounterFlag(void);

uint32_t getCurrentMicros(void);

#endif
