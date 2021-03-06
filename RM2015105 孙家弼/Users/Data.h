/**
  ******************************************************************************
  * @file    Data.h
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.7.5
  * @note    C file of UI
  * @History    V1.0 2015.7.5
  ******************************************************************************
  */



#ifndef __Data_H__
#define __Data_H__
#include "stm32f10x.h"
#include "bsp.h"
/*错误优先级
数值越大优先级越高
*/
typedef enum
{
    Normal = 0,     /*运行正常*/
    NoSignal,       /*无速度信号*/
    CanBusError,    /*CAN总线错误*/
    OverCurrent,    /*过流错误*/
    ErrorStatusNum,
} ErrorStatusType;
typedef enum
{
    NoErr,              /*!< No Error */
    StuffErr,           /*!< Stuff Error */
    FormErr,            /*!< Form Error */
    ACKErr,             /*!< Acknowledgment Error */
    BitRecessiveErr,    /*!< Bit Recessive Error */
    BitDominantErr,     /*!< Bit Dominant Error */
    CRCErr,             /*!< CRC Error  */
    SoftwareSetErr,     /*!< Software Set Error */
} CanErrorType;
typedef enum
{
    ShortTimeOC = 0,  /*!< 短时间过流*/
    TimeOutOC,      /*!< 过流超时*/
    ShutDownOC,     /*!< 过流切断 */
} OverCurrentStatusType;
typedef enum
{
    MoveOut = 0,
    MoveIn,
    RunForrestRun,
    StopForrestStop,
} UpperCommandType;


extern ValueStruct CanID;
extern ValueStruct MotorDirect;
extern ValueStruct EncoderDirect;
extern ValueStruct SetSpeed;
extern ValueStruct Current;
extern ValueStruct Over_Current_Value;
extern ValueStruct Extreme_Current_Value;
extern ValueStruct Recovery_Current_Value;

extern const ValueStruct ConnectQuest;
extern const ValueStruct ResetCommand;
extern const ValueStruct SaveCommand;
extern CanErrorType CanStatusFlag;
extern ErrorStatusType ErrorStatusFlag;
extern OverCurrentStatusType OverCurrentStatus;

extern s32 ConnectingFlag;
extern u32 ConnectTimeLast;
extern u8 StatusText[][12];
extern int CtlMode;
#endif
