#include "main.h"
extern char Rx_data;
	u8 Can_send_flag=1;
int main(void)
{  
  u8 j=0;

	u8 cnt=0;
  u8 canbuf[8];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	CAN1_Configuration();
	PWM_Configuration();
	USART3_Configuration();
	Led_Configuration();
	delay_ms(1000);
	TIM1->CCR1=2100;

	//LED_GREEN_ON();
    while(1)
    {
    delay_ms(10);
//		USART3_SendChar(2);
//		if(Rx_data==3)
//				LED_RED_TOGGLE();
			for(j=0;j<8;j++)
			{
				cnt=0;
				canbuf[j]=cnt+j;//填充发送缓冲区
			}
			if(Rx_data==0) canbuf[0]=0;
			if(Rx_data==1) canbuf[0]=1;
			if(Rx_data==2) canbuf[0]=2;
		Can_send_flag=Can_Send_Msg(canbuf,8);//发送8个字节
			if(Rx_data==4)  
			{
				TIM1->CCR1=100;
				delay_ms(1000);
				TIM1->CCR1=210;
			}
				

    }
}
