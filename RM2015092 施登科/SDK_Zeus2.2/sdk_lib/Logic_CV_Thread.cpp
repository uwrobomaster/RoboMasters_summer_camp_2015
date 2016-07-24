#include <stdio.h>
#include "time.h"
#include <iostream>
#include"fstream"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "CV_header.h"
#include "Logic_CV_Thread.h"
#include "sdk_lib/DJI_Control.h"
#include "sdk_lib/DJI_Pro_App.h"
#include "sdk_lib/DJI_Pro_Link.h"
#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/Tan_Display.h"
#include "Tan_Display.h"
#include "serial.h"



extern PID_Param LW_PID;
extern PID_Param Height_PID;
extern PID_Param YAW_PID;
PID_Param LW_PID;
PID_Param Height_PID;
PID_Param YAW_PID;
PID_Param Vertical_PID ;
extern float Angle_Return;
extern float Dist_Return  ;
extern float Vertical_Return ;
extern float Blk_Vertical_Return ;
extern float Blk_Dist_Return ;
extern float Blk_Angle_Return ;
static int HeightCtrl_Flag = 1;
extern float X,Y;
using namespace std;
void control(float x,float y ,float z, float w )
{
    drone_ctrl_data.x = x ;
    drone_ctrl_data.y = y;
    drone_ctrl_data.z = z;
    drone_ctrl_data.w = w;
}

/*
 * @Author : Logic
 * @Function:  PID funcation calc the Pid of Vertical
 * @Input:set Vertical
 * @Output:  Drone control  variable Vertical
*/
float PID_Vertical(float set,float input)
{

   static float  pre_vertical = 0;
   static float pre_error = 0;
   float out  ;
   float error =set - (input)  ;
  //  printf("%f \n",Dist_Return -320 ) ;
   out= Vertical_PID.P *error + Vertical_PID.D * (error - pre_error) ;
   pre_vertical = input ;
   pre_error = error ;

   if(out< -0.5)
       out = -0.5;
   else if(out >0.5)
       out = 0.5;
    cout<<"Vertical Out = "<<out <<endl ;
    return out ;
}





/*
 * @Author : Logic
 * @Function:  PID funcation calc the Pid of Y
 * @Input:set Y
 * @Output:  Drone control  variable Y
*/
float PID_Y(float set,float input)
{

   static float  pre_y = 0;
   static float pre_error = 0;
   float out  ;
   float error =(input) - set ;
  //  printf("%f \n",Dist_Return -320 ) ;
   out= LW_PID.P *error + LW_PID.D * (error - pre_error) ;
   pre_y = input ;
   pre_error = error ;

   if(out< -0.5)
       out = -0.5;
   else if(out >1)
       out = 1;
   //cout<<"Walk Out = "<<out <<endl ;
    return out ;
}
/*
 * @Author : Logic
 * @Function:  PID funcation calc the Pid of yaw
 * @Input:set yaw
 * @Output:  Drone control  variable yaw
*/
float PID_Yaw(float set,float input)
{

   static float pre_yaw = 0;
   static float pre_error = 0;
   float out  ;
   float error = (input-0) - set ;
   out= YAW_PID.P *error + YAW_PID.D * (error - pre_error) ;
   pre_yaw = input ;
   pre_error = error ;

   if(out< -20)
       out = -20;
   else if(out > 20)
       out = 20;
    return out ;
}

/*
 * @Author : Logic
 * @Function:  PID funcation calc the Pid of yaw
 * @Input:set yaw
 * @Output:  Drone control  variable yaw
*/
extern sdk_std_msg_t recv_sdk_std_msgs;
float PID_Height(float set)
{

   static float pre_height = 0;
   static float pre_error = 0;
   float out  ;
   float error = (recv_sdk_std_msgs.pos.height-0) - set ;
   out= Height_PID.P *error + Height_PID.D * (error - pre_error) ;
   pre_height = recv_sdk_std_msgs.pos.height ;
   pre_error = error ;

   if(out< -1)
       out = -1;
   else if(out > 1)
       out = 1;
    return out ;
}
/*
 * @Author : Logic
 * @Function:   Thread funciton, run at 30Hz
 * @Input:
 * @Output:
*/


#define SystemState_WFTakeOff 0
#define SystemState_TakeOff 1
#define SystemState_FollowLine2BLKBox 2
#define SystemState_FollowLine2SudoKu 3
#define SystemState_SudoKu_1 4
#define SystemState_SudoKu_2 5
#define SystemState_BLKBox 6

extern float Height ;
int SystemState  = SystemState_FollowLine2SudoKu ;
int Cmd_Send_flag = XY_SPEED_YAW_RATE ;
float vz = 0;
float vw = 0;
float vx = 0;
float vy = 0 ;

/***************************************/
static void * CV_Thread_Function(void * arg)
{
    ofstream file("CV_Thread_Data.txt");
    float Euler[4] ;
    float q_[4] ;
    extern float x3,y3,x4,y4;
    int frame = 0,Locate_Counter = 0;
    int SodukuCheckCounter = 0;
    int Last_SodukuCheckFlag = 0,LocateFlag = 1,SodukuFirst_flag = 0;
    int Exposure_0_Counter = -1 ;
    serial();
    if (!file)
        {
        cout<<"Open file failed!"<<endl;
        }
    ExposureChange(100);

    file<<"frame  X  Y  Height  roll  pitch  yaw"  <<endl;
    cout <<"SystemState_FollowLine" <<endl ;
    Height = 1.5 ;
   while(recv_sdk_std_msgs.pos.height < 0.2)
   cout <<"Height = "<<recv_sdk_std_msgs.pos.height <<endl ;
    while(1)
    {
         q_[0] =recv_sdk_std_msgs.q.q0;
         q_[1] =recv_sdk_std_msgs.q.q1;
         q_[2] =recv_sdk_std_msgs.q.q2;
         q_[3] =recv_sdk_std_msgs.q.q3;
         q2e(q_, Euler   ) ;

           Grab_Image() ;
       switch(SystemState)
       {

           case SystemState_FollowLine2BLKBox :
           case SystemState_FollowLine2SudoKu :
          {
            if(SystemState == SystemState_FollowLine2SudoKu)
           {
              vx = 0.3 ;
            if( LineDetect(1) )
            {
                cout <<"Line1" <<endl ;
                cout <<"SystemState_SudoKu_1" <<endl ;
                vx = 0;
                ExposureChange(0);
                if(Exposure_0_Counter==-1)
                Exposure_0_Counter++ ;

            }

            if(Exposure_0_Counter != -1)
                {
                  Exposure_0_Counter++ ;
                  vx = 0;
                }
            if(Exposure_0_Counter == 8)
                {

                    SystemState = SystemState_SudoKu_1 ;
                    Exposure_0_Counter = -1;
                }
           }
            else if(SystemState == SystemState_FollowLine2BLKBox)
            if( LineDetect(2) )
            {
                SystemState = SystemState_BLKBox ;

                cout <<"SystemState_BLKBox"<<endl ;

            }



            vy = PID_Y(0,Dist_Return) ;
            vw = PID_Yaw(0,Angle_Return) ;
            Cmd_Send_flag = XY_SPEED_YAW_RATE ;
           //file<<frame<<"#"<<recv_sdk_std_msgs.pos.height<<" "<<Euler[0]<<" "<<Euler[1]<<" "<<Euler[2]<<" "<<x3<<" "<<y3<<" "<<x4<<" "<<y4<<endl ;
           frame ++ ;

           usleep(100) ;
           break ;
          }
          case(SystemState_SudoKu_1):
           {

               vx = 0.2 ;
               vy = 0;
               vw = Euler[0]*180/3.1415926 ;
               Cmd_Send_flag = XY_SPEED_YAW_ANGLE ;
                Last_SodukuCheckFlag = SodukuCheck();
           if(Last_SodukuCheckFlag )
             {
                SystemState = SystemState_SudoKu_2 ;
                cout <<"SystemState_SudoKu_2" <<endl ;
             }

           CV_Show(1);
           break ;

           }
         case(SystemState_SudoKu_2):
           {
               if(  SodukuFirst_flag == 0 )
               {

                   if(Dist_Return > 0.2) // youbian
                       SodukuFirst_flag = 1;
                    else if(Dist_Return < -0.2)   //zuo bian
                        SodukuFirst_flag =2 ;
                   else  //zhongjian
               SodukuFirst_flag =3 ;


               }

               if( !( Last_SodukuCheckFlag = SodukuCheck() ))  //丢了 保持当前
                  {

                    vw = 0;
                   vx = 0 ;
                  vy = 0 ;

                   if( Last_SodukuCheckFlag == 0 ) //是连续的哦
                    SodukuCheckCounter++ ;
                    if(SodukuCheckCounter == 10)  //连续10张没有找到 日狗了 飞到 2米
                     {
                        cout << " Up Up Up"<<endl ;
                        Height = 1.7 ;
                     }
                  }
                else
                {
                    SodukuCheckCounter = 0 ;
                     if(abs(Dist_Return) < 0.1 && abs(Vertical_Return) <0.1)
                        {
                         Height = 1.2 ;
                         }
            /*                cout<<"Target Locating,Height = "<<Height<<endl ;
                            vw = Euler[0]*180/3.1415926 + 180 ;
                            if(vw > 180)
                            vw -= 360 ;

                            vx = 0;
                            vy = 0;
                            Cmd_Send_flag = XY_POS_YAW_ANGLE ;
                            usleep(3000000) ;
                            if(SodukuFirst_flag == 2)
                            vy = -0.3 ;
                            else if(SodukuFirst_flag == 1)
                            vy = 0.3;
                            else if(SodukuFirst_flag == 3)
                                vy = 0 ;

                            vx = 0.3;

                               usleep(1000000) ;
                            ExposureChange(100) ;
                            SystemState = SystemState_FollowLine2BLKBox  ; */
                      // }
           //         else
            //          {  /**/
                                  vx = PID_Vertical(0.05,Vertical_Return) ;
                                  vy = PID_Y(0,Dist_Return) ;
                                  vw = Euler[0]*180/3.1415926 ;
                                  Cmd_Send_flag = XY_SPEED_YAW_ANGLE ;



              //      }
                    //到了丢球
                }

                         CV_Show(1);
                       break ;

           }

         case(SystemState_BLKBox) :
       {
           if(BlackBoxCheck())
           {
               vw = PID_Yaw(0,Blk_Angle_Return);
               vx = PID_Vertical(0,Blk_Vertical_Return-0.1) ;
               vy = PID_Y(0,Blk_Dist_Return-0.1) ;
HeightCtrl_Flag = 1;
               Cmd_Send_flag = XY_SPEED_YAW_RATE ;

                   if(abs(Blk_Dist_Return-0.1) < 0.05 && abs(Blk_Vertical_Return-0.1) <0.05 && abs(Blk_Angle_Return)<2 && LocateFlag == 1)
                   {
                       vz= -0.7 ;
                     vx = 0 ;
                     vy = 0 ;
                       Locate_Counter = 0;
                       HeightCtrl_Flag = 0;
                       usleep(3000000) ;
                       vz = 0;
                      DJI_Onboard_API_UAV_Control(6);

                          cout<<"Target Locating,Height = "<<recv_sdk_std_msgs.pos.height <<endl ;

                                        }
                   else
                   {
                       Height =  recv_sdk_std_msgs.pos.height  ;
                       HeightCtrl_Flag = 1;
                        if((abs(Blk_Dist_Return-0.1) < 0.05 )&& abs((Blk_Vertical_Return-0.1)<0.05)&& abs(Blk_Angle_Return)<2 )
                       Locate_Counter++ ;
                       if(Locate_Counter == 5)
                       LocateFlag = 1;
                       else
                       LocateFlag = 0 ;

                   }
                             //Landing
         /*          if( recv_sdk_std_msgs.pos.height  < 0.9)
                       {
                       vz = -0.5 ;
                       vx = 0 ;
                                    vy = 0 ;
                                    vw = 0 ;
                           usleep(3000000) ;
                           DJI_Onboard_API_UAV_Control(6);
                   }*/

                   cout<<"Target Locating,Height = "<<recv_sdk_std_msgs.pos.height <<endl ;

                       ;
             }
           else
               ;

        break ;

       }

       default: break ;
       }
       //file<<frame<<" "<< vx<<" "<<vy<<" "<<Dist_Return<<" "<<Vertical_Return<<" "<<recv_sdk_std_msgs.pos.height<<endl;
         file<<frame<<"#"<<X<<" "<<Y<<" "<<recv_sdk_std_msgs.pos.height<<" "<<Euler[0]<<" "<<Euler[1]<<" "<<Euler[2]<<endl ;
       CV_Show(1);

 //          control(vx,vy,vz,vw) ;

 //           Control_Cmd_Send(1);

         usleep(70000) ;

    }
    file.close();

}
/*
 * @Author : Logic
 * @Function:  Height control and cmd send
 * @Input:
 * @Output:
*/
static void *Height_Control_Funcation(void * arg)
{
   while(1)
   {
       if(HeightCtrl_Flag == 1)
    vz = PID(Height,recv_sdk_std_msgs.pos.height);
    control(vx,vy,vz,vw) ;
    Control_Cmd_Send(Cmd_Send_flag) ;
    usleep(50000)  ;
   }
}
/*
 * @Author : Logic
 * @Function:  CV thread Init
 * @Input:
 * @Output:
*/

int Start_CV_Thread(void)
{
    int ret;
    pthread_t A_ARR;

    ret = pthread_create(&A_ARR, 0,CV_Thread_Function,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
int Start_Height_Control_Thread(void)
{
    int ret;
    pthread_t A_ARR;

    ret = pthread_create(&A_ARR, 0,Height_Control_Funcation,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
void Stop_the_CVThread(void)
{
    pthread_exit(0);

}


