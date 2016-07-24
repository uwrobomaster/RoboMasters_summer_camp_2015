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
	if(t==0x01)                                       //PC����0x01�����������жϣ������ʼ����
	{
		
		TIM_OCInitStructure.TIM_Pulse = 10; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   
    Delay_Ms(2000);                                //������Ҫ��һ����ʱ�������Ա�֤��˳���ӵ���������
		TIM_OCInitStructure.TIM_Pulse = 80; 			      //���ת��ȥ�Ѳ��Ź���															  
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  	Delay_Ms(1000);
		USART_SendData(USART2, t);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
  }
	if(t==0x02)                                       //���ڽ���0x02�������жϣ���ʱ�ɻ��ѽ��䣬���������ʼת��
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
		EXTI_ClearITPendingBit(EXTI_Line12);     //����жϱ�־λ
	  EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
		return ;
	}
	USART_SendData(USART2, 'w');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);

  if(EXTI_GetITStatus(EXTI_Line12 ) != RESET) //ȷ���г̿���1������ײ���ж��ǲ����Ƿ������EXTI Line12�ж�
  {
	  // �����ٵ��1����·pwm���ռ�ձȶ���Ϊ0
		//GPIO_WriteBit(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, 
		//             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11))));      //״̬ȡ���������ﲻ��
		
  
		TIM_OCInitStructure.TIM_Pulse = 0; 			        //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
	  TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//	TIM_OC3Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
 
		
	}		
	
		if(EXTI_GetITStatus(EXTI_Line13 ) != RESET)     //ȷ���г̿���2������ײ���ж��ǲ����Ƿ������EXTI Line12�ж�
	{			
		TIM_OCInitStructure.TIM_Pulse = 0; 			        //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//	TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);				//???????PWM???    
 
		          
  }  
	USART_SendData(USART2, 'q');
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
//	Delay_Ms(1000);                                    //������Ҫ��һ����ʱ�������Եȴ����ͣת
	
	if(EXTI_GetITStatus(EXTI_Line12 ) != RESET && EXTI_GetITStatus(EXTI_Line13 ) != RESET) //�ж��ǲ����������ٵ����ͣת��Y�����1��ת��ת���м�
	{
		TIM_OCInitStructure.TIM_Pulse = 0; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);				//???????PWM???  
    TIM_OCInitStructure.TIM_Pulse = 900; 			      //?????,???=(CCRx/ARR)*100%?(TIM_Pulse/TIM_Period)*100%															//PWM??????Fpwm=72M/7200=1Mhz;  
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);				//???????PWM???   	
    //Delay_Ms(2000);                                  //������Ҫ��һ����ʱ����2s���õ�����㹻��ʱ���ߵ��м�ͣ�£�������ʱ�����Ҫ�����������������ת�ٺ;��������
		USART_SendData(USART2, t);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
  }
    EXTI_ClearITPendingBit(EXTI_Line12);     //����жϱ�־λ
	  EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
	
}
// void EXTI15_10_IRQHandler(void)
// {
//   if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//   {
// 	  // LED1 ȡ��		
// 		GPIO_WriteBit(GPIOC, GPIO_Pin_13, 
// 		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
//     EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
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
