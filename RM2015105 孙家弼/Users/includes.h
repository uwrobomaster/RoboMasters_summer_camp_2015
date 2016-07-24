
/**
  ******************************************************************************
  * @file    includes.h
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.3.28
  * @note
  * @history    V1.0 2015.3.28
  *                 include file of main.
  ******************************************************************************
  */
#ifndef __INCLUDES_H
#define __INCLUDES_H
#define FIRMWARE_VERSION "Firmware V1.0.1"

#include "ui.h"
/*driver incluede*/
#include "stm32f10x.h"
#include "Oled.h"
#include "BSP.h"
#include "Afei_Bluetooth.h"
#include "CAN_driver.h"
#include "Data.h"

/*other incluede*/
#include "arm_math.h"
#include "cpustate.h"

/*RTOS incluede*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*0：空闲   1：统计    2：预留   3：开始为常规任务最小优先级*/
#define CORE_TASK_PRIORITY  5
#define UART_TASK_PRIORITY  4
#define UI_TASK_PRIORITY    3
#define STATUS_TASK_PRIORITY 2
#define STAT_TASK_PRIO tskIDLE_PRIORITY+1//统计任务的优先级(1)，仅仅高于空闲任务(0)

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK                      ( 0UL )
extern QueueHandle_t xUSARTQueue;

/*核心任务*/
void prvCoreTask(void *pvParameters );
/*上位机串口发送任务*/
void prvUartTask(void *pvParameters );
/*蜂鸣器及状态LED任务*/
void prvStatusTask(void *pvParameters );
void prvSetupHardware( void );
int vUsartTimerSTart(void);
#endif  //__INCLUDES_H

