/*********************************************************************************
 *@FileName   bsp_flash.h
 *@Version    V0.0.0.1
 *@Author     JohnBee
 *@Date       2015/04/18
 *@Brief      bsp_flash.cͷ�ļ�
 *********************************************************************************/
#ifndef   _BSP_FLASH_H_
#define   _BSP_FLASH_H_

#ifdef  _cplusplus
extern  "C" {
#endif

#include "stm32f4xx.h"
#define   Flash_Address1    0x080C0000
#define   Flash_Address2    0x080E0000
    void Flash_Actions(u8 myFlag);
    void Flash_WriteFloat(float myArry[], u8 len, u32 myAddress);
    void Flash_ReadFloat(float youArry[], u8 len, uint32_t myAddress);
    void Flash_WriteArry(u32 myArry[], u8 len, uint32_t myAddress);
    void Flash_ReadArry(u32 youArry[], u8 len, uint32_t myAddress);

#ifdef  _cplusplus
}

#endif

#endif
