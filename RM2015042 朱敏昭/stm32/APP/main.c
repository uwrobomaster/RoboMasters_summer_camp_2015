#include "system.h"

int CAN_tx_data(void)
{
    CanTxMsg TxMessage;
    uint8_t TransmitMailbox = 0;
  u32 i;
   
    TxMessage.StdId=0x205;
    TxMessage.RTR=CAN_RTR_DATA;
    TxMessage.IDE=CAN_ID_STD;
    TxMessage.DLC=8;
    TxMessage.Data[0]=0x00;
    TxMessage.Data[1]=0x00;
    TxMessage.Data[2]=Speed[1]>>8;
    TxMessage.Data[3]=Speed[1];
	  TxMessage.Data[4]=0x00;
    TxMessage.Data[5]=0x00;
    TxMessage.Data[6]=0x00;
    TxMessage.Data[7]=0x00;
    TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
  {
   i++;
  }
}
int main(void)
{
 	systemInit();
	Motor1_Stop();
	while(1)
	{
		//Loop();
		//CAN_tx_data();
	}
}



