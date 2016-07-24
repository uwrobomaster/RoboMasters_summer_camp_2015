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

/* USARTy and USARTz configuration --------------------------------------------------USARTy��USARTz����--------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;               //����������Ϊ9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ���Ϊ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;       //һλֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;          //û����żУ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //Ӳ��������ʧЧ
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                    //�����շ�ʹ��

  /* Configure USARTy */
  USART_Init(USARTy, &USART_InitStructure);                                          //����USARTy
  /* Configure USARTz */
  USART_Init(USARTz, &USART_InitStructure);                                          //����USARTz
	/* Configure USARTx */
  USART_Init(USARTx, &USART_InitStructure);//wdj                                     //����USARTx
  
  /* Enable USARTy Receive and Transmit interrupts */                                //ʹ��USARTy�շ��ж�
  USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

  /* Enable USARTz Receive and Transmit interrupts */                                //ʹ��USARTz�շ��ж�
  USART_ITConfig(USARTz, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTz, USART_IT_TXE, ENABLE);
	
	/* Enable USARTx Receive and Transmit interrupts */                                //ʹ��USARTx�շ��ж�
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);

  /* Enable the USARTy */                                                            //ʹ��USARTy
  USART_Cmd(USARTy, ENABLE); 
  /* Enable the USARTz */                                                            //ʹ��USARTz
  USART_Cmd(USARTz, ENABLE);
	/* Enable the USARTx */
  USART_Cmd(USARTx, ENABLE);                                                         //ʹ��USARTx
  while (1)
  {
		 USART_SendData(USARTx, 0x31);
  }
}

/**
  * @brief  Configures the different system clocks.------------------------------------------ʹ�ܸ�ϵͳʱ��
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{   
  /* Enable GPIO clock */                                                                    //ʹ��IOʱ��
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK| USARTx_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTy Clock */                                                                  //ʹ��USARTyʱ��
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
//RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 

  /* Enable USARTz Clock */                                                                  //ʹ��USARTzʱ��
  RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);  
	
	/* Enable USARTx Clock */                                                                  //ʹ��USARTxʱ��
  RCC_APB1PeriphClockCmd(USARTx_CLK, ENABLE);  
}

/**
  * @brief  Configures the different GPIO ports.---------------------------------------------���ø�IO
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTy Rx as input floating */                                                 //��USARTy RX����Ϊ��������
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  
  /* Configure USARTz Rx as input floating */                                                 //��USARTz RX����Ϊ��������
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure); 

	/* Configure USARTx Rx as input floating */                                                 //��USARTx RX����Ϊ��������
  GPIO_InitStructure.GPIO_Pin = USARTx_RxPin;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure); 	
  
  /* Configure USARTy Tx as alternate function push-pull */                                   //��USARTy TX����Ϊ�����������
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  /* Configure USARTz Tx as alternate function push-pull */                                   //��USARTz TX����Ϊ�����������
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);

	/* Configure USARTx Tx as alternate function push-pull */                                   //��USARTx TX����Ϊ�����������
  GPIO_InitStructure.GPIO_Pin = USARTx_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  Configures the nested vectored interrupt controller.                              //����Ƕ�������жϿ�����
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */                                           //����NVIC�ж���ռ���ȼ� 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */                                                           //ʹ��USARTy�ж�
  NVIC_InitStructure.NVIC_IRQChannel = USARTy_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USARTz Interrupt */                                                           //ʹ��USARTz�ж�
  NVIC_InitStructure.NVIC_IRQChannel = USARTz_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USARTx Interrupt */                                                           //ʹ��USARTx�ж�
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



