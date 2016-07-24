/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：exti.c
 * 描述    ：I/O线中断应用函数库        
 * 实验平台：野火STM32开发板
 * 硬件连接：-------------------------
 *          | PB12 - sensor1              |
 *          | PB13 - sensor2(本实验没用到)|
 *          |                         |
 *           -------------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "exti.h"


/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置嵌套向量中断控制器NVIC  
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 12;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：EXTI_PB12_Config
 * 描述  ：配置 PB12 为线中断口，并设置中断优先级
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void EXTI_PB12_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PB12) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	/* config the NVIC(PB12) */
	NVIC_Configuration();

	/* EXTI line gpio config(PB12) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PB12) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
void key_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	/* EXTI line gpio config(PB12) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void EXTI_PB13_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PB13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	/* config the NVIC(PB13) */
	NVIC_Configuration();

	/* EXTI line gpio config(PB13) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PB13) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/

