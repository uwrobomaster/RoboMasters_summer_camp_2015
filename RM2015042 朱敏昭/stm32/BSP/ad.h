#ifndef __AD_H__
#define __AD_H__

#include "system.h"

void ADC_Configuration(void);
int ADC_Filter(void);
void Current_Detect(void);

extern float Current;

#endif
