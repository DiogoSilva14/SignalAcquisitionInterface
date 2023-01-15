#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include <main.h>
#include <Interface.h>

int TIMER1_Init(void);
int TIMER2_Init(void);
int TIMER3_Init(void);
void PWM1_changeDutyCycle(uint8_t dutyCycle);

#endif /* INC_TIMERS_H_ */
