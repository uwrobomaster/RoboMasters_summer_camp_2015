#ifndef __AFEI_BLUETOOTH_H
#define __AFEI_BLUETOOTH_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp.h"
/* Private breif   所有上位机显示小数点的数据接收时都要除以1000---------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/



/***********************************************/
void Send_Array(uint8_t * Array, uint16_t Len);
void Send_Data(const ValueStruct* num);
s32 BlueToothInit(void);
/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#endif //Rsing_Periph_Handle.h

