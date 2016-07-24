/**
  ******************************************************************************
  * @file    BlueTooth.c
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015/7/5
  * @brief   ����������������   ��RobotMasters ���̵������
  * @Functions:   void BlueTooth_Init(void)   ������ʼ������
  *
  *               void Send_Data(float num, SetupTypeStruct Num)  ��������
  *                                                            ����ͳһΪfloat��ʽ
  *
  *               void USART3_IRQHandler(void)  ����������λ������,�ڲ���������
  *                                             ���յ���Ӧ���ݺ�Ĳ�����
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/
#include "Afei_Bluetooth.h"
#include "includes.h"
/******************************************************************************
* @fn BlueTooth_Init
*
* @brief lanya chuankou
*
* @return None.
*
* @note  chushihua
*/
s32 BlueToothInit(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef   USART_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
	  /* config USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USART2 GPIO config */
    /* Configure USART2 Rx (PA.3) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* Configure USART2 Tx (PA.2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_FLAG_TXE, ENABLE);

    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    return 1;
}
/****-------------------------------------------------------------------------------***/
u8 SEND_BUF[8] = {0xAA, 0x85, 0, 0, 0, 0, 0, 0};
/**
  * @brief  ������  �ڲ�����
  * @param  None
  * @retval None
  */
u32 Send_num = 0;
void Send_Array(u8 * Array, u16 Len)
{
    u16 i = 0;
    u8 temp = 0;
    for (; i < Len; i++)
    {
        temp = *Array++;
        USART_SendData(USART2, temp);
        while (USART_GetFlagStatus(USART2  , USART_FLAG_TC) == RESET);
    }
}

/******************************************************************************
* @fn Send_Data
*
* @param float num, uint8_t NUM_Type
*
* @return None.
*
* @note  ����λ�����Ͳ���   float num ��ֵ    uint8_t NUM_Type  ������������
*/
void Send_Data(const ValueStruct* num)
{

    SEND_BUF[2]=(u8)num->SetupType;/*��������*/
    SEND_BUF[3]=(u8)num->ValueType;/*����or����*/
    SEND_BUF[4] = num->Num.uchar_form[0] ;
    SEND_BUF[5] = num->Num.uchar_form[1] ;
    SEND_BUF[6] = num->Num.uchar_form[2] ;
    SEND_BUF[7] = num->Num.uchar_form[3] ;
    Send_Array(SEND_BUF, 8);

}
