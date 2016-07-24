#ifndef __USART_H__
#define __USART_H__

#include "system.h"

void USART2_Configuration(void);
void USART2_IRQHandler(void);

void Uart2_Put_String(unsigned char *Str);
void Uart2_Put_Buf(unsigned char *DataToSend , u8 data_num);

#endif
