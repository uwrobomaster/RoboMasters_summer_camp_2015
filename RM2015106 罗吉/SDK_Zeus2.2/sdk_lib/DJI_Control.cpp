/*
 * DJI_Pro_App.cpp
 *
 *  Created on: July 25, 2015
 *      Author: Zeus Zhou
 */

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "sdk_lib/DJI_Control.h"
#include "sdk_lib/DJI_Pro_App.h"
#include "sdk_lib/DJI_Pro_Link.h"
#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/Logic_CV_Thread.h"
#include "sdk_lib/serial.h"


#define abs(x) ((x)>0?(x):(-x))
api_ctrl_data drone_ctrl_data;



/*
 * @Function:   Send Contrl Command by UART
 * @Input:
 * @Output:
*/
int Control_Cmd_Send(int flag)
{
    api_ctrl_without_sensor_data_t send_data;
    memset(&send_data, 0, sizeof(send_data));

 if(flag ==  XY_SPEED_YAW_RATE)
    send_data.ctrl_flag     = 0x4b;	//Vetical:velocity Horizon:velocity Yaw:angular rate

  else if( flag ==  XY_SPEED_YAW_ANGLE)
    send_data.ctrl_flag 	= 0x43;	//veti_velo hori_velo yaw_angle

  else if (flag == XY_POS_YAW_ANGLE)
    send_data.ctrl_flag 	= 0x83; //veti_velo hori_posi yaw_angle

    send_data.roll_or_x     = drone_ctrl_data.x;
    send_data.pitch_or_y    = drone_ctrl_data.y;
    send_data.thr_z         = drone_ctrl_data.z;
    send_data.yaw           = drone_ctrl_data.w;

    App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data), NULL, 0, 0);

	return 0;
}

/*
 * app_tan
 * Line_Walking
 *
 */
#define Line_Walking_TargetPos 0
extern PID_Param LW_PID;
extern PID_Param Height_PID;
extern PID_Param YAW_PID;
extern PID_Param Vertical_PID ;
float Line_Walking(float LW_Current_Pos)
{
    float LW_control_var = 0;

//    const float LW_P = 0.1;
//    const float Lw_I = 0.005;
//    const float LW_D = 0;//0.05;

    static float LW_inte = 0;

    static float LW_error[2] = {0.0, 0.0};

    if(abs(LW_Current_Pos) < 0.1)
        LW_Current_Pos = 0;

    LW_error[0] = LW_error[1];
    LW_error[1] = Line_Walking_TargetPos - LW_Current_Pos;
    LW_inte += LW_error[1];

    LW_control_var = LW_error[1] * LW_PID.P +
             LW_inte * LW_PID.I +
             (LW_error[1]-LW_error[0])  * LW_PID.D;

    if(LW_control_var > 2)
        LW_control_var = 2;
    if(LW_control_var < -2)
        LW_control_var = -2;

    if(LW_inte > 1)
        LW_inte = 1;
    if(LW_inte < -1)
        LW_inte = -1;

    return LW_control_var;
}

/*
 * app_tan
 * fly test fly the square
 * Vetical:velocity Horizon:velocity Yaw:angular rate
 */
void task_test_tan(int step)
{

   switch (step) {
   case 0:
       drone_ctrl_data.x = 0.5;
       drone_ctrl_data.y = 0;
       drone_ctrl_data.z = 0;
       drone_ctrl_data.w = 0;
       break;
   case 1:
       drone_ctrl_data.x = 0;
       drone_ctrl_data.y = 0.5;
       drone_ctrl_data.z = 0;
       drone_ctrl_data.w = 0;
       break;
   case 2:
       drone_ctrl_data.x = -0.5;
       drone_ctrl_data.y = 0;
       drone_ctrl_data.z = 0;
       drone_ctrl_data.w = 0;
       break;
   case 3:
       drone_ctrl_data.x = 0;
       drone_ctrl_data.y = -0.5;
       drone_ctrl_data.z = 0;
       drone_ctrl_data.w = 0;
       break;
   case 4://Landing
       DJI_Onboard_API_UAV_Control(6);
       break;
   case 5:

   default:
       break;
   }

}

/*
 * app_tan
 * fly task_test_tan();
 * Vetical:velocity Horizon:velocity yaw:angle
 */
void fly_task(int step)
{

    switch(step)
    {
        case 0://Tak off finish,Go ahead a little while
             drone_ctrl_data.x = 0.5;
             drone_ctrl_data.y = 0;
             drone_ctrl_data.z = 0;
             drone_ctrl_data.w = 0;
            break;
        case 1://line walking
             drone_ctrl_data.x = 0.5;
//             drone_ctrl_data.y = Line_Walking(get_image();
             drone_ctrl_data.z = 0;
             drone_ctrl_data.w = 0;
            break;
        case 2://arrived the square target,slow down
            break;
        case 3:
            break;
        case 4:
            break;

        default://other status,Landing
            DJI_Onboard_API_UAV_Control(6);
            break;
    }
}

/*
 * app_tan
 * Height_PID_Control_Test
 * Vetical:velocity Horizon:velocity Yaw:angular rate
 */
extern sdk_std_msg_t recv_sdk_std_msgs;
float Height = 1;
void Height_PID_Control_Test(int H)
{

   drone_ctrl_data.z = PID(Height,recv_sdk_std_msgs.pos.height);

}

/*
 * @Function:   Thread funciton, run at 50Hz
 * @Input:
 * @Output:
*/

static void * Control_Thread_Function(void * arg)
 {
    static int fly_cnt = 0;
    static int fly_step = 0;
    static float vz = 0;
    static float vw = 0;
    static float vx = 0;
    static float vy = 0 ;
 	while(1)
 	{	

        fly_cnt++;

        if(fly_cnt % 200 == 0 )
        {
         //   task_test_tan((fly_step));
            fly_step++;
        //    printf("FLY_STEP %d\n",fly_step);

         // send date to machine hand
          serial();

        }
        vz = PID(Height,recv_sdk_std_msgs.pos.height);

        control(0,vy,vz,vw) ;
 //Height_PID_Control_Test(1);
         //Control_Cmd_Send();
        usleep(20000);  //run at 50Hz

        
        


 	}

 	return (void *)NULL;
 }
/*
static void * Control_Thread_Function(void * arg)
 {
    static int fly_cnt = 0;
    static int fly_step = 0;
    float w = 0;
    float Y = 0;
    while(1)
    {
        switch( SystemState )
        {
            case(SystemState_FollowLine) :
            {
               w =  PID_Yaw( 0) ;
               Y = PID_Y(0) ;

              control(0  , 0 , 0, w ) ;

              break ;
            }

        }
        Control_Cmd_Send() ;
    usleep(100000);  //run at 50Hz

    }

    return (void *)NULL;
 } */

/*
 * @Function:   Creat a thread to send contrl command
 * @Input:
 * @Output:
*/
int Start_Control_Thread(void)
{
	int ret;
	pthread_t A_ARR;
	ret = pthread_create(&A_ARR, 0,Control_Thread_Function,NULL);
	if(ret != 0)
    { //
		return -1;
	}
	return 0;
}

/*
 * @Function:   Convert Quaternion to Rotation Matrix
 * @Input:      Quaternion Array q[4]
 * @Output:     Rotation Matrix Array r[9]
*/
int q2r(const float *pq, float *pr)
{
	float x, y, z, r;

	r = pq[0]; x = pq[1]; y = pq[2]; z = pq[3];
    pr[0] = r*r+x*x-y*y-z*z;;
	pr[1] = 2*(x*y-r*z);
	pr[2] = 2*(z*x+r*y);

	pr[3] = 2*(x*y+r*z);
	pr[4] = r*r-x*x+y*y-z*z;
    pr[5] = 2*(y*z-r*x);

	pr[6] = 2*(z*x-r*y);
	pr[7] = 2*(y*z+r*x);;
	pr[8] = r*r-x*x-y*y+z*z;

	return 0;
}

/*
 * @Function:   Convert Rotation Matrix to Euler Angle
 * @Input:      Rotation Matrix Array r[9]
 * @Output:     Euler Angle Array e[3]  (e[0]:yaw e[1]:pitch e[2]:roll)
*/
int r2e(const float *pr, float *pe)
{
	float ex, ey, ez;

	ez = atan2(pr[3], pr[0]);
	ey = asin(-pr[6]);
	ex = atan2(pr[7], pr[8]);

	pe[0] = ez;		//yaw
	pe[1] = ey;		//pitch
	pe[2] = ex;		//roll

	return 0;
}

/*
 * @Function:   Convert Quaternion to Euler Angle
 * @Input:      Quaternion Array q[4]
 * @Output:     Euler Angle Array e[3]  (e[0]:yaw e[1]:pitch e[2]:roll)
*/
int q2e(const float *pq, float *pe)
{
	float rr[3*3];

	q2r(pq, rr);
	r2e(rr, pe);

	return 0;
}

/*
 * @Function:   Just for testing
 * @Input:
 * @Output:
*/
int Set_Control_Param(void)
{
    drone_ctrl_data.x = 0;//2;
    drone_ctrl_data.y = 0;//3;
    drone_ctrl_data.z = 1;
    drone_ctrl_data.w = 0;//15;
    LW_PID.P = 0.35;
    LW_PID.I = 0;
    LW_PID.D = 0;
    Height_PID.P = 2.2;
    Height_PID.I = 0;
     Height_PID.D = 0 ;
  //  Height_PID.D = 10;

    YAW_PID.P = 0.25;
    YAW_PID.I = 0;
    YAW_PID.D = 0;

    Vertical_PID.P = 0.35;
    Vertical_PID.D = 0 ;
    Vertical_PID.I = 0;
    return -1;
}

/*
 * @Function:  PID
 * @Input:
 * @Output:
*/
float PID(float target,float current)
{


    static float inte = 0;

    static float error[2] = {0.0, 0.0};
    static float output = 0;
  float temp_p  ;
     if(abs(current) < 0.1)
        current = 0;

    error[0] = error[1];
    error[1] = target - current;

    if(abs(error[1]) < 0.5)
       temp_p = Height_PID.P ;
    else
        temp_p = Height_PID.P * 2;
    inte += error[1];

    output = error[1] * Height_PID.P +
            inte * Height_PID.I
             +(error[1]-error[0]) * Height_PID.D;

    if(output > 2)
        output = 2;
    if(output < -2)
        output = -2;

    if(inte > 1)
        inte = 1;
    if(inte < -1)
        inte = -1;

    return output;
}


