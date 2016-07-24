/**
  ******************************************************************************
  * @file    UI.h
  * @author  Johnny Sun
  * @version V1.0
  * @date    2015.2.5
  * @note    H file of UI
  ******************************************************************************
  */


#ifndef __UI_H
#define __UI_H
#include "OLED.h"
#include "includes.h"
#include "bsp.h"
/*startup list info*/
#define MESSAGE_QUEUE_LENGTH    6
#define INITIALIZATION_DELAY    0xfff /*display delay number*/
/* Defines for the UI Lable */


/*
******************�˵��ṹ*******************
�˵��������Ϸ������ӣ� �·������٣� ǰ�� ����
�������� �������� ���浱ǰ
���Բ���Para_Deb��Bal_PD,Spe_PI,Pos_PD��
��Ӧ����Para_Env��CCD_Thr,Spe_Max,Spe_C1,Spe_C2,Spe_Min
����
*********************************************
*/

#define MENU_ITEM_MAX                   10
#define NUM_OF_MENU_ARRAY               (sizeof(Menu) / sizeof(Menu[0]))
#define MENU_DIS_NUM_MAX                6
#define NUM_OF_PROG_ARRAY               (sizeof(MenuProg) / sizeof(MenuProg[0]))

/*Menu chosen status lable*/
typedef enum    
{
    ItemChanging,
    ItemFix,
    ValueChanging,
}MenuStatusType;


typedef struct
{
    s32 (*pro)();
    u8 text[21];
}SetupHardwareType;   


typedef struct  MenuType
{
    u16 ID;//�˵�ֵ
    const char* text[MENU_ITEM_MAX];   //�˵���ʾ����

    const u8 total;//��ǰ��˵���
    u8 cur;   //ѡ����
    u8 line;   //��һ��ʾ��
    struct  MenuType* father;   //���˵�
    ValueStruct* value[MENU_ITEM_MAX];//�˵�ֵ��ַ
} MenuType;

/*define program list idem*/
typedef struct MenuProgType
{
    u32 ID;
    void (*pro)();
} MenuProgType;


/*GPIOs interface*/
/*A��B��Cȡ��D����*/
/*�ɰ�����
#define KEY1_GPIO                    GPIOC
#define KEY2_GPIO                    GPIOA

#define KEY1_Pin                GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define KEY2_Pin                GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3

#define KEY1_H      KEY1_GPIO->ODR |= KEY1_Pin
#define KEY1_L      KEY1_GPIO->ODR &= ~KEY1_Pin
#define KEY2_H      KEY2_GPIO->ODR |= KEY2_Pin
#define KEY2_L      KEY2_GPIO->ODR &= ~KEY2_Pin
*/
#define KEY_UP      KeyUpCounter
#define KEY_DOWN    KeyDownCounter
#define KEY_LEFT    KeyLeftCounter
#define KEY_RIGHT   KeyRightCounter

extern u32 KeyUpCounter,KeyDownCounter,KeyLeftCounter,KeyRightCounter;
extern MenuType* CurrentMenu;
const extern uint8_t BootingDisTemp[];
//const extern uint8_t LogoDisTemp[];

void LED(u32 num, StatusType on);
void UIInit(void);
s32 MenuInit(void);
void MenuDisplay(void);
void AddMessage(uint8_t ch[]);
void print_message(void);
void KeyInit(void);
void KeyScan(void);
void LEDInit(void);
void MenuOperationScan(void);
int32_t SetupDD(SetupHardwareType SetupHardwareInit);
void ValueArrangeLimit(ValueStruct* Value);
u32 NextLevelProg(MenuType*  CurrentMenu);
MenuType* NextLevelMenu(MenuType*  CurrentMenu);

void menu_cls(void);

void prvUITask( void *pvParameters );
#endif  //__UI_H

/* End of "UI.h" */
