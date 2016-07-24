/**
  ******************************************************************************
  * @file    Data.c
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.7.5
  * @note    C file of UI
  * @History    V1.0 2015.7.5
  ******************************************************************************
  */

#include "Data.h"

/*int*/
ValueStruct CanID = {0, 1, 15, VALUE_TYPE_INTEGER, CAN_ID};
ValueStruct MotorDirect = {0, 0, 1, VALUE_TYPE_INTEGER, MOTOR_DIRECT};
ValueStruct EncoderDirect = {0, 0, 1, VALUE_TYPE_INTEGER, ENCODER_DIRECT};
ValueStruct SetSpeed = {0, -210, 210, VALUE_TYPE_INTEGER, SET_SPEED};
ValueStruct Over_Current_Value = {0, 0, 100, VALUE_TYPE_FLOAT, OVER_CURRENT_VALUE}; /*A*/
ValueStruct Extreme_Current_Value = {0, 0, 100, VALUE_TYPE_FLOAT, EXTREME_CURRENT_VALUE}; /*A*/
ValueStruct Recovery_Current_Value = {0, 0, 20, VALUE_TYPE_FLOAT, RECOVERY_CURRENT_VALUE}; /*A*/
const ValueStruct ConnectQuest = {0, 0, 0, VALUE_TYPE_INTEGER, CONNECTED_QUEST};
const ValueStruct ResetCommand = {0, 0, 0, VALUE_TYPE_INTEGER, RESET_COMMAND};
const ValueStruct SaveCommand = {0, 0, 0, VALUE_TYPE_INTEGER, SAVE_COMMAND};
/*float*/
ValueStruct Current = {0, -100, 100, VALUE_TYPE_FLOAT, CURRENT};

CanErrorType CanStatusFlag = NoErr;
ErrorStatusType ErrorStatusFlag = Normal;
OverCurrentStatusType OverCurrentStatus;


int CtlMode;
u32 ConnectTimeLast;
s32 ConnectingFlag = 0;
u8 StatusText[][12] =
{
    "Normal",
    "NoSignal",
    "CanBusError",
    "OverCurrent",
};
