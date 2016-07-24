/********************************************************************************
 * 文件名  ：main.c
 * 描述    定时器TIM1输出4路PWM，其中两路驱动一部减速电机实现正反转，一路驱动一部减速电机实现正转；一路驱动一部舵机实现定角度转动  
 *           - PA.08: (TIM1_CH1)
 *           - PA.09: (TIM1_CH2)
 *           - PA.10: (TIM1_CH3)
 *           - PA.11: (TIM1_CH4) 
 *           TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
 *           TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
 *           TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
 *           TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
 *       
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "stm32f10x.h"
#include "pwm_output.h"
#include "stm32f10x_tim.h"
#include "exti.h"

#include "stm32f10x_exti.h"
#include "misc.h"
#include "pwm_output.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"


// #include "stm32f10x_gpio.h"
// #include "stm32f10x_rcc.h"
TIM_OCInitTypeDef  TIM_OCInitStructure;	
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */


extern void TIM1_PWM_Init(void);
extern void USART2_Configuration(void);


int main(void)
{
	
	bool ReadValue;
  /* 配置系统时钟为72M */
	SystemInit();
 
	/* TIM1 PWM波输出初始化，并使能TIM1 PWM输出 */
  //飞机起飞，飞到扔球区扔球，扔球，回到抓球区，等待PC发出指令，两个减速电机开始转动，共用3路pwm
	
	
//	TIM1_PWM_Init();                 //四路都开始输出pwm
 TIM1_PWM_Init();
	
	/*PB12 PB13分别连接两个行程开关，
	等滚子位置到达装球盒子边缘时
	任意一个行程开关发生碰撞进入中断就将两个减速电机停下
	*/
	
	EXTI_PB12_Config();
 EXTI_PB13_Config();	
	
	USART2_Configuration();
	
	key_init();
	
  while (1)
  {
	//	ReadValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
		
		//USART_ReceiveData(USART2);
	//	USART_SendData(USART2, ReadValue);
}
//		return 0;
}


/**********************************END OF FILE*************************************/



/********************************************************************************
 * 文件名  ：main.c
 * 描述    定时器TIM1输出4路PWM，其中两路驱动一部减速电机实现正反转，一路驱动一部减速电机实现正转；一路驱动一部舵机实现定角度转动  
 *           - PA.08: (TIM1_CH1)
 *           - PA.09: (TIM1_CH2)
 *           - PA.10: (TIM1_CH3)
 *           - PA.11: (TIM1_CH4) 
 *           TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
 *           TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
 *           TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
 *           TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
 *       
 * 库版本  ：ST3.5.0
**********************************************************************************/

