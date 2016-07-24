/*********************************************************************************
 * 文件名  ：pwm_output.c
 * 描述    定时器TIM1输出4路PWM，其中两路驱动一部减速电机实现正反转，一路驱动一部减速电机实现正转；一路驱动一部舵机实现定角度转动         
 * 项目：Robomasters2015_Group11_Designed by LiChao
 * 硬件连接：---------------------
 *          |  PA.08: (TIM1_CH1)  |
 *          |  PA.09: (TIM1_CH2)  |
 *      	  |  PA.10: (TIM1_CH3)  | 
 *    		  |  PA.11: (TIM1_CH4)  |
 *           ---------------------    			
 * 
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "pwm_output.h"
extern TIM_OCInitTypeDef  TIM_OCInitStructure;		      //PWM ??????????
/*
 * 函数名：TIM1_GPIO_Config
 * 描述  ：配置TIM1复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM1_GPIO_Config(void) 
{
 
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM1 clock enable */                                                                                          // 定时器1时钟使能
	//PCLK1经过2倍频后作为TIM1的时钟源等于36MHz
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 

  /* GPIOA and GPIOB clock enable */                                                                               //IO时钟使能,都在2里
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA /*| RCC_APB2Periph_GPIOB*/, ENABLE); 

  /*GPIOA Configuration: TIM1 channel 1 2 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;                             // 配置四个通道
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                                                              // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //TIM1->BDTR |= 1<<15;
  /*GPIOB Configuration */                                                                           
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;             //串口 行程开关 蜂鸣器

 // GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * 函数名：TIM1_Mode_Config
 * 描述  ：配置TIM1输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM1_Mode_Config(void)
{ 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
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
  TIM_TimeBaseStructure.TIM_Period = 60000-1;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 23;	    //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);	 //使能通道1


  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);	  //使能通道
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);			 // 使能TIM1重载寄存器ARR
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);                   //使能定时器1	
}

/*
 * 函数名：TIM1_Mode_Config
 * 描述  ：TIM1 输出PWM信号初始化，只要调用这个函数
 *         TIM1的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM1_PWM_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}

/****************************************END OF FILE******************************************/
// /*
//  * 函数名：TIM3_GPIO_Config
//  * 描述  ：配置TIM3复用输出PWM时用到的I/O
//  * 输入  ：无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */
// #include "pwm_output.h"
// static void TIM3_GPIO_Config(void) 
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

// 	/* TIM3 clock enable */
// 	//PCLK1经过2倍频后作为TIM3的时钟源等于36MHz
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//   /* GPIOA and GPIOB clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

//   /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

//   GPIO_Init(GPIOB, &GPIO_InitStructure);
// }

// /*
//  * 函数名：TIM3_Mode_Config
//  * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
//  * 输入  ：无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */
//  void TIM3_Mode_Config(void)
// {
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	TIM_OCInitTypeDef  TIM_OCInitStructure;

// 	/* PWM信号电平跳变值 */
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
//   TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
//   TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
//   TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

//   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//   /* PWM1 Mode configuration: Channel1 */
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

//   TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1

//   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel2 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2

//   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

//   /* TIM3 enable counter */
//   TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
// }

// /*
//  * 函数名：TIM3_Mode_Config
//  * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
//  *         TIM3的四个通道就会有PWM信号输出
//  * 输入  ：无
//  * 输出  ：无
//  * 调用  ：外部调用
//  */
// void TIM3_PWM_Init(void)
// {
// 	TIM3_GPIO_Config();
// 	TIM3_Mode_Config();	
// }


