
/**
  ******************************************************************************
  * @file    BSP.h
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.4.21
  * @note
  * @history    V1.0 2015.3.31
  *                 其他未包含在模块中的初始化配置
  *             V2.0 2015.4.21
  *                 Compatible with other mainboard
  ******************************************************************************
  */
  

#ifndef __BSP_H__
#define __BSP_H__
#include "stm32f10x.h"

/*定义当前板子类型*/

#define rad2ang(rad)  ((rad)/PI*180.f)
#define ang2rad(ang)  ((ang)*PI/180.f)
#define TickByms(ms) (ms * (configTICK_RATE_HZ / 1000))
#define length(a)                       (sizeof(a) / sizeof(a[0]))
    
#define OLED_SCREEN
#define OLED_SCL_GPIO           GPIOA
#define OLED_SCL_PIN       GPIO_Pin_5
#define OLED_SDA_GPIO           GPIOA
#define OLED_SDA_PIN       GPIO_Pin_6
#define OLED_RST_GPIO           GPIOA
#define OLED_RST_PIN       GPIO_Pin_4
#define OLED_DC_GPIO            GPIOA
#define OLED_DC_PIN        GPIO_Pin_7
/*___________________________来自遥控驱动数据**_____________________________*/

typedef enum
{
    ENCODER_DIRECT,
    CAN_ID,
    MOTOR_DIRECT,
    SET_SPEED,

    DRIVE_ERROR,
    CURRENT,  
    OVER_CURRENT_VALUE,
    EXTREME_CURRENT_VALUE,
    RECOVERY_CURRENT_VALUE,
    
    RESET_COMMAND,
    SAVE_COMMAND,
    CONNECTED_QUEST,
    CONNECTED_ACK,
}SetupType;


/*自定义预留端口名字*/
union float_trans
{
	float float_form;
    s32     int_form;
	unsigned char uchar_form[4];
};

/* Defines for the UI parameter */
typedef enum    
{
    VALUE_TYPE_INTEGER,
    VALUE_TYPE_FLOAT,
}ValueTypeType;

typedef struct
{
    union float_trans Num;
    const union float_trans MinValue;
    const union float_trans MaxValue;
    const ValueTypeType ValueType;
    const SetupType SetupType;
}ValueStruct;

typedef enum
{
    UpKey = 0,
    LeftKey,
    DownKey,
    RightKey,
    AKey,
    BKey,
    XKey,
    YKey,
    F1Key,
    F2Key,
    F3Key,
    F4Key,
}ReservedIOType;

typedef struct BinIOType
{
    GPIO_TypeDef*    io;
    u32             pin;    
}BinIOType;

typedef struct PwmType
{
    GPIO_TypeDef*    io;
    u32             pin;   
    u32             source;
}PwmType;

typedef enum IOStatusType
{
    Low,
    High,
}IOStatusType;

typedef enum StatusType
{
    off = 0,
    on,
    toggle,
}StatusType;

typedef enum ColorType
{
    red     = 0,
    green   = 1,
    blue    = 2,
    yellow  = 3,
    cyan    = 4,
    purple  = 5,
    white   = 6,
}ColorType;

void io_init_delay(void);
void MyGPIOInit(GPIO_TypeDef    * GPIOx,
                uint16_t          GPIO_Pin,
                GPIOMode_TypeDef  GPIO_Mode,
                GPIOSpeed_TypeDef GPIO_Speed);

s32 ReservedIOInit(void);
u32 ReadReservedIO(ReservedIOType ioName);

s32 IWDG_Init(void);
//s32 EncoderInit(void);
void IWDG_Feed(void);
s32 CoreLedInit(void);
void CoreLed(ColorType color,StatusType status);
#endif /* __BSP_H__ */
