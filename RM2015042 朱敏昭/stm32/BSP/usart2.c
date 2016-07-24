#include "usart2.h"

void USART2_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;//I/O�0�7��3�0�1���0�4�0�3���0�5��11���0�2
    NVIC_InitTypeDef NVIC_InitStructure;//�0�0D�0�9�0�33�0�1���0�4�0�3���0�5��11���0�2
    USART_InitTypeDef USART_InitStructure;
	  USART_ClockInitTypeDef USART_ClockInitStruct;
	
	/* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    
    		 /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	//����PA2��ΪUSART1��Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//����PA3��ΪUSART1��Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	//����USART2
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = 115200;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART2ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	//ʹ��USART1�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(USART2, ENABLE); 
   
}

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0;
static u8 RxBuffer[50];
static u8 RxState = 0;

void USART2_IRQHandler(void)
{
	//char t=USART_ReceiveData(USART2);
	LED_GREEN_ON();
	//USART_SendData(USART2,t);
//	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
//    {
//      USART_ReceiveData(USART1);
//    }
//		
//	//�����ж�
//	if((USART2->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//	{
//		USART2->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
//		if(TxCounter == count)
//		{
//			USART2->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�
//		}
//	}
	//�����ж� (���ռĴ����ǿ�) 
	if(USART2->SR & (1<<5))    
	{
		u8 com_data = USART2->DR;
		static u8 _data_len = 0,_data_cnt = 0;

		if(RxState == 0&&com_data==0xAA)
		{
			RxState=1;
			RxBuffer[0]=com_data;
		}
		else if(RxState==1)
		{
			RxState=2;
			RxBuffer[1]=com_data;
		}
		else if(RxState==2&&com_data>0&&com_data<100)
		{
			RxState=3;
			RxBuffer[2]=com_data;
			_data_len = com_data;
			_data_cnt = 0;
		}
		else if(RxState==3&&_data_len>0)  
		{
			_data_len--;
			RxBuffer[3+_data_cnt++]=com_data;
			//delay(5);
			if(_data_len<=0)
			{
				delay(5);
				RxState = 0;
				Data_Receive_Anl(RxBuffer);
			}
		}
		else
    		RxState = 0;
	 }
	
//		u8 com_data = USART2->DR;
//		static u8 _data_len = 0,_data_cnt = 0;

//		if(RxState == 0&&com_data==0xAA)
//		{
//			RxState=1;
//			RxBuffer[0]=com_data;
//		}
//		else if(RxState==1&&com_data==0xAF)
//		{
//			RxState=2;
//			RxBuffer[1]=com_data;
//		}
//		else if(RxState==2&&com_data>0&&com_data<0XF1)
//		{
//			RxState=3;
//			RxBuffer[2]=com_data;
//		}
//		else if(RxState==3&&com_data<50)//����
//		{
//			RxState = 4;
//			RxBuffer[3]=com_data;
//			_data_len = com_data;
//			_data_cnt = 0;
//		}
//		else if(RxState==4&&_data_len>0)
//		{
//			_data_len--;
//			RxBuffer[4+_data_cnt++]=com_data;
//			if(_data_len==0)
//				RxState = 5;
//		}
//		else if(RxState==5)
//		{
//			RxState = 0;
//			RxBuffer[4+_data_cnt]=com_data;
//			Data_Receive_Anl(RxBuffer,_data_cnt+5);
//		}
//		else
//			RxState = 0;
//	}
}

static void Uart2_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
}
void Uart2_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str)
	{
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')Uart2_Put_Char(0x0d);
		else if(*Str=='\n')Uart2_Put_Char(0x0a);
			else Uart2_Put_Char(*Str);
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}
void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num)
{
	u8 i;
	for(i=0;i<data_num;i++)
		TxBuffer[count++] = *(DataToSend+i);
	if(!(USART2->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 
}

