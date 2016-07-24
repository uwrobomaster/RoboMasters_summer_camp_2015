#ifndef __SYSTEM_H
#define	__SYSTEM_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

#include "encoder.h"
#include "led.h"
#include "pwm.h"
#include "usart2.h"
#include "can.h"
#include "ad.h"
#include "key.h"
#include "motor.h"
#include "data_transfer.h"

#define abs(x) ((x)>0? (x):(-(x)))

#define MOTOR_NUM1   		//前左
//#define MOTOR_NUM2   		//前右
//#define MOTOR_NUM3   		//后右
//#define MOTOR_NUM4  	  //后左

typedef struct 
{   
	float P;
	float I;
	float D;
} PID_t;

extern volatile uint32_t sysTickUptime;
extern PID_t SpeedPID;

int constrain(int amt, int low, int high);
uint32_t micros(void);
void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);
void Parameter_Init(void);
void systemInit(void);
void Loop(void);

#endif 
