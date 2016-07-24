
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.5.5
  * @note
  * @history    V1.0 2015.5.5
  *                 Transplant
  ******************************************************************************
  */

#include "includes.h"



/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : can中断接收
* Input          : None
* Output         : g_Motor_Yaw/g_Motor_Pitch        云台电机结构体参数

* Return         : None
*******************************************************************************/

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    union float_trans Trans_temple;
    CanRxMsg RxMessage;
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        if (RxMessage.StdId == Pitch_StdId )
        {
            g_Motor_Yaw.Measure_angle   = (u16)RxMessage.Data[0] << 8 |
                                          RxMessage.Data[1];     //接收机械角度值
            g_Motor_Yaw.Measure_current = (u16)RxMessage.Data[2] << 8 |
                                          RxMessage.Data[3];    //接收转矩电流测量值
            g_Motor_Yaw.Given_current   = (u16)RxMessage.Data[4] << 8 |
                                          RxMessage.Data[5];      //接收转矩电流给定值
            g_Motor_Yaw.Hall_Switch     =
                RxMessage.Data[6];                                     //接收霍尔开关值
        }
        else if (RxMessage.StdId == Yaw_StdId )
        {
            g_Motor_Pitch.Measure_angle   = (u16)RxMessage.Data[0] << 8 |
                                            RxMessage.Data[1];   //接收机械角度值
            g_Motor_Pitch.Measure_current = (u16)RxMessage.Data[2] << 8 |
                                            RxMessage.Data[3];  //接收转矩电流测量值
            g_Motor_Pitch.Given_current  = (u16)RxMessage.Data[4] << 8 |
                                           RxMessage.Data[5];   //接收转矩电流给定值
            g_Motor_Pitch.Hall_Switch  =
                RxMessage.Data[6];                              //接收霍尔开关值
        }
        else if (RxMessage.StdId == UnderPan_Speed )                 //接收地盘速度
        {
            g_iUnder_SpeedLF = (u32)RxMessage.Data[0] << 8 | (u32)RxMessage.Data[1];
            g_iUnder_SpeedRF = (u32)RxMessage.Data[2] << 8 | (u32)RxMessage.Data[3];
            g_iUnder_SpeedLB = (u32)RxMessage.Data[4] << 8 | (u32)RxMessage.Data[5];
            g_iUnder_SpeedRB = (u32)RxMessage.Data[6] << 8 | (u32)RxMessage.Data[7];
        }
        else if (RxMessage.StdId == UnderPan_Yaw )                   //接收地盘Yaw值
        {
            Trans_temple.uchar_form [0] = RxMessage.Data[0];
            Trans_temple.uchar_form [1] = RxMessage.Data[1];
            Trans_temple.uchar_form [2] = RxMessage.Data[2];
            Trans_temple.uchar_form [3] = RxMessage.Data[3];
            g_fUnder_Yaw = Trans_temple.float_form ;
        }
        else if (RxMessage.StdId ==
                 Gimbal_Angle)                    //接收云台编码器角度
        {
            Trans_temple.uchar_form [0] = RxMessage.Data [0];
            Trans_temple.uchar_form [1] = RxMessage.Data [1];
            Trans_temple.uchar_form [2] = RxMessage.Data [2];
            Trans_temple.uchar_form [3] = RxMessage.Data [3];
            g_fGimbal_EncYaw = Trans_temple.float_form ;
            Trans_temple.uchar_form [0] = RxMessage.Data [4];
            Trans_temple.uchar_form [1] = RxMessage.Data [5];
            Trans_temple.uchar_form [2] = RxMessage.Data [6];
            Trans_temple.uchar_form [3] = RxMessage.Data [7];
            g_fGimbal_EncPitch = Trans_temple.float_form ;
        }
        else if (RxMessage.StdId ==
                 Gimbal_Pitch)                    //接收云台陀螺仪Yaw值
        {
            Trans_temple.uchar_form [0] = RxMessage.Data [0];
            Trans_temple.uchar_form [1] = RxMessage.Data [1];
            Trans_temple.uchar_form [2] = RxMessage.Data [2];
            Trans_temple.uchar_form [3] = RxMessage.Data [3];
            g_fGimbal_Pitch = Trans_temple.float_form ;
        }
        else if (RxMessage.StdId == Control_Data)                    //接收控制数据
        {
            g_iShoot_Flag = RxMessage.Data [0];
            g_iAuto_Flag  = RxMessage.Data [1];
            g_iYaw_Speed   = (u16)RxMessage.Data [4] << 8 | RxMessage.Data [5];
            g_iPitch_Speed = (u16)RxMessage.Data [6] << 8 | RxMessage.Data [7];
        }
        else if (RxMessage.StdId ==
                 DiaoBiGyro )                   //?óê???±?íó?Yò?êy?Y
        {
            Trans_temple.uchar_form [0] = RxMessage.Data[0];
            Trans_temple.uchar_form [1] = RxMessage.Data[1];
            Trans_temple.uchar_form [2] = RxMessage.Data[2];
            Trans_temple.uchar_form [3] = RxMessage.Data[3];
            if (Trans_temple.float_form != 0)
                g_fUnder_Yaw = Trans_temple.float_form ;
        }
    }
}



/******************************************************************************
* @fn USART3_IRQHandler
*
* @brief 蓝牙中断
*
* @return None.
*
* @note 接收上位机接收数据
*/
u32 USART[8];
/*****************************/
void USART2_IRQHandler(void)
{
    static u8 i = 2;
    union float_trans usart_temp;
    if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(USART2, USART_FLAG_RXNE);
        if (i == 2)
        {
            USART[0] = USART[1];
            USART[1] = USART[2];
        }
        *(USART + i) = USART_ReceiveData(USART2);
        if (USART[0] == 0xAA && USART[1] == 0x85)
        {
            i++;
        }
        if (i == 8)
        {
            usart_temp.uchar_form[0] = USART[4];
            usart_temp.uchar_form[1] = USART[5];
            usart_temp.uchar_form[2] = USART[6];
            usart_temp.uchar_form[3] = USART[7];
            switch (USART[2])
            {
                case DRIVE_ERROR:
                    CanID.Num.int_form = usart_temp.uchar_form[2];
                    ErrorStatusFlag = (ErrorStatusType)usart_temp.uchar_form[0];
                    CanStatusFlag = (CanErrorType)usart_temp.uchar_form[1] ;
                    OverCurrentStatus = (OverCurrentStatusType)usart_temp.uchar_form[3];
                    ConnectingFlag = 1;
                    ConnectTimeLast = xTaskGetTickCount();
                    break;
                case MOTOR_DIRECT:
                    MotorDirect.Num.int_form = usart_temp.int_form;
                    break;
                case ENCODER_DIRECT:
                    EncoderDirect.Num.int_form = usart_temp.int_form;
                    break;
                case OVER_CURRENT_VALUE:
                    Over_Current_Value.Num.int_form = usart_temp.int_form;
                    break;
                case EXTREME_CURRENT_VALUE:
                    Extreme_Current_Value.Num.int_form = usart_temp.int_form;
                    break;
                case RECOVERY_CURRENT_VALUE:
                    Recovery_Current_Value.Num.int_form = usart_temp.int_form;
                    break;
                case CURRENT:
                    Current.Num.int_form = usart_temp.int_form;
                    break;
                case CONNECTED_ACK:
                    ConnectingFlag = 1;
                    ConnectTimeLast = xTaskGetTickCount ();
                    break;
            }
            i = 2;
        }
    }
}


