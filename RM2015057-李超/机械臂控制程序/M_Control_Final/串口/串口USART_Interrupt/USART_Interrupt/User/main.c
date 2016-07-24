/**
  ******************************************************************************
  * @file    USART/Interrupt/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
#define RxBufferSize1   TxBufferSize2
#define RxBufferSize2   TxBufferSize1

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer1[] = "USART Interrupt Example: USARTy -> USARTz using Interrupt";
uint8_t TxBuffer2[] = "USART Interrupt Example: USARTz -> USARTy using Interrupt";
uint8_t RxBuffer1[RxBufferSize1];
uint8_t RxBuffer2[RxBufferSize2];
__IO uint8_t TxCounter1 = 0x00;
__IO uint8_t TxCounter2 = 0x00;
__IO uint8_t RxCounter1 = 0x00; 
__IO uint8_t RxCounter2 = 0x00;
uint8_t NbrOfDataToTransfer1 = TxBufferSize1;
uint8_t NbrOfDataToTransfer2 = TxBufferSize2;
uint8_t NbrOfDataToRead1 = RxBufferSize1;
uint8_t NbrOfDataToRead2 = RxBufferSize2;
__IO TestStatus TransferStatus1 = FAILED; 
__IO TestStatus TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
	
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System Clocks Configuration */
  RCC_Configuration();
       
  /* NVIC configuration */
  NVIC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

/* USARTy and USARTz configuration --------------------------------------------------USARTy和USARTz配置--------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;               //波特率配置为9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长设为8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;       //一位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;          //没有奇偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //硬件流控制失效
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                    //串口收发使能

  /* Configure USARTy */
  USART_Init(USARTy, &USART_InitStructure);                                          //配置USARTy
  /* Configure USARTz */
  USART_Init(USARTz, &USART_InitStructure);                                          //配置USARTz
	/* Configure USARTx */
  USART_Init(USARTx, &USART_InitStructure);//wdj                                     //配置USARTx
  
  /* Enable USARTy Receive and Transmit interrupts */                                //使能USARTy收发中断
  USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

  /* Enable USARTz Receive and Transmit interrupts */                                //使能USARTz收发中断
  USART_ITConfig(USARTz, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTz, USART_IT_TXE, ENABLE);
	
	/* Enable USARTx Receive and Transmit interrupts */                                //使能USARTx收发中断
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);

  /* Enable the USARTy */                                                            //使能USARTy
  USART_Cmd(USARTy, ENABLE); 
  /* Enable the USARTz */                                                            //使能USARTz
  USART_Cmd(USARTz, ENABLE);
	/* Enable the USARTx */
  USART_Cmd(USARTx, ENABLE);                                                         //使能USARTx
  while (1)
  {
		 USART_SendData(USARTx, 0x31);
  }
}

/**
  * @brief  Configures the different system clocks.------------------------------------------使能各系统时钟
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{   
  /* Enable GPIO clock */                                                                    //使能IO时钟
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK| USARTx_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTy Clock */                                                                  //使能USARTy时钟
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
//RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 

  /* Enable USARTz Clock */                                                                  //使能USARTz时钟
  RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);  
	
	/* Enable USARTx Clock */                                                                  //使能USARTx时钟
  RCC_APB1PeriphClockCmd(USARTx_CLK, ENABLE);  
}

/**
  * @brief  Configures the different GPIO ports.---------------------------------------------配置各IO
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTy Rx as input floating */                                                 //将USARTy RX配置为浮空输入
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  
  /* Configure USARTz Rx as input floating */                                                 //将USARTz RX配置为浮空输入
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure); 

	/* Configure USARTx Rx as input floating */                                                 //将USARTx RX配置为浮空输入
  GPIO_InitStructure.GPIO_Pin = USARTx_RxPin;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure); 	
  
  /* Configure USARTy Tx as alternate function push-pull */                                   //将USARTy TX配置为交替推挽输出
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  /* Configure USARTz Tx as alternate function push-pull */                                   //将USARTz TX配置为交替推挽输出
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);

	/* Configure USARTx Tx as alternate function push-pull */                                   //将USARTx TX配置为交替推挽输出
  GPIO_InitStructure.GPIO_Pin = USARTx_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  Configures the nested vectored interrupt controller.                              //配置嵌套向量中断控制器
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */                                           //配置NVIC中断抢占优先级 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */                                                           //使能USARTy中断
  NVIC_InitStructure.NVIC_IRQChannel = USARTy_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USARTz Interrupt */                                                           //使能USARTz中断
  NVIC_InitStructure.NVIC_IRQChannel = USARTz_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USARTx Interrupt */                                                           //使能USARTx中断
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



