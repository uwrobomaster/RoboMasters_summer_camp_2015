#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f10x.h"

void PWM_Configuration(void);
void PWM_Out1(int16_t pwm1, int16_t pwm2);
void PWM_Out2(int16_t pwm3, int16_t pwm4);
void setduoji(int16_t pwm1, int16_t pwm2);

#endif
