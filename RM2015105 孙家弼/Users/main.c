
/**
  ******************************************************************************
  * @file    main.c
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.5.5
  * @note
  * @history    V1.0 2015.3.27
  *                 Set up a basic framwork.
  *             V1.1 2015.4.21
  *                 Add work status task
  *             V1.2 2015.5.5
  *                 Refine
  *             V1.210 2015.5
  *                 Change for embedded board in dji summer camp .
  ******************************************************************************
  */

/*driver incluede*/
#include "includes.h"
/*硬件启动函数组
左边为初始化函数，右边为初始化时显示的字符
*/
int UsartQueueSendInit(void);
const SetupHardwareType SetupHardwareInit[] =
{
    BlueToothInit,  " Init BlueTooth      ",
    ReservedIOInit, " Init Keys           ",
//    CanInit,        " Init CANs           ",
    StartupInfo,    "                     ",
    MenuInit,       "                     ",
};



int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,
                        ENABLE);      /*使能SWD 禁用JTAG*/
    /*System startup task*/
    xTaskCreate( prvOSStatInit, "OSStatInit", configMINIMAL_STACK_SIZE + 100, NULL,
                 STAT_TASK_PRIO + 1, NULL);
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    for ( ;; );
}
/*系统初始化任务*/
void  prvOSStatInit( void *pvParameters )
{
    for (;;)
    {
        prvSetupHardware();
        /*
                创建系统所有任务
                用户自定义任务放在这里建立
        */
        xTaskCreate(OS_TaskStat, "OS_TaskStat", 256,  NULL,
                    STAT_TASK_PRIO, NULL);//创建统计任务，优先级(1)仅高于空闲任务
        xTaskCreate( prvCoreTask,  "Core",  64, NULL, CORE_TASK_PRIORITY, NULL );
        xTaskCreate( prvUITask,    "UI",    256, NULL, UI_TASK_PRIORITY,   NULL );
        xTaskCreate( prvStatusTask, "Status", 64, NULL, STATUS_TASK_PRIORITY, NULL );

        vTaskDelete(NULL);
    }
}

/*核心任务*/
u32 CoreTaskCounter;
float t;
void prvCoreTask( void *pvParameters )
{
    TickType_t xLastWakeTime;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();

    for ( ;; )
    {
        vTaskDelayUntil( &xLastWakeTime, TickByms(200));
        CoreTaskCounter++;


        taskENTER_CRITICAL();
        Send_Data(&ConnectQuest);
        taskEXIT_CRITICAL();
        switch (CtlMode)
        {
            case MoveOut:

                break;
            case MoveIn:

                break;
            case RunForrestRun:

                break;
            case StopForrestStop:

                break;
        }
        if (ConnectingFlag)
            if (ConnectTimeLast + TickByms(1000) < xTaskGetTickCount())
            {
                ConnectingFlag = 0;
            }
    }

//    vTaskDelete( NULL );
}




///*串口发送任务*/
//u32 UsartCounter;
//void prvUartTask( void *pvParameters )
//{
//    TickType_t xLastWakeTime;
//    // Initialise the xLastWakeTime variable with the current time.
//    xLastWakeTime = xTaskGetTickCount ();

//    for ( ;; )
//    {
//        vTaskDelayUntil( &xLastWakeTime, TickByms(20));
//        UsartCounter++;

//    }

////    vTaskDelete( NULL );
//}

/*
状态显示任务优先级最低，运行正常说明其他任务无阻塞
LED:
    一秒两次正常运行
    ***快闪进入Hard Fault***
    自定义其他状态,可用状态机等显示多种状态
蜂鸣器：
    自定义状态吧，只有自己听得懂哔哔哔是不是很牛逼
*/
u32 StatusCounter;
void prvStatusTask( void *pvParameters )
{
    TickType_t xLastWakeTime;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    u32 TaskDelay = 100;
    CoreLed(red, off);
    for ( ;; )
    {
        vTaskDelayUntil( &xLastWakeTime, TickByms(TaskDelay));
        StatusCounter++;
        if ((u32)ErrorStatusFlag > (u32)NoSignal)
        {
            CoreLed(yellow, on);
            CoreLed(blue, off);
        }
        if (ConnectingFlag)
        {
            CoreLed(yellow, off);
            CoreLed(blue, on);
        }
        else
        {
            CoreLed(yellow, off);
            CoreLed(blue, toggle);
        }


    }

//    vTaskDelete( NULL );
}

/* A tick hook is used by the "Full" build configuration.  The simple build
configuration also has to define a tick hook - even though it does not actually
use it for anything. */
void vApplicationTickHook( void )
{
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    taskDISABLE_INTERRUPTS();
    u32 i;
    while (1)
    {
        CoreLed(red, on);
        i = 0xffffff;
        while (i--);

        CoreLed(red, off);
        i = 0xcfffff;
        while (i--);


        CoreLed(red, on);
        i = 0x1fffff;
        while (i--);

        CoreLed(red, off);
        i = 0x3fffff;
        while (i--);

    }
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    u32 i;
    while (1)/*一长两短*/
    {
        CoreLed(red, on);
        i = 0xffffff;
        while (i--);

        CoreLed(red, off);
        i = 0xcfffff;
        while (i--);


        CoreLed(red, on);
        i = 0x1fffff;
        while (i--);

        CoreLed(red, off);
        i = 0x7fffff;
        while (i--);


        CoreLed(red, on);
        i = 0x1fffff;
        while (i--);

        CoreLed(red, off);
        i = 0x3ffffff;
        while (i--);


    }
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


/* The Blinky build configuration does not include run time stats gathering,
however, the Full and Blinky build configurations share a FreeRTOSConfig.h
file.  Therefore, dummy run time stats functions need to be defined to keep the
linker happy. */
void vMainConfigureTimerForRunTimeStats( void ) {}
//unsigned long ulMainGetRunTimeCounterValue( void ) { return 0UL; }


void HardFault_Handler(void)
{

    uint32_t i = 0xffff;
    while (1)
    {
        CoreLed(red, on);
        while (i--);
        i = 0x1fffff;
        CoreLed(red, off);
        while (i--);
        i = 0x1fffff;
    }

}

void prvSetupHardware( void )
{
    CoreLedInit();    /*CoreLedInit,    " Init Status LED     ",*/
    UIInit();
    taskENTER_CRITICAL();                        //关中断
    OSIdleCtr =
        0;                      // Clear idle counter                                 */
    taskEXIT_CRITICAL();
    vTaskDelay(
        CULCULATE_PERIOD);      /* Determine MAX. idle counter value for CULCULATE_PERIOD ms ,延时 */
    taskENTER_CRITICAL();
    OSIdleCtrMax =
        OSIdleCtr;      /* Store maximum idle counter count in CULCULATE_PERIOD ms，获取CULCULATE_PERIODms内OSIdleCtrMax加到的最大值    */
    taskEXIT_CRITICAL();         //开中断
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    OLED_CLS();
    OLED_Print(OLED_INVERSE_OFF, 16, 0, (u8*)BootingDisTemp);/*傻逼启动中*/
    AddMessage(" System Booting...");
    /*Booting...*/
    u32 i;
    for (i = 0; i < length(SetupHardwareInit); i++)
    {
        SetupDD(SetupHardwareInit[i]);
    }
}


