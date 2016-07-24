/*********************************************************************************
 * �ļ���  ��pwm_output.c
 * ����    ��ʱ��TIM1���4·PWM��������·����һ�����ٵ��ʵ������ת��һ·����һ�����ٵ��ʵ����ת��һ·����һ�����ʵ�ֶ��Ƕ�ת��         
 * ��Ŀ��Robomasters2015_Group11_Designed by LiChao
 * Ӳ�����ӣ�---------------------
 *          |  PA.08: (TIM1_CH1)  |
 *          |  PA.09: (TIM1_CH2)  |
 *      	  |  PA.10: (TIM1_CH3)  | 
 *    		  |  PA.11: (TIM1_CH4)  |
 *           ---------------------    			
 * 
 * ��汾  ��ST3.5.0
**********************************************************************************/
#include "pwm_output.h"
extern TIM_OCInitTypeDef  TIM_OCInitStructure;		      //PWM ??????????
/*
 * ��������TIM1_GPIO_Config
 * ����  ������TIM1�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM1_GPIO_Config(void) 
{
 
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM1 clock enable */                                                                                          // ��ʱ��1ʱ��ʹ��
	//PCLK1����2��Ƶ����ΪTIM1��ʱ��Դ����36MHz
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 

  /* GPIOA and GPIOB clock enable */                                                                               //IOʱ��ʹ��,����2��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA /*| RCC_APB2Periph_GPIOB*/, ENABLE); 

  /*GPIOA Configuration: TIM1 channel 1 2 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;                             // �����ĸ�ͨ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                                                              // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //TIM1->BDTR |= 1<<15;
  /*GPIOB Configuration */                                                                           
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;             //���� �г̿��� ������

 // GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * ��������TIM1_Mode_Config
 * ����  ������TIM1�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM1_Mode_Config(void)
{ 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	u16 CCR1_Val = 30000;        
	u16 CCR2_Val = 1700;
	u16 CCR3_Val = 1700;
	u16 CCR4_Val = 30000;

/* -----------------------------------------------------------------------
    TIM1 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM1CLK = 36 MHz, Prescaler = 0x0, TIM1 counter clock = 36 MHz
    TIM1 ARR Register = 999 => TIM1 Frequency = TIM1 counter clock/(ARR + 1)
    TIM1 Frequency = 36 KHz.
    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 60000-1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 23;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);	 //ʹ��ͨ��1


  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);	  //ʹ��ͨ��
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);			 // ʹ��TIM1���ؼĴ���ARR
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);                   //ʹ�ܶ�ʱ��1	
}

/*
 * ��������TIM1_Mode_Config
 * ����  ��TIM1 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM1���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM1_PWM_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}

/****************************************END OF FILE******************************************/
// /*
//  * ��������TIM3_GPIO_Config
//  * ����  ������TIM3�������PWMʱ�õ���I/O
//  * ����  ����
//  * ���  ����
//  * ����  ���ڲ�����
//  */
// #include "pwm_output.h"
// static void TIM3_GPIO_Config(void) 
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

// 	/* TIM3 clock enable */
// 	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����36MHz
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//   /* GPIOA and GPIOB clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

//   /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

//   GPIO_Init(GPIOB, &GPIO_InitStructure);
// }

// /*
//  * ��������TIM3_Mode_Config
//  * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
//  * ����  ����
//  * ���  ����
//  * ����  ���ڲ�����
//  */
//  void TIM3_Mode_Config(void)
// {
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	TIM_OCInitTypeDef  TIM_OCInitStructure;

// 	/* PWM�źŵ�ƽ����ֵ */
// 	u16 CCR1_Val = 500;        
// 	u16 CCR2_Val = 375;
// 	u16 CCR3_Val = 250;
// 	u16 CCR4_Val = 125;

// /* -----------------------------------------------------------------------
//     TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
//     TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
//     TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
//     TIM3 Frequency = 36 KHz.
//     TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
//     TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
//     TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
//     TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
//   ----------------------------------------------------------------------- */

//   /* Time base configuration */		 
//   TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
//   TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
//   TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

//   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//   /* PWM1 Mode configuration: Channel1 */
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

//   TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1

//   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel2 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2

//   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

//   /* TIM3 enable counter */
//   TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
// }

// /*
//  * ��������TIM3_Mode_Config
//  * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
//  *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
//  * ����  ����
//  * ���  ����
//  * ����  ���ⲿ����
//  */
// void TIM3_PWM_Init(void)
// {
// 	TIM3_GPIO_Config();
// 	TIM3_Mode_Config();	
// }


