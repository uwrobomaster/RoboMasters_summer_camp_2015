/*********************************************************************************
 *@FileName   bsp_flash.c
 *@Version    V0.0.0.1
 *@Author     Johnbee
 *@Date       2015/04/18
 *@Brief      flash����
 *********************************************************************************/
/*#include************************************************************************/
#include "bsp_flash.h"
#include "includes.h"
/*ȫ�ֱ�������*********************************************************************/
const u8 SaveArrry_Len1 = MyComId_Num;


/*
 *@ <function name=> Flash_Actions() </function>
 *@ <summary>
     ��flash���ж�д����
 *@ </summary>
 *@ <param name="*myFlag">������־</param>
 *@ <returns> null </returns>
*/
void Flash_Actions(u8 myFlag)
{
    if (myFlag == 1) //дflash
    {
        Set_ComData();
        Flash_WriteFloat(ComData, MyComId_Num, Flash_Address1);
        ComData[Rec_SaveFlag] = 0;
    }
    else if (myFlag == 2) //��flash
    {
        Flash_ReadFloat(ComData, MyComId_Num, Flash_Address1);
        ComData[Rec_SaveFlag] = 0;
        Get_ComData();
#ifndef AfeiBuletooth
        Send_Data(HolderX_Sensitivity, Rec_HolderMouseX);
        Send_Data(HolderY_Sensitivity, Rec_HolderMouseY);
        Send_Data(UnderX_Sensitivity1, Rec_UnderMouseX1);
        Send_Data(Under_SpeedLv1, Rec_RightDialUp);
        Send_Data(Under_SpeedLv2, Rec_RightDialMid);
        Send_Data(Under_SpeedLv3, Rec_RightDialDown);
        Send_Data(UnderX_Sensitivity1, Rec_UnderMouseX1);
        Send_Data(UnderX_Sensitivity2, Rec_UnderMouseX2);
        Send_Data(MouseRegion, Rec_MouseRegion);

        Send_Data(Under_MoveMode1, Underpan_ModeFlag1);
        Send_Data(Under_MoveMode2, Underpan_ModeFlag2);
        Send_Data(Under_MoveMode3, Underpan_ModeFlag3);
        Send_Data(Speed_Add, Speed_Add_Flag);
        Send_Data(Speed_Dec, Speed_Dec_Flag);
        Send_Data(OmegaLv1, Rec_OmegaLv1);
        Send_Data(OmegaLv2, Rec_OmegaLv2);
        Send_Data(OmegaLv3, Rec_OmegaLv3);
        Send_Data(RM_Acce, Rec_RM_Acce);
        Send_Data(RM_Dece, Rec_RM_Dece);
        Send_Data(StaticSpeed, StaticOffSet);

        Send_Data(Mode5_SpeedLv1, Rec_Mode5_SpeedLv1);
        Send_Data(Mode5_SpeedLv2, Rec_Mode5_SpeedLv2);
        Send_Data(Mode5_SpeedLv3, Rec_Mode5_SpeedLv3);
        Send_Data(Mode5_QE_Sensitivity, Rec_Mode5_QE_Sensitivity);
        Send_Data(Mode5_Theta_Kp, Rec_Mode5_Theta_Kp);
        Send_Data(Mode5_Theta_Kd, Rec_Mode5_Theta_Kd);

        Send_Data(Mode5_HolderX1_Sensitivity, Rec_Mode5_HolderX1_Sensitivity);
        Send_Data(Mode5_HolderY1_Sensitivity, Rec_Mode5_HolderY1_Sensitivity);
        Send_Data(Mode5_HolderX2_Sensitivity, Rec_Mode5_HolderX2_Sensitivity);
        Send_Data(Mode5_HolderY2_Sensitivity, Rec_Mode5_HolderY2_Sensitivity);

        Send_Data(My_UnderPanPID.loc_kp, Rec_Loc_Kp);
        Send_Data(My_UnderPanPID.loc_ki, Rec_Loc_Ki);
        Send_Data(My_UnderPanPID.loc_kd, Rec_Loc_Kd);

        Send_Data(My_UnderPanPID.psi_kp, Rec_Psi_Kp);
        Send_Data(My_UnderPanPID.psi_ki, Rec_Psi_Ki);
        Send_Data(My_UnderPanPID.psi_kd, Rec_Psi_Kd);
#endif
    }
    else
    {
    }
}
/*
 *@ <function name=> Flash_WriteArry() </function>
 *@ <summary>
     дflash
 *@ </summary>
 *@ <param name="myArry">��Ҫд�������</param>
 *@ <param name="len">��Ҫд�������ĳ���</param>
 *@ <param name="myAddress">д��ĵ�ַ</param>
 *@ <returns> null </returns>
*/
void Flash_WriteArry(u32 myArry[], u8 len, uint32_t myAddress)
{
    int i = 0;
    FLASH_Status Flash_Status_1 = FLASH_COMPLETE;
//  FLASH_Status Flash_Status_2 = FLASH_COMPLETE;

//  RCC_HSICmd(ENABLE);
    FLASH_Unlock();

    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    if (myAddress == Flash_Address1)
    {
        Flash_Status_1 = FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);
        //  Flash_Status_1 = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3);
        while (Flash_Status_1 != FLASH_COMPLETE); //�ȴ���д���
        if (Flash_Status_1 ==  FLASH_COMPLETE)
            for (i = 0; i < len; i++)
            {
                if (Flash_Status_1 == FLASH_COMPLETE)
                {
                    Flash_Status_1 = FLASH_ProgramWord((myAddress + i * 4), (u32)myArry[i]);
                }
            }
    }
    if (myAddress == Flash_Address2)
    {
//      Flash_Status_1 = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3);
        Flash_Status_1 = FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
        if (Flash_Status_1 ==  FLASH_COMPLETE)
            for (i = 0; i < len; i++)
            {
                if (Flash_Status_1 == FLASH_COMPLETE)
                {
                    Flash_Status_1 = FLASH_ProgramWord((myAddress + i * 4), (u32)myArry[i]);
                }
            }
    }

    FLASH_Lock();

}

/*
 *@ <function name=> Flash_ReadArry() </function>
 *@ <summary>
     ��flash���������
 *@ </summary>
 *@ <param name="len">��ȡ�ĳ���</param>
 *@ <param name="myAddress">��ȡ�ĵ�ַ</param>
 *@ <returns> youArr:��������������< </returns>
*/
void Flash_ReadArry(u32 youArry[], u8 len, uint32_t myAddress)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        youArry[i] = (u32)( *(__IO uint32_t*)(myAddress + i * 4));
    }

}
/*
 *@ <function name=> Flash_WriteArry() </function>
 *@ <summary>
     ����������д��flash
 *@ </summary>
 *@ <param name="myArry">��Ҫд�������</param>
 *@ <param name="len">��Ҫд�������ĳ���</param>
 *@ <param name="myAddress">д��ĵ�ַ</param>
 *@ <returns> null </returns>
*/
void Flash_WriteFloat(float myArry[], u8 len, u32 myAddress)
{
    u32 temp_myarry[SaveArrry_Len1] = {0};
    u8 ii = 0;
    if (len > SaveArrry_Len1)
    {
        len = SaveArrry_Len1;
    }
    for (ii = 0; ii < len; ii++)
    {
        temp_myarry[ii] = *(u32*)(myArry + ii);
    }
    Flash_WriteArry(temp_myarry, len, myAddress);

}
/*
 *@ <function name=> Flash_ReadFloat() </function>
 *@ <summary>
     ��flash����ĸ�����
 *@ </summary>
 *@ <param name="len">��ȡ�ĳ���</param>
 *@ <param name="myAddress">��ȡ�ĵ�ַ</param>
 *@ <returns> youArr:��������������< </returns>
*/
void Flash_ReadFloat(float youArry[], u8 len, uint32_t myAddress)
{
    u32 temp_myarry[SaveArrry_Len1] = {0};
    u8 ii = 0;
    if (len > SaveArrry_Len1)
    {
        len = SaveArrry_Len1;
    }
    Flash_ReadArry(temp_myarry, len, myAddress);
    for (ii = 0; ii < len; ii++)
    {
        youArry[ii] = *(float*)(temp_myarry + ii);
    }
}
