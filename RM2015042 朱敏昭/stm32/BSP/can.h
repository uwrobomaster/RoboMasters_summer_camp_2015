#ifndef __CAN_H__
#define __CAN_H__

#include "system.h"

extern CanTxMsg TxMessage;			  //���ͻ�����
extern CanRxMsg RxMessage; 			//���ջ�����

void CAN_Configuration(void);
void CAN_SetMessage(void);

#endif 
