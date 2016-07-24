/**
  ******************************************************************************
  * @file    Oled.h
  * @author  Afei
  * @version V1.0
  * @date    2015.1.29
  * @note    file for Oled
  ******************************************************************************
  */

#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"
#include "bsp.h"
/*时钟*/

typedef enum
{
    SCL=0,
    SDA,
    RST,
    DC,
}OledIOType;



#define OLED_INVERSE_ON     (unsigned char)1
#define OLED_INVERSE_OFF    (unsigned char)0


#define OLED_SCL(a)	if (a)	\
					OledStruct[SCL].io->BSRR = OledStruct[SCL].pin;\
					else		\
					OledStruct[SCL].io->BRR = OledStruct[SCL].pin;//__nop();
/*数据*/
#define OLED_SDA(a)	if (a)	\
					OledStruct[SDA].io->BSRR = OledStruct[SDA].pin;\
					else		\
					OledStruct[SDA].io->BRR = OledStruct[SDA].pin;//__nop();
/*复位引脚*/
#define OLED_RST(a)	if (a)	\
					OledStruct[RST].io->BSRR = OledStruct[RST].pin;\
					else		\
					OledStruct[RST].io->BRR = OledStruct[RST].pin;//__nop();
/*对应SPI的MOSI*/
#define OLED_DC(a)	if (a)	\
					OledStruct[DC].io->BSRR = OledStruct[DC].pin;\
					else		\
					OledStruct[DC].io->BRR = OledStruct[DC].pin;//__nop();
///*使能引脚*/
//#define OLED_CS(a)	if (a)	\
//					GPIO_SetBit(HW_GPIOD,15);\
//					else		\
//					GPIO_ResetBit(HW_GPIOD,15)
                    
extern const BinIOType OledStruct[];
/***********************************************************/
void Dly_ms(uint16_t ms);
void OLED_WrCmd(uint8_t cmd);
void OLED_Fill(uint8_t bmp_data);
void OLED_PutPixel(uint8_t Inverse,uint8_t x,uint8_t y);
void OLED_Rectangle(uint8_t Inverse,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
void OLED_P6x8Str(uint8_t Inverse,uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P8x16Str(uint8_t Inverse,uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P14x16Str(uint8_t Inverse,uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_Print(uint8_t Inverse,uint8_t x, uint8_t y, uint8_t ch[]);
void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
void OLED_DLY_ms(uint16_t ms);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void Scroll_Showing(uint8_t *temp);
void OLED_CLS(void);
void OLED_Init(void);
void OLED_Boot(void);
void Standard_Printf(unsigned char inverse,unsigned char x,unsigned char y,const char *fmt,...);
#endif

