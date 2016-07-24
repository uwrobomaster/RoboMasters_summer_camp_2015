#include "motor.h"

bool Motor_L = 0,Motor_R = 0,MotorStop = 0;
bool Motor1_Dir = 0,Motor1Stop = 0;
bool Motor2_Dir = 0,Motor2Stop = 0;
float Motor_PWM = 0;
float Speed_Target = 0;
int16_t Speed[4] = {0};
float Speed_Measure = 0;
float Speed_Measure1 = 0;
float Speed_Measure2 = 0;
float LPF_factor = 0.80;	//低通滤波系数

void Motor1_Forward(int16_t pwm)
{
	if(Motor1_Dir == 0)
	{
		Motor1_Stop();
		delayMicroseconds(10);
	}
	pwm = constrain(pwm,0,4500);
	TIM1->CCR1 = pwm;
	TIM1->CCR2 = 0;
	Motor1_Dir = 1;
}
	
void Motor1_Back(int16_t pwm)
{
	if(Motor1_Dir == 1)
	{
		Motor1_Stop();
		delayMicroseconds(10);
	}
	pwm = constrain(pwm,0,4500);
	TIM1->CCR1 = 0;
	TIM1->CCR2 = pwm;
	Motor1_Dir = 0;
}

//电机停止转动
void Motor1_Stop(void)
{
	TIM1->CCR1 = 0;	
	TIM1->CCR2 = 0;	
}
	
void Motor1_Con(float PIDTerm)
{
	if(PIDTerm >= 0)
	{
		Motor1_Forward((int16_t)PIDTerm);
	}
	else if(PIDTerm < 0)
	{
		PIDTerm = -PIDTerm;
		Motor1_Back((int16_t)PIDTerm);
	}
}


void Motor2_Forward(int16_t pwm)
{
	if(Motor2_Dir == 0)
	{
		Motor2_Stop();
		delayMicroseconds(10);
	}
	pwm = constrain(pwm,0,4500);
	TIM1->CCR3 = pwm;
	TIM1->CCR4 = 0;
	Motor2_Dir = 1;
}
	
void Motor2_Back(int16_t pwm)
{
	if(Motor2_Dir == 1)
	{
		Motor2_Stop();
		delayMicroseconds(10);
	}
	pwm = constrain(pwm,0,4500);
		TIM1->CCR3 = 0;
	TIM1->CCR4 = pwm;
	Motor2_Dir = 0;
}

//电机停止转动
void Motor2_Stop(void)
{
	TIM1->CCR3 = 0;	
	TIM1->CCR4 = 0;	
}
	
void Motor2_Con(float PIDTerm)
{
	if(PIDTerm >= 0)
	{
		Motor2_Forward((int16_t)PIDTerm);
	}
	else if(PIDTerm < 0)
	{
		PIDTerm = -PIDTerm;
		Motor2_Back((int16_t)PIDTerm);
	}
}


//速度环控制
float Speed_Control(float speed_current,float speed_target)
{
	static float lasterror,errorI;
	float	error,delta;
	float PTerm,ITerm,DTerm,PIDTerm;
	
	if(speed_target < -260) speed_target = -260;
	else if(speed_target > 260) speed_target = 260;	
	
	error = speed_current - speed_target;
		
	//P
	PTerm = SpeedPID.P * error;
	//I
	errorI += error * 0.05;	//误差积分
	if(errorI > 200) errorI = 200;	//积分限幅
	if(errorI < -200) errorI = -200;
	ITerm = errorI * SpeedPID.I;
	//D
	delta = error - lasterror;
	lasterror = error;
	DTerm = delta * SpeedPID.D;
	//PID
	PIDTerm = PTerm + ITerm + DTerm;
	return PIDTerm;
}

void Motor_Control(void)
{	
	Speed_Measure1 = Encoder_Get_CNT();
  Speed_Measure2 = Encoder_Get_CNT();
//	Speed_Target = Speed[0] / 3;
//	Motor1_Con(Speed_Control(Speed_Measure1,Speed_Target));		
//	Speed_Target = Speed[1] / 3;
//	Motor2_Con(Speed_Control(Speed_Measure2,Speed_Target));		
//	
//	#ifdef MOTOR_NUM1	
//		//Speed_Target = Speed_Target * LPF_factor + (float)Speed[0] / 3 * (1 - LPF_factor);
//	Speed_Target = Speed[0] / 3;
//	Motor1_Con(Speed_Control(Speed_Measure1,Speed_Target));		
//	#endif
//	
//	#ifdef MOTOR_NUM2	
//		//Speed_Target = Speed_Target * LPF_factor + (float)Speed[1] / 3 * (1 - LPF_factor);
//	Speed_Target = Speed[1] / 3;
//	Motor2_Con(Speed_Control(Speed_Measure2,Speed_Target));		
//	#endif
//	#ifdef MOTOR_NUM3	
//		//Speed_Target = Speed_Target * LPF_factor + (float)Speed[2] / 3 * (1 - LPF_factor);
//		Speed_Target = Speed[2] / 3;
//		Speed_Control(Speed_Measure,-Speed_Target);		
//	#endif
//	#ifdef MOTOR_NUM4	
//		//Speed_Target = Speed_Target * LPF_factor + (float)Speed[3] / 3 * (1 - LPF_factor);
//		Speed_Target = Speed[3] / 3;
//		Speed_Control(Speed_Measure,Speed_Target);		
//	#endif	
}
