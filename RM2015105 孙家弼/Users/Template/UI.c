/**
  ******************************************************************************
  * @file    UI.c
  * @author  Johnny Sun
  * @version V2.0
  * @date    2015.4.21
  * @note    C file of UI
  * @History    V1.0 2015.2.5
  *                 Basic Function:Menu shift & parameters change.
  *             V1.1 2015.2.18
  *                 Add function running support.
  *             V1.2 2015.3.27
  *                 Refine HardwareInit & other custom type
  *                 Modify to STM32F4 edition.
  *             V1.3 2015.4.21
  *                 LEDs Compatible with other mainboard
  *             V2.0 2015.7.5
  *                 Unite int&float type number into the same menu
  *                 Add number arrangment
  ******************************************************************************
  */
/*
对外函数:

常用
    清屏
        void OLED_CLS(void);
    初始化
        void OLED_Init(void);
    标准化打印，Inverse控制反相显示，x坐标（0-128）y坐标（0-7）,ch字符串（0-21）
        void Standard_Printf(unsigned char inverse,
                        unsigned char x,unsigned char y,const char *fmt,...);
可用
    打印一个字符
        void OLED_Print(uint8_t Inverse,uint8_t x, uint8_t y, uint8_t ch[]);
    刷屏显示一张128*64的图
        void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
    设置零点坐标
        void OLED_Set_Pos(uint8_t x, uint8_t y);
    使一个字符串循环平移
        void Scroll_Showing(uint8_t *temp);
*/


#include "UI.h"
#include "bsp.h"
//#include <string.h>


#define KEY_ON_BOARD
u32 KeyUpCounter, KeyDownCounter, KeyLeftCounter, KeyRightCounter;
static char display_cache[MESSAGE_QUEUE_LENGTH][128] = {""};
const uint8_t BootingDisTemp[] = {"傻逼启动中。。。"};
const uint8_t LogoBmp128x64[] =
{

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0xF0,
    0x78, 0x78, 0x98, 0x1C, 0xFC, 0xFC, 0xFC, 0xFC,
    0xFC, 0xFC, 0xFC, 0xFC, 0x3C, 0x1C, 0x38, 0xB8,
    0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFC,
    0xEE, 0xEF, 0xDF, 0x8F, 0x7F, 0x3F, 0x1F, 0x0F,
    0x07, 0x86, 0x43, 0x43, 0x41, 0xC1, 0x81, 0x81,
    0x01, 0x01, 0x81, 0x81, 0xC3, 0x43, 0x42, 0x47,
    0x87, 0x0F, 0x1F, 0x3F, 0xFF, 0xDF, 0xDF, 0xCF,
    0xFC, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x1C, 0xDC, 0xFC, 0xFC,
    0x3C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8,
    0xF8, 0x38, 0x00, 0x00, 0x00, 0x18, 0x1C, 0xCC,
    0xFC, 0xFC, 0x7C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x00, 0x00, 0xF0, 0xF8, 0xF8, 0x9C,
    0x0C, 0x0C, 0x0C, 0x1C, 0x1C, 0x00, 0x00, 0x38,
    0x3C, 0x1C, 0x1C, 0x9C, 0xFC, 0xFC, 0xFC, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1E, 0x0F, 0xC7, 0xE0, 0xF0,
    0x78, 0x38, 0x1C, 0x0C, 0x0C, 0x0C, 0x1C, 0x1C,
    0x3C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0xFF, 0xBF, 0x3F, 0x3F,
    0x7F, 0xFF, 0x1F, 0x01, 0x00, 0xE0, 0x80, 0xE0,
    0xE7, 0x40, 0x30, 0x10, 0xD0, 0x98, 0xEC, 0x07,
    0x63, 0xA3, 0x05, 0x0C, 0x38, 0xE0, 0x20, 0xC0,
    0xEC, 0x27, 0xA0, 0x00, 0x00, 0x03, 0xFF, 0xBF,
    0x3F, 0x9F, 0x9F, 0xFF, 0xFE, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7E, 0xFF, 0xFF, 0xE1,
    0xC0, 0xC0, 0xE0, 0xF0, 0x78, 0xFE, 0xFF, 0xFF,
    0x83, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFB, 0xFF,
    0xFF, 0xC7, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xE3,
    0x63, 0xE0, 0xE0, 0x80, 0x01, 0x03, 0x07, 0x0F,
    0x9F, 0xFE, 0xFC, 0x78, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xF1,
    0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xF0,
    0x70, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x3F, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xF0, 0xC0, 0x00, 0x01, 0x01,
    0x3C, 0x4E, 0x81, 0x83, 0xC2, 0x47, 0x6C, 0x68,
    0x31, 0x39, 0x78, 0x4C, 0x46, 0x81, 0x80, 0x00,
    0x05, 0x19, 0x01, 0x80, 0xC0, 0xF0, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
    0xC1, 0x41, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0xC1, 0x01, 0x00,
    0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0xC1, 0x81, 0x00, 0x00, 0x00, 0x00, 0x80,
    0xC0, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07,
    0x0F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFE, 0xFC,
    0xFC, 0xF8, 0xF8, 0xF0, 0x70, 0xE0, 0xE0, 0x60,
    0xE0, 0xE0, 0x60, 0xF0, 0x70, 0x70, 0xF8, 0xF8,
    0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F,
    0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x04, 0x04, 0x0A, 0x31, 0x00, 0x00, 0x3E,
    0x21, 0x21, 0x22, 0x1E, 0x00, 0x3F, 0x22, 0x21,
    0x22, 0x1E, 0x00, 0x3E, 0x21, 0x21, 0x22, 0x1E,
    0x00, 0x3F, 0x01, 0x0E, 0x30, 0x18, 0x06, 0x01,
    0x3F, 0x00, 0x10, 0x2A, 0x29, 0x29, 0x3E, 0x00,
    0x00, 0x26, 0x29, 0x39, 0x00, 0x01, 0x3F, 0x21,
    0x00, 0x00, 0x3E, 0x29, 0x29, 0x26, 0x00, 0x00,
    0x3F, 0x02, 0x01, 0x00, 0x26, 0x29, 0x39, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03,
    0x03, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x0E, 0x0F,
    0x0F, 0x0E, 0x0F, 0x0F, 0x06, 0x07, 0x07, 0x07,
    0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
/*Check parameters differed from which in EEPROM & Store them
*/
void ProgSave()
{
    taskENTER_CRITICAL();
    Send_Data(&SaveCommand);
    taskEXIT_CRITICAL();
}

/**/
void ProgStatus()
{
   
}
void ProgReset()
{
    taskENTER_CRITICAL();
    Send_Data(&ResetCommand);
    taskEXIT_CRITICAL();
}
/*List for Programs used in Menu*/
MenuProgType MenuProg[] =
{
    {0x0012, ProgStatus},
    {0x0119, ProgSave},
    {0x0118, ProgReset}
};


MenuStatusType MenuStatus = ItemChanging;

MenuType*  CurrentMenu;
/*自定义菜单*/
MenuType  Menu[] =
{
    /*main menu*/
    {
        0x0001, {" 1.Set Param", " 2.Control", " 3.About"},
        3, 0, 0, NULL, {NULL,   NULL,    NULL,    NULL}
    },
    /*Param*/
    {
        0x0011, {" 1.Encoder Direct", " 2.CAN ID", " 3.Motor Direct",  " 4.Set Speed",
            "5.Over Current","6.Extreme_Current","7.Recovery Current"," 8.Reset Driver", " *Save Changes*"
        },
        9, 0, 0, Menu, {&EncoderDirect, &CanID, &MotorDirect, &SetSpeed,
                 &Over_Current_Value,&Extreme_Current_Value,&Recovery_Current_Value}
    },
    /*"Control"*/
    {
        0x0012, {" coming sooooon"},
        1, 0, 0, Menu, {NULL}
    },
    /*"About"*/
    {
        0x0013, {"Motor Driver Setup", FIRMWARE_VERSION,"  --Johnny Sun @UESTC", "    Copyright @2015"},
        4, 0, 0, Menu, {NULL}
    },

};

//Menu display task（button scan）
u32 UI_Counter;
void prvUITask( void *pvParameters )
{
    TickType_t xLastWakeTime;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
//    SemaphoreHandle_t smStatusDisplaying;
//    smStatusDisplaying = xSemaphoreCreateMutex();
    for ( ;; )
    {
        u32 j = 0;
        UI_Counter++;
        /*Prevent OLED Crash*/
        if (UI_Counter % 30 == 0)
        {
            OLED_Boot();
            //OLED_CLS();
            MenuDisplay();
        }
        //Output parameters with no relate with interactive experience
        if(ConnectingFlag)
        {
            Standard_Printf(OLED_INVERSE_OFF, 0, 0, "Status:%12s",StatusText[ErrorStatusFlag]);
            Standard_Printf(OLED_INVERSE_OFF, 0, 1, "Current:%10.3f",Current.Num.float_form);
        }
        else
        {
            Standard_Printf(OLED_INVERSE_OFF, 0, 0, "Please Connect Driver");
        }
            //Standard_Printf(OLED_INVERSE_OFF, 0, 1, "Speed:%d",&);
        //      PAout(17) = !PAout(17);
        //Standard_Printf(OLED_INVERSE_ON, 80, 7, "%lds", xLastWakeTime / configTICK_RATE_HZ);

        //交互优先
        while (j < 10)
        {
            vTaskDelayUntil( &xLastWakeTime, TickByms(33) );
#if defined KEY_ON_BOARD
            KeyScan();
#endif
            MenuOperationScan();
            j++;
        }
        if (((CurrentMenu->value[CurrentMenu->line]) == NULL)
            && (CurrentMenu->value[CurrentMenu->cur]) == NULL)
        {
//                Standard_Printf(OLED_INVERSE_OFF, 78, 2, "P:%6.2f", Pitch);
//                Standard_Printf(OLED_INVERSE_OFF, 78, 3, "R:%6.2f", Roll);
//                Standard_Printf(OLED_INVERSE_OFF, 78, 4, "Y:%6.2f", Yaw);
        }
        j = 0;
    }

//    vTaskDelete( NULL );
}



/*
Run a configure program. Display & Delay it on OLED.
*/
int32_t SetupDD(SetupHardwareType SetupHardwareInit)
{
    uint32_t t;
    uint32_t result;

    AddMessage(SetupHardwareInit.text);

    s32 retry_counter;
    retry_counter = 3;
    while (retry_counter--)
    {
        result = SetupHardwareInit.pro();
        t = INITIALIZATION_DELAY;
        while (t--);

        if (result)
        {
            return result;
        }
        else/*失败再次初始化*/
        {
            if (retry_counter == 4) AddMessage(" Try again...      ");
            t = INITIALIZATION_DELAY;
            while (t--);
        }
    }
    /*跑到这说明初始化失败*/
    AddMessage(" Failed!             ");
    while (result != 1)
    {
        CoreLed(red, on);
        vTaskDelay(TickByms(50));
        CoreLed(red, off);
        vTaskDelay(TickByms(600));
    }
    return result;
}

#include "semphr.h"
/*Searching for program according to ID.
    return: i of MenuProg if one exists
    otherwise return NULL
*/
u32 NextLevelProg(MenuType*  CurrentMenu)
{
    u32 i;
    u16 target_menu_id;
    i = 0;
    target_menu_id = (CurrentMenu->ID << 4) + CurrentMenu->cur  + 1;
    while ((MenuProg[i].ID != target_menu_id)
           && (i < NUM_OF_MENU_ARRAY)) /*While ID doesn't match*/
    {
        i++;
    }
    if (i < NUM_OF_PROG_ARRAY)/*Program exists*/
    {
        return (i);
    }
    else
    {
        return NULL;
    }
}

void UIInit(void)
{
    OLED_Init();
    OLED_CLS();
    Draw_BMP(0, 0, 128, 7, (u8*)LogoBmp128x64);
//    OLED_Print(OLED_INVERSE_OFF, 32, 2, LogoDisTemp);
//    u32 t = 0x4ffffff;
//    while (t--);
//    OLED_CLS();
//    OLED_Print(OLED_INVERSE_OFF, 16, 0, (u8*)BootingDisTemp);
#if defined KEY_ON_BOARD
    KeyInit();
#endif
    CoreLed(red, on);
}

#if defined KEY_ON_BOARD
extern BinIOType ReservedStruct[];

void KeyScan(void)
{
    if     (ReadReservedIO(UpKey))KeyUpCounter++;
    else if (ReadReservedIO(LeftKey))KeyDownCounter++;
    else if (ReadReservedIO(DownKey))KeyLeftCounter++;
    else if (ReadReservedIO(RightKey))KeyRightCounter++;
}
void KeyInit(void)
{
//    u32 i;
//    for (i = 0; i < length(ReservedStruct); i++)
//    {
//        MyGPIOInit(ReservedStruct[i].io, ReservedStruct[i].pin,
//                   GPIO_Mode_IPD, GPIO_Speed_2MHz);
//        io_init_delay();
//    }
}
#endif
s32 MenuInit(void)
{


    CurrentMenu = Menu;
    Standard_Printf(OLED_INVERSE_OFF, 0, 7, "Any Key to continue..");
//    do
//    {
//       KeyScan();
//    }
//    while (!KEY_UP && !KEY_DOWN &&!KEY_LEFT && !KEY_RIGHT);
    OLED_CLS();
    MenuDisplay();
    return 1;
}

void MenuDisplay(void)
{
    u32 i;
    i = 0;

    //current item beyond first screen by the bottom
    if (CurrentMenu->cur > CurrentMenu->line + MENU_DIS_NUM_MAX - 1)
    {
        CurrentMenu->line = CurrentMenu->cur - MENU_DIS_NUM_MAX + 1;
    }
    //current item beyond first screen by the top
    else if (CurrentMenu->cur < CurrentMenu->line )
    {
        CurrentMenu->line = CurrentMenu->cur;
    }
    menu_cls();
    while ((i + CurrentMenu->line) < CurrentMenu->total && i < MENU_DIS_NUM_MAX)
    {
        if ((i + CurrentMenu->line) == CurrentMenu->cur)
        {
            //Highlight display
            Standard_Printf(OLED_INVERSE_ON, 1, i + 2, CurrentMenu->text[CurrentMenu->cur]);
            if ((CurrentMenu->value[i + CurrentMenu->line]) != NULL) //oooh~~value exist!!!
            {
                if (MenuStatus == ValueChanging)
                {
                    if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                        VALUE_TYPE_FLOAT)/*adjust float parameters*/
                    {
                        Standard_Printf(OLED_INVERSE_ON, 90, i + 2, "%5.1f",
                                        (CurrentMenu->value[i + CurrentMenu->line])->Num.float_form);
                    }
                    else if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                             VALUE_TYPE_INTEGER)/*adjust int parameters*/
                    {
                        Standard_Printf(OLED_INVERSE_ON, 90, i + 2, "%6d",
                                        (CurrentMenu->value[i + CurrentMenu->line])->Num.int_form);
                    }
                }
                else
                {
                    if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                        VALUE_TYPE_FLOAT) /*adjust float parameters*/
                    {
                        Standard_Printf(OLED_INVERSE_OFF, 90, i + 2, "%5.1f",
                                        (CurrentMenu->value[i + CurrentMenu->line])->Num.float_form);
                    }
                    else if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                             VALUE_TYPE_INTEGER)/*adjust int parameters*/
                    {
                        Standard_Printf(OLED_INVERSE_OFF, 90, i + 2, "%6d",
                                        (CurrentMenu->value[i + CurrentMenu->line])->Num.int_form);
                    }
                }
            }
        }
        else
        {
            Standard_Printf(OLED_INVERSE_OFF, 1, i + 2,
                            CurrentMenu->text[i + CurrentMenu->line]);
            if ((CurrentMenu->value[i + CurrentMenu->line]) != NULL) //oooh~~value exist!!!
            {
                if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                    VALUE_TYPE_FLOAT)/*adjust float parameters*/
                {
                    Standard_Printf(OLED_INVERSE_OFF, 90, i + 2, "%5.1f",
                                    (CurrentMenu->value[i + CurrentMenu->line])->Num.float_form);
                }
                else if ((CurrentMenu->value[i + CurrentMenu->line])->ValueType ==
                         VALUE_TYPE_INTEGER)/*adjust int parameters*/
                {
                    Standard_Printf(OLED_INVERSE_OFF, 90, i + 2, "%6d",
                                    (CurrentMenu->value[i + CurrentMenu->line])->Num.int_form);
                }

            }
        }
        i++;
    }



    /*old
    OLED_CLS();
    while(CuCurrent_Menu->next != NULL)//当前菜单未显示完
    {
        Standard_Printf(OLED_INVERSE_OFF,1,Current_Menu->ID+2,Current_Menu->text);
        if (Current_Menu->children==NULL)//末菜单（可调参）
        {
            if(Current_Menu->value == VALUE_TYPE_INTEGER)
            {
                Standard_Printf(OLED_INVERSE_OFF,70,(u8)(Current_Menu->ID+2),
                                "%d",(Current_Menu->value));
            }
            else
            {
                Standard_Printf(OLED_INVERSE_OFF,70,(u8)(Current_Menu->ID+2),
                                "%f",Current_Menu->value);
            }
        }
    }
    */
}


void menu_cls(void)
{
    u32 i;
    for (i = 0; i < MENU_DIS_NUM_MAX; i++)
    {
        Standard_Printf(OLED_INVERSE_OFF, 0, i + 2, "%21s", " ");
    }
}

//Find next level menu
MenuType* NextLevelMenu(MenuType*  CurrentMenu)
{
    u32 i;
    u16 target_menu_id;
    i = 0;
    target_menu_id = (CurrentMenu->ID << 4) + CurrentMenu->cur  + 1;
    while ((Menu[i].ID != target_menu_id) && (i < NUM_OF_MENU_ARRAY))
    {
        i++;//find next level
    }

    if (i < NUM_OF_MENU_ARRAY)
    {
        return (Menu + i);
    }
    else
    {
        return CurrentMenu;
    }
}

void MenuOperationScan(void)
{
    if (KEY_UP)
    {
        switch (MenuStatus)
        {
            case (ItemFix):
                break;
            case (ItemChanging):
                if (CurrentMenu->cur > 0) CurrentMenu->cur--; //shift item
                else    CurrentMenu->cur = CurrentMenu->total - 1;
                break;
            case (ValueChanging):
                if (CurrentMenu->value[CurrentMenu->cur]->ValueType ==
                    VALUE_TYPE_FLOAT) /*adjust float parameters*/
                {
                    CurrentMenu->value[CurrentMenu->cur]->Num.float_form += 0.1;
                }
                else if (CurrentMenu->value[CurrentMenu->cur]->ValueType ==
                         VALUE_TYPE_INTEGER) /*adjust int parameters*/
                {
                    CurrentMenu->value[CurrentMenu->cur]->Num.int_form++;
                }
                ValueArrangeLimit(CurrentMenu->value[CurrentMenu->cur]);/*limit arrange*/
                Send_Data(CurrentMenu->value[CurrentMenu->cur]);
                break;
        }
        MenuDisplay();/*refresh menu*/
    }
    else if (KEY_DOWN)
    {
        switch (MenuStatus)
        {
            case (ItemFix):
                break;
            case (ItemChanging):
                if (CurrentMenu->cur < CurrentMenu->total - 1) CurrentMenu->cur++; //shift item
                else  CurrentMenu->cur = 0;
                break;
            case (ValueChanging):
                if (CurrentMenu->value[CurrentMenu->cur]->ValueType ==
                    VALUE_TYPE_FLOAT) /*adjust float parameters*/
                {
                    CurrentMenu->value[CurrentMenu->cur]->Num.float_form -= 0.1;
                }
                else if (CurrentMenu->value[CurrentMenu->cur]->ValueType ==
                         VALUE_TYPE_INTEGER) /*adjust int parameters*/
                {
                    CurrentMenu->value[CurrentMenu->cur]->Num.int_form--;
                }
                ValueArrangeLimit(CurrentMenu->value[CurrentMenu->cur]);/*limit arrange*/
                Send_Data(CurrentMenu->value[CurrentMenu->cur]);
                break;
        }
        MenuDisplay();
    }
    else if (KEY_LEFT)
    {
        switch (MenuStatus)
        {
            case (ItemFix):
            case (ItemChanging):
                if (CurrentMenu->father != NULL) CurrentMenu =
                        CurrentMenu->father; //back to prelevel
                break;
            case (ValueChanging):
                MenuStatus = ItemChanging;
                break;
        }
        MenuDisplay();
    }
    else if (KEY_RIGHT)
    {
        switch (MenuStatus)
        {
            case (ItemFix):
            case (ItemChanging):
                if ((CurrentMenu->value[CurrentMenu->cur]) != NULL)//menu has value
                {
                    MenuStatus = ValueChanging;
                }
                else
                {
                    CurrentMenu = NextLevelMenu(CurrentMenu);
                }
                /*Run Program*/
                MenuProg[NextLevelProg(CurrentMenu)].pro();
                break;
            case (ValueChanging):
                break;
        }

        MenuDisplay();
    }

    while (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT )
    {
        KeyUpCounter = 0;
        KeyDownCounter = 0;
        KeyLeftCounter = 0;
        KeyRightCounter = 0;
        vTaskDelay(2000);
    }

}

u32 Message_Queue_Head = 0, Message_Queue_Tail = 0;
void AddMessage(uint8_t ch[])
{
    assert_param(length(ch) < 22);
    u32 i = 0;
    if (++Message_Queue_Tail > MESSAGE_QUEUE_LENGTH - 1) Message_Queue_Tail = 0;
    while (ch[i] != '\0')
    {
        display_cache[Message_Queue_Tail][i] = ch[i];
        i++;
    }

    if (Message_Queue_Head == Message_Queue_Tail)
    {
        Message_Queue_Head++;
        if (Message_Queue_Head > MESSAGE_QUEUE_LENGTH - 1) Message_Queue_Head = 0;
    }
    print_message();
}

void print_message()
{
    u32 i, y;
    y = 2;
    i = Message_Queue_Head;

    while (i != Message_Queue_Tail)
    {
        if (i++ > MESSAGE_QUEUE_LENGTH - 1) i = 0;
        if (display_cache[i][0] != '\0')
        {
            Standard_Printf(OLED_INVERSE_OFF, 0, y, display_cache[i]);
            y++;
        }
    }

}
void ValueArrangeLimit(ValueStruct* Value)
{
    if (Value->ValueType==VALUE_TYPE_FLOAT)
    {
        if(Value->Num.float_form > Value->MaxValue.float_form) Value->Num=Value->MaxValue;
        else if(Value->Num.float_form < Value->MinValue.float_form) Value->Num=Value->MinValue;
    }
    else
    {
        if(Value->Num.int_form > Value->MaxValue.float_form) Value->Num.int_form=Value->MaxValue.float_form;
        else if(Value->Num.int_form < Value->MinValue.float_form) Value->Num.int_form=Value->MinValue.float_form;
    }
}
//        xQueueSendToBack( xPrintQueue, &( pcStringsToPrint[ iIndexToString ] ), 0 );

