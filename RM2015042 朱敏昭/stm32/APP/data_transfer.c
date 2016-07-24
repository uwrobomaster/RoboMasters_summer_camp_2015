#include "data_transfer.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 Data_Check,Send_Status,Send_Senser,Send_RCData,Send_Offset,Send_MotoPwm;
u8 Send_PID1,Send_PID2,Send_PID3,Send_PID4,Send_PID5,Send_PID6;
u8 data_to_send[50];

void Data_Receive_Anl(u8 *data_buf)
{
	vs16 rc_value_temp;
	u8 i;
	if(!(*(data_buf)==0xAA))		return;		//≈–∂œ÷°Õ∑
	if(*(data_buf+1)==0Xf1)
	{
	  Speed[0] = (int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//    if(*(data_buf+3)>>7)
//		{
//			*(data_buf+3)=*(data_buf+3)&0x7f;
//			Speed[0] = -(int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//		}
//		else
//		   Speed[0] = (int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//	
//		USART_SendData(USART2,Speed[0]>>8);
//		delay(5);		
//		USART_SendData(USART2,Speed[0]);
//		delay(5);
	}
	if(*(data_buf+1)==0Xf2)
	{
	  Speed[1] = (int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//	if(*(data_buf+3)>>7)
//		{
//			*(data_buf+3)=*(data_buf+3)&0x7f;
//			Speed[1] = -(int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//		}
//		else
//		   Speed[1] = (int16_t)(*(data_buf+3)<<8) | *(data_buf+4);
//			
	}

//	for(i = 0;i<(num-1);i++)
//		sum += *(data_buf+i);
//	if(!(sum==*(data_buf+num-1)))		return;		//≈–∂œsum
//	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//≈–∂œ÷°Õ∑

//	if(*(data_buf+2)==0X01)
//	{
//		if(*(data_buf+4)==0X01)
//		{
//	
//		}

//		if(*(data_buf+4)==0X02)
//		{
//       
//		}
//	
//		if(*(data_buf+4)==0X03)
//		{

//		}
//		if(*(data_buf+4)==0XA0)
//		{

//		}
//		if(*(data_buf+4)==0XA1)
//		{

//		}
//	}
//	
//	if(*(data_buf+2)==0X02)
//	{
//		if(*(data_buf+4)==0X01)
//		{
//			Send_PID1 = 1;
//			Send_PID2 = 1;
//			Send_PID3 = 1;
//			Send_PID4 = 1;
//			Send_PID5 = 1;
//			Send_PID6 = 1;
//		}
//		if(*(data_buf+4)==0X02)
//			Send_Offset = 1;
//	}

//	if(*(data_buf+2)==0X03)
//	{
//		
//		
//	}

//	if(*(data_buf+2)==0X10)								//PID
//	{
//		SpeedPID.P = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
//		SpeedPID.I = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
//		SpeedPID.D = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
//		Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X11)							
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X12)								
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X13)								
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X14)								
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X15)								
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0X16)								//OFFSET
//	{
//			Data_Send_Check(sum);
//	}
//	if(*(data_buf+2)==0x18)								
//	{

//	}
//	//////////////
//	if(*(data_buf+2)==0xf0)								
//	{
//		  Speed[0] = (int16_t)(*(data_buf+4)<<8) | *(data_buf+5);
//			
//	}
//	if(*(data_buf+2)==0xf0)								
//	{
//		   Speed[1] = (int16_t)(*(data_buf+4)<<8) | *(data_buf+5);
//	
//	}
//}

//void Data_Exchange(void)
//{

//}

//void Data_Send_Status(void)
//{
//	vs32 _temp2;
//	vs16 _temp;
//	u8 _cnt=0,sum = 0,i;
//	
//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0x01;
//	data_to_send[_cnt++]=0;
//	_temp = 0;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = 0;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = 0;
//	//_temp = (int)(Mag_Heading*100);
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp2 = 0;
//	data_to_send[_cnt++]=BYTE3(_temp2);
//	data_to_send[_cnt++]=BYTE2(_temp2);
//	data_to_send[_cnt++]=BYTE1(_temp2);
//	data_to_send[_cnt++]=BYTE0(_temp2);
//		
//	
//	data_to_send[_cnt++] = 0xA1;
//	
//	data_to_send[3] = _cnt-4;
//	
//	for(i = 0;i<_cnt;i++)
//		sum += data_to_send[i];
//	
//	data_to_send[_cnt++]=sum;
//	
//	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_RCData(void)
{

}

void Data_Send_MotoPWM(void)
{

}

void Data_Send_PID1(void)
{
	u8 _cnt = 0,sum = 0,i;	
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10;
	data_to_send[_cnt++]=0;

	_temp = SpeedPID.P;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = SpeedPID.I;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = SpeedPID.D;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_PID2(void)
{
	u8 _cnt=0,sum = 0,i;
	vs16 _temp;	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x11;
	data_to_send[_cnt++]=0;
	
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	

	Uart2_Put_Buf(data_to_send,_cnt);
}
void Data_Send_PID3(void)
{
	u8 _cnt=0,sum = 0,i;	
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x12;
	data_to_send[_cnt++]=0;
	
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_PID4(void)
{
	u8 _cnt=0,sum = 0,i;	
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x13;
	data_to_send[_cnt++]=0;
	
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_PID5(void)
{
	u8 _cnt=0,sum = 0,i;	
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x14;
	data_to_send[_cnt++]=0;
	
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_PID6(void)
{
	u8 _cnt=0,sum = 0,i;	
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x15;
	data_to_send[_cnt++]=0;
	
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i = 0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart2_Put_Buf(data_to_send,_cnt);
}

void Data_Send_Check(u16 check)
{
	u8 sum = 0,i;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xF0;
	data_to_send[3]=3;
	data_to_send[4]=0xBA;
	
	data_to_send[5]=BYTE1(check);
	data_to_send[6]=BYTE0(check);

	for(i = 0;i<7;i++)
		sum += data_to_send[i];
	
	data_to_send[7]=sum;

	Uart2_Put_Buf(data_to_send,8);
}
