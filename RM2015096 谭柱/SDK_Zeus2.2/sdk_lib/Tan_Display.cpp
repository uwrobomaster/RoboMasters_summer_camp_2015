#include <stdio.h>

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "Logic_CV_Thread.h"
#include "Tan_Display.h"
#include "DJI_Control.h"
#include "DJI_Pro_Test.h"
#include "CV_header.h"
#include "serial.h"

extern PID_Param LW_PID;
extern PID_Param Height_PID;
extern PID_Param YAW_PID;
using namespace std ;
char START_FLAG = 0;
char CMD_FLAG = 0;
int value=0;
void Set_LW_PID_Param(void)
{
    PID_Param temp_PID;
    char temp_flag ='n';

    printf("\r\n");
    printf("-----------   < Set LW_PID Param >  ----------\r\n\r\n");

    printf("Current Param:\n");
    printf("Line Waling_PID_P = %f\n",LW_PID.P);
    printf("Line Waling_PID_I = %f\n",LW_PID.I);
    printf("Line Waling_PID_D = %f\n\n",LW_PID.D);

    printf("Input Line Walking P =");
    scanf("%f",&temp_PID.P);
    printf("Input Line Walking I =");
     scanf("%f",&temp_PID.I);
    printf("Input Line Walking D =");
     scanf("%f",&temp_PID.D);
    printf("sure???[y= yes/n =no]");
    std::cin>>temp_flag;

    if(temp_flag == 'y')
    {

        LW_PID.P = temp_PID.P;
        LW_PID.I = temp_PID.I;
        LW_PID.D = temp_PID.D;
        printf("\033[2J");
        printf("-----------------\n");
        printf("Set LW_PID Finish!\n");
        printf("Current Param:\n");
        printf("Line Waling_PID_P = %f\n",LW_PID.P);
        printf("Line Waling_PID_I = %f\n",LW_PID.I);
        printf("Line Waling_PID_D = %f\n",LW_PID.D);
        printf("-----------------\n");

    }
    else if(temp_flag == 'n')
    {
        printf("\033[2J");
        printf("-----------------\n");
        printf("Didn't save param!\n");
        printf("-----------------\n");


    }

    Display_Menu();
}

void Set_Height_PID_Param(void)
{
    PID_Param temp_PID;
    char temp_flag ='n';
    printf("\r\n");
    printf("-----------   < Set Height PID Param >  ----------\r\n\r\n");

    printf("Current Param:\n");
    printf("Height_PID_P = %f\n",Height_PID.P);
    printf("Height_PID_I = %f\n",Height_PID.I);
    printf("Height_PID_D = %f\n\n",Height_PID.D);

    printf("Input Height P =");
    scanf("%f",&temp_PID.P);
    printf("Input Height I =");
    scanf("%f",&temp_PID.I);
    printf("Input Height D =");
    scanf("%f",&temp_PID.D);
    printf("sure???[y= yes/n =no]");
    std::cin>>temp_flag;

    if(temp_flag == 'y')
    {

        Height_PID.P = temp_PID.P;
        Height_PID.I = temp_PID.I;
        Height_PID.D = temp_PID.D;
        printf("\033[2J");
        printf("---------------------\n");
        printf("Set Height PID Finish!\n");
        printf("Current Param:\n");
        printf("Height_PID_P = %f\n",Height_PID.P);
        printf("Height_PID_I = %f\n",Height_PID.I);
        printf("Height_PID_D = %f\n",Height_PID.D);
        printf("----------------------\n");

    }
    else if(temp_flag == 'n')
    {
        printf("\033[2J");
        printf("-----------------\n");
        printf("Didn't save param!\n");
        printf("-----------------\n");


    }

    Display_Menu();
}

void Set_YAW_PID_Param(void)
{
    PID_Param temp_PID;
    char temp_flag ='n';
    printf("\r\n");
    printf("-----------   < Set YAW PID Param >  ----------\r\n\r\n");

    printf("Current Param:\n");
    printf("YAW_PID_P = %f\n",YAW_PID.P);
    printf("YAW_PID_I = %f\n",YAW_PID.I);
    printf("YAW_PID_D = %f\n\n",YAW_PID.D);

    printf("Input YAW P =");
    scanf("%f",&temp_PID.P);
    printf("Input YAW I =");
    scanf("%f",&temp_PID.I);
    printf("Input YAW D =");
    scanf("%f",&temp_PID.D);
    printf("sure???[y= yes/n =no]");
    std::cin>>temp_flag;

    if(temp_flag == 'y')
    {

        YAW_PID.P = temp_PID.P;
        YAW_PID.I = temp_PID.I;
        YAW_PID.D = temp_PID.D;
        printf("\033[2J");
        printf("---------------------\n");
        printf("Set YAW PID Finish!\n");
        printf("Current Param:\n");
        printf("YAW_PID_P = %f\n",YAW_PID.P);
        printf("YAW_PID_I = %f\n",YAW_PID.I);
        printf("YAW_PID_D = %f\n",YAW_PID.D);
        printf("----------------------\n");

    }
    else if(temp_flag == 'n')
    {
        printf("\033[2J");
        printf("-----------------\n");
        printf("Didn't save param!\n");
        printf("-----------------\n");


    }

    Display_Menu();
}

extern float Height;
void Set_Height_Param(void)
{
    float temp_Height;
    char temp_flag ='n';
    printf("\r\n");
    printf("-----------   < Set YAW PID Param >  ----------\r\n\r\n");

    printf("Current Param:\n");
    printf("Height = %f\n",Height);


    printf("Height =");
    scanf("%f",&temp_Height);
    printf("sure???[y= yes/n =no]");
    std::cin>>temp_flag;

    if(temp_flag == 'y')
    {

        Height = temp_Height;

        printf("\033[2J");
        printf("---------------------\n");
        printf("Set Height Finish!\n");
        printf("Current Param:\n");
        printf("Height = %f\n",Height);
        printf("----------------------\n");

    }
    else if(temp_flag == 'n')
    {
        printf("\033[2J");
        printf("-----------------\n");
        printf("Didn't save param!\n");
        printf("-----------------\n");


    }

    Display_Menu();
}

void Display_Menu(void)
{
    printf("\r\n");
    printf("-----------   < Main menu >  ----------\r\n\r\n");

    printf("[a]  Start Fly\n");
    printf("[b]  Set Line Walking PID Param\n");
    printf("[c]  Set Height PID Param\n");
    printf("[d]  Set YAW PID Param\n");
    printf("[e]  The UAV Current Status\n");
    printf("[f]  Landing\n");
    printf("[g]  Set the Hight\n");
    printf("[q]  Release the Camera\n");

    printf("\ninput a/b/c etc..then press enter key\r\n");
    printf("---------------------------------------\r\n");
    printf("input: ");


}

void Choose_Menu(void)
{

    CMD_FLAG = getchar();

    switch(CMD_FLAG)
    {
        case 'a':
            if(START_FLAG == 0)
            {
               printf("STARTING...WAITE...");
               if(DJI_Pro_Test_Setup() < 0)
               {
                   printf("Serial Port Open ERROR\n");
                //   return 0;
               }
               else
               {
                   memset(&drone_ctrl_data, 0, sizeof(drone_ctrl_data));
               }

               usleep(500000);
               DJI_Onboard_API_Activation();
               usleep(500000);
               DJI_Onboard_API_Control(1);

               usleep(500000);
                //take off
               DJI_Onboard_API_UAV_Control(4);

               usleep(500000);
        //  Start_Control_Thread();
               Start_CV_Thread() ;
               Start_Height_Control_Thread() ;
               usleep(1000000);
               printf("\033[2J");

               printf("---------------\n");
               printf("!!!START FLY!!!\n");
               printf("---------------\n");
               START_FLAG = 1;
            }
            else
            {
                printf("\033[2J");
                printf("---------------\n");
                printf("!!!  Flying  !!!\n");
                printf("---------------\n");

            }
            Display_Menu();
            CMD_FLAG = 0;
        break;

        case 'b':
            printf("\033[2J");
            Set_LW_PID_Param();
            CMD_FLAG = 0;
        break;

        case 'c':
            printf("\033[2J");
            Set_Height_PID_Param();
            CMD_FLAG = 0;
        break;

        case 'd':
            printf("\033[2J");
            Set_YAW_PID_Param();
            CMD_FLAG = 0;

        break;

        case 'e':
        break;

        case 'f':
            //Landing
            DJI_Onboard_API_UAV_Control(6);
        break;

        case 'g':
            Set_Height_Param();
            CMD_FLAG = 0;
        break;

        case 'm':
            printf("\033[2J");
            Display_Menu();
            CMD_FLAG = 0;
        break;

        case 'p':
            cout << "p" <<endl ;
            value=value+50;
            ExposureChange(value);
            CMD_FLAG = 0;
        break;

        case 'l':
            cout << "l" <<endl ;
            value=value-10;
            ExposureChange(value);
            CMD_FLAG = 0;
        break;

        case 'k':
            cout << "k" <<endl ;
            value=value-1;
            ExposureChange(value);
            CMD_FLAG = 0;
        break;

        case 'q':

            //Close the camera&Serial
              Camera_Serial_Close();
              //stop the CV_Thread
              //Stop_the_CVThread();
              printf("Release Camera Finished!,\n");
              CMD_FLAG = 0;
        break;

        default:
            printf("Wrong Order,\n");
            CMD_FLAG = 0;
            Choose_Menu();
        break;


    }


}
