#include "stm32f10x.h"

#ifndef       __CAN_DRIVER_H__
#define       __CAN_DRIVER_H__



typedef struct MotorTypeDef
{
    s16  Given_current;     //����ת�ص���ֵ
    s16  Measure_current;   //ʵ��ת�ص���ֵ
    s16  Measure_angle;     //ʵ�ʻ�е�Ƕ�ֵ
    u8   Hall_Switch;       //��������ֵ
} MotorTypeDef;

typedef struct GyroTypeDef
{
    float  Yaw;
    float  Pitch;
    float  Roll;
} GyroTypeDef;



#define Yaw_StdId   	(uint32_t)0x201          //��̨Yaw�� 
#define Pitch_StdId  	(uint32_t)0x203			 //��̨Pitch��
#define Board_StdId  	(uint32_t)0x200			 //��̨����������
#define UnderPan_Yaw	(uint32_t)0x170			 //����Yawֵ
#define UnderPan_Speed  (uint32_t)0x100			 //�����ٶ�
#define Gimbal_Angle    (uint32_t)0x178 		 //��̨�������Ƕ�
#define Gimbal_Pitch    (uint32_t)0x17A			 //��̨������Pitchֵ
#define Control_Data    (uint32_t)0x180			 //��̨��������
#define DiaoBiGyro      (uint32_t)0x11
#define Pitch_Max (float)15.0
#define Yaw_Max   (float)720.0				//cyq:��̨�Ƕȵķ�Χ


volatile extern MotorTypeDef  g_Motor_Yaw,g_Motor_Pitch;      		 //������̨�������  ��е�Ƕȣ���������             		 //����ŷ���ǲ���
volatile extern s16           g_iYaw_Speed,g_iPitch_Speed;   	     //����x,y����ٶ�
volatile extern s32			 g_iTarget_Yaw,g_iTarget_Pitch;	         //������x,y��λ��
volatile extern float         g_fUnder_Yaw;							 //����Yawֵ
volatile extern float         g_fGimbal_EncYaw, g_fGimbal_EncPitch;	 //��̨Yaw,Pitch ������ֵ
volatile extern float         g_fGimbal_Pitch;						 //��̨������Pitch
extern u8            g_ucCAN_buffer[8];					 
volatile extern s32            g_iShoot_Flag, g_iAuto_Flag;		 //�����־λ���Զ���׼��־λ
volatile extern s32           g_iUnder_SpeedRF, g_iUnder_SpeedRB, g_iUnder_SpeedLF, g_iUnder_SpeedLB;
													 //���̵��4���ٶ�


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





