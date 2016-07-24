#include "pwm.h"

//PA8----EN
//PA9----PWM---TIM1_CH2
//PA10---EN
//PA11---PWM---TIM1_CH4

void PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
  GPIO_InitTypeDef         gpio;
    
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);	
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10|GPIO_Pin_9 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
  
	tim.TIM_Period=5000;
	tim.TIM_Prescaler=0;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &oc);
	TIM_OC2Init(TIM1, &oc);
	TIM_OC3Init(TIM1, &oc);
	TIM_OC4Init(TIM1, &oc);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	
	
	
	///////���
	//TIM_TimeBaseInitTypeDef	 tim;			//����һ����ʱ���ṹ�����
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);	
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
	
	TIM_DeInit(TIM4);                              			//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM4); 							//���� TIM4 �ڲ�ʱ��  
	tim.TIM_Period = 20000; 				//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  1000000/1000=1000us=1ms																												//TIM_Period��TIM1_ARR��=7200�����������ϼ�����7200����������¼���															//����ֵ���� Ҳ���� 1MS���������¼�һ��
	tim.TIM_Prescaler = 72-1;  				//�Զ���Ԥ��Ƶϵ��Ϊ0������ʱ����ʱ��Ƶ��Ϊ72M�ṩ����ʱ����ʱ��	0~65535֮��														//����Ԥ��Ƶ����Ƶϵ��0
	tim.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	tim.TIM_CounterMode = TIM_CounterMode_Up;   														//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &tim); 		//����ָ��������ʼ��TIMʱ������Ĵ���	
 
 
   oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 1500;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM4, &oc);
	//TIM_OC2Init(TIM4, &oc);
	

  TIM_ARRPreloadConfig(TIM4, ENABLE);						//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_Cmd(TIM4, ENABLE); 		//TIM4�ܿ��أ����� 
	
}

void PWM_Out1(int16_t pwm1, int16_t pwm2)
{
	TIM1->CCR1 = pwm1;
	TIM1->CCR2 = pwm2;
}

void PWM_Out2(int16_t pwm3, int16_t pwm4)
{
	TIM1->CCR3 = pwm3;
	TIM1->CCR4 = pwm4;
}
	

void setduoji1(int16_t pwm1)
{
	TIM4->CCR1 = pwm1;
}
