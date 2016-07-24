#include "usart2.h"

void USART2_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;//I/O¿ú3õê¼»ˉ½á11ìå
    NVIC_InitTypeDef NVIC_InitStructure;//ÖD¶Ï3õê¼»ˉ½á11ìå
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
 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	//配置PA2作为USART1　Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//配置PA3作为USART1　Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	//配置USART2
	//中断被屏蔽了
	USART_InitStructure.USART_BaudRate = 115200;       //波特率可以通过地面站配置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;    //禁用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
	//配置USART2时钟
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	//使能USART1接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//使能USART1
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
//	//发送中断
//	if((USART2->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//	{
//		USART2->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
//		if(TxCounter == count)
//		{
//			USART2->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断
//		}
//	}
	//接收中断 (接收寄存器非空) 
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
//		else if(RxState==3&&com_data<50)//长度
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
	//判断Str指向的数据是否有效.
	while(*Str)
	{
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\r')Uart2_Put_Char(0x0d);
		else if(*Str=='\n')Uart2_Put_Char(0x0a);
			else Uart2_Put_Char(*Str);
	//指针++ 指向下一个字节.
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

