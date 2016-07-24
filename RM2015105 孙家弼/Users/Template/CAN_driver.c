/*******************************************************************************
* Function Name  : Can_Driver
* Description    : Can总线的初始化，Can的发送函数，Can的接收中断
* Author         : Cp
* Date           : 2015/4/21
* Note           :
*                  2015.4.4
*                 加入volatile类型防优化——Johnny Sun
*                  2015.4.21
*                 Compatible with other mainboard
s32 CanInit()
void Send_GimBalMotor( int16_t Yaw_Give_Current, int16_t Pitch_Give_Current,CAN_TypeDef* CANx)
void Send_UnderMotor( s32 Speed_LF, s32 Speed_RF, s32 Speed_LB, s32 Speed_RB,CAN_TypeDef* CANx)
void Send_UnderYaw( float Yaw, CAN_TypeDef* CANx)
void Send_GimbalAngle( float Enc_Yaw, float Enc_Pitch , CAN_TypeDef* CANx)
void Send_GimbalPitch(float Pitch , CAN_TypeDef* CANx)
void Send_ControlData(s16 Pitch_Speed , s16 Yaw_Speed, u8 Shoot_Flag,u8 Auto_Flag, CAN_TypeDef* CANx)

*******************************************************************************/
#include "CAN_driver.h"
#include "Bsp.h"
/*两轴云台电机参数  机械角度，给定电流*/
volatile MotorTypeDef  g_Motor_Yaw, g_Motor_Pitch;   
/*鼠标的x,y轴的速度*/
volatile s16           g_iYaw_Speed, g_iPitch_Speed;    
/*鼠标给定x,y轴位置*/
volatile s32              g_iTarget_Yaw, g_iTarget_Pitch;      
/*地盘Yaw值  */
volatile float         g_fUnder_Yaw;  
/*云台Yaw,Pitch 编码器值*/
volatile float         g_fGimbal_EncYaw,g_fGimbal_EncPitch;     
         
/*云台陀螺仪Pitch*/
volatile float         g_fGimbal_Pitch;                      
u8            g_ucCAN_buffer[8];
/*射击标志位，自动瞄准标志位*/
volatile s32            g_iShoot_Flag, g_iAuto_Flag;    
/*底盘电机4个速度   */
volatile s32  g_iUnder_SpeedRF, g_iUnder_SpeedRB, 
                g_iUnder_SpeedLF, g_iUnder_SpeedLB;



/*******************************************************************************
* Function Name  : Can_Init
* Description    : Can初始化，与主控通信
* Input          : CAN1/CAN2
* Output         : None
* Return         : 1 Success
                   0 False
*******************************************************************************/
s32 CanInit(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_AFIO , ENABLE);

    // GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &gpio);    //CAN_RX

    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &gpio);    //CAN_TX

//    nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 0;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);

    nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN1);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack;//CAN_Mode_Normal;
    can.CAN_SJW = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_5tq;
    can.CAN_BS2 = CAN_BS2_3tq;//36/
    can.CAN_Prescaler = 4;     //CAN BaudRate 36/(1+5+3)/4=1Mbps
    CAN_Init(CAN1, &can);

    can_filter.CAN_FilterNumber = 0;
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = 0;
    can_filter.CAN_FilterIdLow = 0;
    can_filter.CAN_FilterMaskIdHigh = 0;
    can_filter.CAN_FilterMaskIdLow = 0;
    can_filter.CAN_FilterFIFOAssignment = CAN_FIFO0;
    can_filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&can_filter);
    CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE); // fifo0中断
//    CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE); // 发送中断
    CAN_ITConfig(CAN1, CAN_IT_LEC, ENABLE);
    return 1;
}
u32 CanErrorCounter[8];

//u32 CAN_Error_Detect()
//{
//    CanStatusFlag = (CanErrorType)(CAN_GetLastErrorCode(CAN1) >> 4);
//    CanErrorCounter[CanStatusFlag]++;
//    return CanStatusFlag;
//}

/*******************************************************************************
* Function Name  : Can_Send_Msg
* Description    : can发送一组数据
* Input          : len:数据长度,最大为8
                     msg:数据指针,最大为8个字节.
                     id:发送标识符
* Output         : None
* Return         : 1,成功;
                   0,失败;
*******************************************************************************/
u8 mbox_queue[3] = {0};
u8 mbox_index = 0;
u8 Can_Send_Msg(u8* msg, u8 len, uint32_t id, CAN_TypeDef* CANx)
{
    u8 mbox;
    u16 i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = id;                   //标准标识符，11位
    TxMessage.IDE = CAN_Id_Standard;        //使用标准标识符
    TxMessage.RTR = CAN_RTR_Data;           //发送数据帧
    TxMessage.DLC = len;                    //发送len字节
    for (i = 0; i < len; i++)
        TxMessage.Data[i] = msg[i];
    mbox = CAN_Transmit(CANx, &TxMessage);    //获取邮箱号
    mbox_queue[mbox_index] = mbox;
    mbox_index++;
    if (mbox_index > 2)
        mbox_index = 0;
    while ((CAN_TransmitStatus(CANx, mbox) != CAN_TxStatus_Ok)
           && (i < 0xfff))i++;   //等待发送结束
    if (i >= 0xfff)return 0;
    return 1;
}

/*******************************************************************************
* Function Name  : Send_GimbalMotor
* Description    : can发送一组电流数据给云台电机
* Input          : Yaw_Given_Current ,Pitch_Given_Current(给定电流 -5000~+5000) , CANx
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_GimBalMotor( int16_t Yaw_Give_Current, int16_t Pitch_Give_Current,
                       CAN_TypeDef* CANx)
{
    if (Pitch_Give_Current > 5000)
        Pitch_Give_Current = 5000;
    if (Pitch_Give_Current < -5000)
        Pitch_Give_Current = -5000;
    if (Yaw_Give_Current > 5000)
        Yaw_Give_Current = 5000;
    if (Yaw_Give_Current < -5000)
        Yaw_Give_Current = -5000;

    g_ucCAN_buffer[0] = Pitch_Give_Current >> 8 & 0xff;
    g_ucCAN_buffer[1] = Pitch_Give_Current >> 0 & 0xff;
    g_ucCAN_buffer[2] = 0;
    g_ucCAN_buffer[3] = 0;
    g_ucCAN_buffer[4] = Yaw_Give_Current >> 8 & 0xff;
    g_ucCAN_buffer[5] = Yaw_Give_Current >> 0 & 0xff;
    g_ucCAN_buffer[6] = 0;
    g_ucCAN_buffer[7] = 0;
    Can_Send_Msg(g_ucCAN_buffer, 0x08, Board_StdId , CANx);
}

/*******************************************************************************
* Function Name  : Send_GimbalMotor
* Description    : can发送一组速度数据给地盘电机
* Input          : Yaw_Given_Current ,Pitch_Given_Current(给定电流 -5000~+5000) , CANx
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_UnderMotor( s32 Speed_LF, s32 Speed_RF, s32 Speed_LB, s32 Speed_RB,
                      CAN_TypeDef* CANx)
{
#ifdef ELMO_ON_CAR
    Elmo_PVM(1,Speed_LF);
    Elmo_PVM(2,Speed_RF);
    Elmo_PVM(3,Speed_LB);
    Elmo_PVM(4,Speed_RB);
#elif defined VEYRON_ON_CAR
    g_ucCAN_buffer[0] = Speed_LF >> 8 & 0xff;
    g_ucCAN_buffer[1] = Speed_LF >> 0 & 0xff;
    g_ucCAN_buffer[2] = Speed_RF >> 8 & 0xff;
    g_ucCAN_buffer[3] = Speed_RF >> 0 & 0xff;
    g_ucCAN_buffer[4] = Speed_LB >> 8 & 0xff;
    g_ucCAN_buffer[5] = Speed_LB >> 0 & 0xff;
    g_ucCAN_buffer[6] = Speed_RB >> 8 & 0xff;
    g_ucCAN_buffer[7] = Speed_RB >> 0 & 0xff;
    Can_Send_Msg(g_ucCAN_buffer, 0x08, UnderPan_Speed , CANx);
#endif
}

/*******************************************************************************
* Function Name  : Send_UnderYaw
* Description    : can发送地盘的Yaw值
* Input          : 地盘的Yaw  , CANx
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_UnderYaw( float Yaw, CAN_TypeDef* CANx)
{
    union float_trans Trans_temple;
    Trans_temple.float_form  = Yaw;
    g_ucCAN_buffer[0] = Trans_temple.uchar_form[0];
    g_ucCAN_buffer[1] = Trans_temple.uchar_form[1];
    g_ucCAN_buffer[2] = Trans_temple.uchar_form[2];
    g_ucCAN_buffer[3] = Trans_temple.uchar_form[3];
    g_ucCAN_buffer[4] = 0;
    g_ucCAN_buffer[5] = 0;
    g_ucCAN_buffer[6] = 0;
    g_ucCAN_buffer[7] = 0;
    Can_Send_Msg(g_ucCAN_buffer, 0x08, UnderPan_Yaw , CANx);
}

/*******************************************************************************
* Function Name  : Send_GimbalAngle
* Description    : can发送云台的Pitch,Yaw值
* Input          : 云台的水平编码器Enc_Yaw ,云台的垂直编码艵nc_Pitch , CANx
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_GimbalAngle( float Enc_Yaw, float Enc_Pitch , CAN_TypeDef* CANx)
{
    union float_trans Trans_temple;
    Trans_temple.float_form  = Enc_Yaw;
    g_ucCAN_buffer[0] = Trans_temple.uchar_form[0];
    g_ucCAN_buffer[1] = Trans_temple.uchar_form[1];
    g_ucCAN_buffer[2] = Trans_temple.uchar_form[2];
    g_ucCAN_buffer[3] = Trans_temple.uchar_form[3];
    Trans_temple.float_form  = Enc_Pitch;
    g_ucCAN_buffer[4] = Trans_temple.uchar_form[0];
    g_ucCAN_buffer[5] = Trans_temple.uchar_form[1];
    g_ucCAN_buffer[6] = Trans_temple.uchar_form[2];
    g_ucCAN_buffer[7] = Trans_temple.uchar_form[3];
    Can_Send_Msg(g_ucCAN_buffer, 0x08, Gimbal_Angle , CANx);
}

/*******************************************************************************
* Function Name  : Send_GimbalPitch
* Description    : can发送云台上9250的Pitch值
* Input          : 云台Pitch, CANx
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_GimbalPitch(float Pitch , CAN_TypeDef* CANx)
{
    union float_trans Trans_temple;
    Trans_temple.float_form  = Pitch;
    g_ucCAN_buffer[0] = Trans_temple.uchar_form[0];
    g_ucCAN_buffer[1] = Trans_temple.uchar_form[1];
    g_ucCAN_buffer[2] = Trans_temple.uchar_form[2];
    g_ucCAN_buffer[3] = Trans_temple.uchar_form[3];
    g_ucCAN_buffer[4] = 0;
    g_ucCAN_buffer[5] = 0;
    g_ucCAN_buffer[6] = 0;
    g_ucCAN_buffer[7] = 0;
    Can_Send_Msg(g_ucCAN_buffer, 0x08, Gimbal_Pitch , CANx);
}

/*******************************************************************************
* Function Name  : Send_ControlData
* Description    : can发送控制数据
* Input          : 鼠标的y轴速度 Pitch_Speed  鼠标的x轴速度 Yaw_Speed  射击标志位 Shoot_Flag 自动瞄准 Auto_Flag
* Output         : None
* Return         : None
                   None
*******************************************************************************/
void Send_ControlData(s16 Pitch_Speed , s16 Yaw_Speed, u8 Shoot_Flag,
                      u8 Auto_Flag, CAN_TypeDef* CANx)
{

    g_ucCAN_buffer[0] = Shoot_Flag;
    g_ucCAN_buffer[1] = Auto_Flag;
    g_ucCAN_buffer[2] = 0;
    g_ucCAN_buffer[3] = 0;
    g_ucCAN_buffer[4] = Yaw_Speed >> 8 & 0xff;
    g_ucCAN_buffer[5] = Yaw_Speed >> 0 & 0xff;
    g_ucCAN_buffer[6] = Pitch_Speed >> 8 & 0xff;
    g_ucCAN_buffer[7] = Pitch_Speed >> 0 & 0xff;
    Can_Send_Msg(g_ucCAN_buffer, 0x08, Control_Data , CANx);
}




