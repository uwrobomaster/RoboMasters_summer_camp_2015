#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "system.h"

extern float Motor_PWM;
extern float Speed_Target;
extern int16_t Speed[4];
extern float Speed_Measure;
extern bool Motor1Stop;

void Motor1_Forward(int16_t pwm);	//��ת
void Motor1_Back(int16_t pwm);	//��ת
void Motor1_Stop(void);	//ͣ�
void Motor2_Forward(int16_t pwm);	//��ת
void Motor2_Back(int16_t pwm);	//��ת
void Motor2_Stop(void);	//ͣת�
void Motor_Control(void);
float Speed_Control(float speed_current,float speed_target);

#endif 
