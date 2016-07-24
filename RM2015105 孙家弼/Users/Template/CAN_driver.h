#include "stm32f10x.h"

#ifndef       __CAN_DRIVER_H__
#define       __CAN_DRIVER_H__



typedef struct MotorTypeDef
{
    s16  Given_current;     //给定转矩电流值
    s16  Measure_current;   //实际转矩电流值
    s16  Measure_angle;     //实际机械角度值
    u8   Hall_Switch;       //霍尔开关值
} MotorTypeDef;

typedef struct GyroTypeDef
{
    float  Yaw;
    float  Pitch;
    float  Roll;
} GyroTypeDef;



#define Yaw_StdId   	(uint32_t)0x201          //云台Yaw轴 
#define Pitch_StdId  	(uint32_t)0x203			 //云台Pitch轴
#define Board_StdId  	(uint32_t)0x200			 //云台驱动电流板
#define UnderPan_Yaw	(uint32_t)0x170			 //地盘Yaw值
#define UnderPan_Speed  (uint32_t)0x100			 //地盘速度
#define Gimbal_Angle    (uint32_t)0x178 		 //云台编码器角度
#define Gimbal_Pitch    (uint32_t)0x17A			 //云台陀螺仪Pitch值
#define Control_Data    (uint32_t)0x180			 //云台控制数据
#define DiaoBiGyro      (uint32_t)0x11
#define Pitch_Max (float)15.0
#define Yaw_Max   (float)720.0				//cyq:云台角度的范围


volatile extern MotorTypeDef  g_Motor_Yaw,g_Motor_Pitch;      		 //两轴云台电机参数  机械角度，给定电流             		 //底盘欧拉角参数
volatile extern s16           g_iYaw_Speed,g_iPitch_Speed;   	     //鼠标的x,y轴的速度
volatile extern s32			 g_iTarget_Yaw,g_iTarget_Pitch;	         //鼠标给定x,y轴位置
volatile extern float         g_fUnder_Yaw;							 //地盘Yaw值
volatile extern float         g_fGimbal_EncYaw, g_fGimbal_EncPitch;	 //云台Yaw,Pitch 编码器值
volatile extern float         g_fGimbal_Pitch;						 //云台陀螺仪Pitch
extern u8            g_ucCAN_buffer[8];					 
volatile extern s32            g_iShoot_Flag, g_iAuto_Flag;		 //射击标志位，自动瞄准标志位
volatile extern s32           g_iUnder_SpeedRF, g_iUnder_SpeedRB, g_iUnder_SpeedLF, g_iUnder_SpeedLB;
													 //底盘电机4个速度


s32 CanInit(void);
u8 Can_Send_Msg(u8* msg, u8 len, uint32_t id,CAN_TypeDef* CANx);
void Send_GimBalMotor( int16_t Yaw_Give_Current, int16_t Pitch_Give_Current,CAN_TypeDef* CANx);
void Send_UnderMotor( s32 Speed_LF, s32 Speed_RF, s32 Speed_LB, s32 Speed_RB,CAN_TypeDef* CANx);
void Send_UnderYaw( float Yaw, CAN_TypeDef* CANx);
void Send_GimbalAngle( float Enc_Yaw, float Enc_Pitch , CAN_TypeDef* CANx);
void Send_GimbalPitch(float Pitch , CAN_TypeDef* CANx);
void Send_ControlData(s16 Pitch_Speed , s16 Yaw_Speed, u8 Shoot_Flag,u8 Auto_Flag, CAN_TypeDef* CANx);
void Can_Init(CAN_TypeDef* CANx); 

#endif





