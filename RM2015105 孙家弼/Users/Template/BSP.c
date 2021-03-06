
/**
  ******************************************************************************
  * @file    BSP.c
  * @author  Johnny Sun
  * @version V2.0
  * @date    2015.5.5
  * @note    这么高端的IO初始化
  * @history    V1.0 2015.3.31
  *                 其他未包含在模块中的初始化配置
  *             V1.1 2015.4.4
  *                 Refine.
  *             V1.2 2015.4.5
  *                 Refine.
  *             V2.0 2015.4.21
  *                 Compatible with other mainboard
  *             V2.2 2015.5.5
  *                 Refine.
  ******************************************************************************
  */

/*
函数详见 .h 底部
*/
#include "BSP.h"

void io_init_delay(void)
{
    u32 i;
    i=0xffff;
    while(i--);
}
/*简单封装了GPIO初始化程序*/
void MyGPIOInit(GPIO_TypeDef    * GPIOx,
                uint16_t          GPIO_Pin,
                GPIOMode_TypeDef  GPIO_Mode,
                GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*打开相应的时钟*/
    if (GPIO_Mode == GPIO_Mode_AF_OD ||
        GPIO_Mode == GPIO_Mode_AF_PP ||
        GPIO_Mode == GPIO_Mode_AIN  )
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //AFIO
    }
    switch ((uint32_t)GPIOx)
    {
        case (uint32_t)GPIOA :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            break;
        case (uint32_t)GPIOB :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            break;
        case (uint32_t)GPIOC :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
            break;
        case (uint32_t)GPIOD :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
            break;
        case (uint32_t)GPIOE :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
            break;
        case (uint32_t)GPIOF :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
            break;
        case (uint32_t)GPIOG :
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
            break;
    }

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed;

    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

///*GPIO写函数*/
//void MyGPIOWriteBit(GPIO_TypeDef * GPIOx,
//                    uint16_t       GPIO_Pin,
//                    IOLevel        Level)
//{
//    BitAction IOLevel;
//    switch ((uint8_t)Level)
//    {
//        case (uint8_t)LOW  : GPIO_WriteBit(GPIOx, GPIO_Pin,(BitAction)0);break;
//        case (uint8_t)HIGH : GPIO_WriteBit(GPIOx, GPIO_Pin,(BitAction)1);break;
//        case (uint8_t)TOGGLE:GPIOx->ODR ^= GPIO_Pin;
//            break;
//    }
//}


/*外部中断配置的简单封装*/
void MyEXTIInit(GPIO_TypeDef    *   GPIOx,
                uint16_t            GPIO_Pin,
                uint32_t            EXTI_Line,
                EXTIMode_TypeDef    EXTI_Mode,
                EXTITrigger_TypeDef EXTI_Trigger,
                FunctionalState     EXTI_LineCmd
               )
{
    uint8_t GPIO_PortSource;
    uint8_t GPIO_PinSource;
    EXTI_InitTypeDef EXTI_InitStructure;

    switch ((uint32_t)GPIOx)
    {
        case (uint32_t)GPIOA :
            GPIO_PortSource = GPIO_PortSourceGPIOA;
            break;
        case (uint32_t)GPIOB :
            GPIO_PortSource = GPIO_PortSourceGPIOB;
            break;
        case (uint32_t)GPIOC :
            GPIO_PortSource = GPIO_PortSourceGPIOC;
            break;
        case (uint32_t)GPIOD :
            GPIO_PortSource = GPIO_PortSourceGPIOD;
            break;
        case (uint32_t)GPIOE :
            GPIO_PortSource = GPIO_PortSourceGPIOE;
            break;
        case (uint32_t)GPIOF :
            GPIO_PortSource = GPIO_PortSourceGPIOF;
            break;
        case (uint32_t)GPIOG :
            GPIO_PortSource = GPIO_PortSourceGPIOG;
            break;
    }

    switch (GPIO_Pin)
    {
        case GPIO_Pin_1:
            GPIO_PinSource = GPIO_PinSource1;
            break;
        case GPIO_Pin_2:
            GPIO_PinSource = GPIO_PinSource2;
            break;
        case GPIO_Pin_3:
            GPIO_PinSource = GPIO_PinSource3;
            break;
        case GPIO_Pin_4:
            GPIO_PinSource = GPIO_PinSource4;
            break;
        case GPIO_Pin_5:
            GPIO_PinSource = GPIO_PinSource5;
            break;
        case GPIO_Pin_6:
            GPIO_PinSource = GPIO_PinSource6;
            break;
        case GPIO_Pin_7:
            GPIO_PinSource = GPIO_PinSource7;
            break;
        case GPIO_Pin_8:
            GPIO_PinSource = GPIO_PinSource8;
            break;
        case GPIO_Pin_9:
            GPIO_PinSource = GPIO_PinSource9;
            break;
        case GPIO_Pin_10:
            GPIO_PinSource = GPIO_PinSource10;
            break;
        case GPIO_Pin_11:
            GPIO_PinSource = GPIO_PinSource11;
            break;
        case GPIO_Pin_12:
            GPIO_PinSource = GPIO_PinSource12;
            break;
        case GPIO_Pin_13:
            GPIO_PinSource = GPIO_PinSource13;
            break;
        case GPIO_Pin_14:
            GPIO_PinSource = GPIO_PinSource14;
            break;
        case GPIO_Pin_15:
            GPIO_PinSource = GPIO_PinSource15;
            break;
    }

    GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);

    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
    EXTI_InitStructure.EXTI_LineCmd = EXTI_LineCmd;
    EXTI_Init(&EXTI_InitStructure);
}
const BinIOType ReservedStruct[] =
{
    GPIOA,  GPIO_Pin_15, /*W*/
    GPIOB,  GPIO_Pin_4, /*A*/
    GPIOB,  GPIO_Pin_3, /*S*/
    GPIOB,  GPIO_Pin_5, /*D*/
    GPIOB,  GPIO_Pin_6, /*A*/
    GPIOB,  GPIO_Pin_7, /*B*/
    GPIOB,  GPIO_Pin_8, /*X*/
    GPIOB,  GPIO_Pin_9, /*Y*/
    GPIOB,  GPIO_Pin_10, /*F1*/
    GPIOB,  GPIO_Pin_11, /*F2*/
    GPIOB,  GPIO_Pin_12, /*F3*/
    GPIOB,  GPIO_Pin_13, /*F4*/
};

s32 ReservedIOInit(void)
{
    u32 i;
    
    for (i = 0; i < length(ReservedStruct); i++)
    {
        MyGPIOInit(ReservedStruct[i].io, ReservedStruct[i].pin,
                   GPIO_Mode_IPD, GPIO_Speed_2MHz);
        io_init_delay();
    }
    return 1;
}

u32 ReadReservedIO(ReservedIOType ioName)
{
    return (ReservedStruct[ioName].io->IDR & ReservedStruct[ioName].pin);
};

const BinIOType CoreLedStruct[] =
{
    GPIOA,  GPIO_Pin_8,/*Red*/
    GPIOB,  GPIO_Pin_15,/*Green*/
    GPIOB,  GPIO_Pin_14,/*Blue*/
};
s32 CoreLedInit(void)
{
    u32 i;
    for (i = 0; i < length(CoreLedStruct); i++)
    {
        MyGPIOInit(CoreLedStruct[i].io, CoreLedStruct[i].pin,
                   GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
        io_init_delay();
    }
    CoreLed(red, on);
    CoreLed(green, off);
    CoreLed(blue, off);

    CoreLed(red, off);
//    i=0;
//    while(1)
//    {
//        i++;
//        CoreLed(white,off);
//        CoreLed(i%7,on);

//        u32 t=0x2ffffff;
//        while(t--);
//    }

    return 1;
}
void CoreLed(ColorType color, StatusType status)
{
#ifdef STM32F427_437xx
    color = red;
#endif
    if (color < yellow) /*三原色*/
    {
        switch (status)
        {
            case on:
                CoreLedStruct[color].io->BRR = CoreLedStruct[color].pin;
                break;
            case off:
                CoreLedStruct[color].io->BSRR = CoreLedStruct[color].pin;
                break;
            case toggle:
                CoreLedStruct[color].io->ODR ^= CoreLedStruct[color].pin;
                break;
        }
    }
    else if (color < white) /*双色混合色*/
    {
        ColorType color_comp1, color_comp2;
        color_comp1 = (ColorType)((u32)color - (u32)yellow);
        color_comp2 = (ColorType)((u32)color - (u32)blue);
        if (color_comp2 >= yellow) color_comp2 -= yellow;
        CoreLed(color_comp1, status);
        CoreLed(color_comp2, status);
    }
    else/*白色*/
    {
        CoreLed(red, status);
        CoreLed(green, status);
        CoreLed(blue, status);
    }
}
