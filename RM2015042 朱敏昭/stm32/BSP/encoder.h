#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f10x.h"

void Encoder_Configuration(void);
void Encoder_Start(void);
int Encoder_Get_CNT(void);

#endif
