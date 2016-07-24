/*
 * DJI_Pro_App.cpp
 *
 *  Created on: July 25, 2015
 *      Author: Zeus Zhou
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <signal.h>

#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/DJI_Control.h"
#include "sdk_lib/my_serial_lib.h"
#include "sdk_lib/DJI_Pro_Hw.h"
///////////////////////////////////onboard SDK/////////////////////////////////////////////////////
extern int line;
extern int n;

int speed=0;
int main(int argc,char **argv)
{
    unsigned char c;
    unsigned char buf[5]={0x01,'1','1','1','1'};
    unsigned char buf_2[5]={'5','6','7','8','9'};
    printf("start\n");
   // Pro_Hw_Setup("/dev/ttyUSB0",115200);
   // Pro_Hw_Setup("/dev/ttyUSB0",230400);
    serial_2_setup("/dev/ttyUSB0",115200);
    printf("here");
      while(1)
      {
         //  printf("here");
         //  my_delay(1);
       //   Pro_Hw_Send(buf,1);
    // Serial_2_Write(buf_2,5);

      }
    if(DJI_Pro_Test_Setup() < 0)
    {
        printf("Serial Port Open ERROR\n");
        return 0;
    }
    else
    {
        memset(&drone_ctrl_data, 0, sizeof(drone_ctrl_data));
        printf("Serial Port Open \n");
    }

    usleep(500000);
    DJI_Onboard_API_Activation();
    usleep(500000);
    DJI_Onboard_API_Control(1);
    usleep(500000);
    DJI_Onboard_API_UAV_Control(4);
    //DJI_Onboard_API_UAV_Control(6);
    usleep(500000);

    Start_Control_Thread();
    usleep(1000000);

    //Control Test
    Test_Control();
    //App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data), NULL, 0, 0);
    //usleep(5000000);

    while(1)
    {
        // Test_Control();
        //if(count>nq&&count<5*nq/2) speed++;
       // if(count>5*nq/2&&count<5*nq) speed--;
       // if(count==5*nq) speed=0;
       // drone_ctrl_data.x=speed;
       // squre(line);
        if(n>3) break;
       // if(count==100)
       // {
       //     drone_ctrl_data.y=0;
        //    drone_ctrl_data.w=180;
       // }

       // if(count==300) drone_ctrl_data.y=2;
        //if(count==400) drone_ctrl_data.w=180;
            //DJI_Onboard_API_UAV_Control(6);
       // if(count>=600) break;

    }
    DJI_Onboard_API_UAV_Control(6);
    while(1);
    return -1;
}
