/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "SysTick.h"


extern TIM_OCInitTypeDef  TIM_OCInitStructure;	

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}






void USART2_IRQHandler(void)
{
	int t=USART_ReceiveData(USART2);
	//USART_SendData(USART2, t);
	if(t==0x01)                                       //PC传送0x01即触发串口中断，舵机开始工作
	{
		
		TIM_OCInitStructure.TIM_Pulse = 10; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   
    Delay_Ms(2000);                                //这里需要加一个延时函数，以保证球都顺利从弹夹里落下
		TIM_OCInitStructure.TIM_Pulse = 80; 			      //舵机转回去把舱门关上															  
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  	Delay_Ms(1000);
		USART_SendData(USART2, t);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
  }
	if(t==0x02)                                       //串口接收0x02即触发中断，此时飞机已降落，两个电机开始转动
	{
		TIM_OCInitStructure.TIM_Pulse = 300; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???  
    TIM_OCInitStructure.TIM_Pulse = 0; 			        //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   	
    TIM_OCInitStructure.TIM_Pulse = 300; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);				//???????PWM???  
		TIM_OCInitStructure.TIM_Pulse = 80; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   
 	  
		
		
  }
}




void EXTI15_10_IRQHandler(void)
{
	
	int t;
	Delay_Ms(50); 
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);     //清除中断标志位
	  EXTI_ClearITPendingBit(EXTI_Line13);     //清除中断标志位
		return ;
	}
	USART_SendData(USART2, 'w');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);

  if(EXTI_GetITStatus(EXTI_Line12 ) != RESET) //确保行程开关1发生碰撞，判断是不是是否产生了EXTI Line12中断
  {
	  // 将减速电机1的两路pwm输出占空比都置为0
		//GPIO_WriteBit(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, 
		//             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11))));      //状态取反，在这里不对
		
  
		TIM_OCInitStructure.TIM_Pulse = 0; 			        //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
	  TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//	TIM_OC3Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
 
		
	}		
	
		if(EXTI_GetITStatus(EXTI_Line13 ) != RESET)     //确保行程开关2发生碰撞，判断是不是是否产生了EXTI Line12中断
	{			
		TIM_OCInitStructure.TIM_Pulse = 0; 			        //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//	TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
 
		          
  }  
	USART_SendData(USART2, 'q');
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
//	Delay_Ms(1000);                                    //这里需要加一个延时函数，以等待电机停转
	
	if(EXTI_GetITStatus(EXTI_Line12 ) != RESET && EXTI_GetITStatus(EXTI_Line13 ) != RESET) //判断是不是两个减速电机都停转，Y：电机1反转，转到中间
	{
		TIM_OCInitStructure.TIM_Pulse = 0; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???  
    TIM_OCInitStructure.TIM_Pulse = 900; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   	
    //Delay_Ms(2000);                                  //这里需要加一个延时函数2s，让电机有足够的时间走到中间停下，具体延时多久需要看具体调节情况（电机转速和距离决定）
		USART_SendData(USART2, t);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
  }
    EXTI_ClearITPendingBit(EXTI_Line12);     //清除中断标志位
	  EXTI_ClearITPendingBit(EXTI_Line13);     //清除中断标志位
	
}
// void EXTI15_10_IRQHandler(void)
// {
//   if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
//   {
// 	  // LED1 取反		
// 		GPIO_WriteBit(GPIOC, GPIO_Pin_13, 
// 		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
//     EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
//   }  
// 	
// 	
//     }




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/**********************************END OF FILE**********************************/
