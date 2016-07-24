#ifndef __CAN_H__
#define __CAN_H__

#include "system.h"

extern CanTxMsg TxMessage;			  //发送缓冲区
extern CanRxMsg RxMessage; 			//接收缓冲区

void CAN_Configuration(void);
void CAN_SetMessage(void);

#endif 
