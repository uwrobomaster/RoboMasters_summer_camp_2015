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
#include "sdk_lib/DJI_Imag_Dect.h"

api_ctrl_data drone_ctrl_data;

extern float parameters[3];


//#define XY_SPEED_YAW_RATE
#define XY_ATTI_YAW_RATE

/*
 * @Function:   Send Contrl Command by UART
 * @Input:
 * @Output:
*/
static int Control_Cmd_Send(void)
{
    api_ctrl_without_sensor_data_t send_data;
    memset(&send_data, 0, sizeof(send_data));

#ifdef XY_SPEED_YAW_RATE
    send_data.ctrl_flag     = 0x4b;	//Vetical:velocity Horizon:velocity Yaw:angular rate
#endif

#ifdef XY_SPEED_YAW_ANGLE
    send_data.ctrl_flag 	= 0x43;	//veti_velo hori_velo yaw_angle
#endif

#ifdef XY_POS_YAW_ANGLE
    send_data.ctrl_flag 	= 0x83; //veti_velo hori_posi yaw_angle
#endif

#ifdef XY_ATTI_YAW_RATE
    send_data.ctrl_flag     = 0x0b; //
#endif

    send_data.roll_or_x     = drone_ctrl_data.x;
    send_data.pitch_or_y    = drone_ctrl_data.y;
    send_data.thr_z         = drone_ctrl_data.z;
    send_data.yaw           = drone_ctrl_data.w;

    App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data), NULL, 0, 0);

	return 0;
}

/*
 * @Function:   Thread funciton, run at 50Hz
 * @Input:
 * @Output:
*/
/*******PID_Parameter*********/
extern float Act_Hei;
extern float Height_Set;
extern float Rho_Actual;
extern float Theta_Actual;
extern float Act_Spe_Y;
extern Rho_PID Rho;
extern Theta_PID Theta;
extern float SpeedZ_out,Rate_YAW_out,SpeedY_out,Y_Atti;
/*****************************/
static void * Control_Thread_Function(void * arg)
 {
      int count;
      Height_PIDinit();
      Theta_PIDinit();
      Rho_PIDinit();
 	while(1)
 	{		

                count++;
                /******PID_Height*******/
                SpeedZ_out=PID_Height_Adj(Height_Set,Act_Hei)/10.00;

                if(SpeedZ_out>=2)
                {
                    drone_ctrl_data.z=2;
                }
                else if(SpeedZ_out<=-2)
                {
                    drone_ctrl_data.z=-2;
                }
                else
                {
                    drone_ctrl_data.z=SpeedZ_out;
                }

                /******PID_offset_Adj*******/
                /*******Theta_Adj**********/
                Rate_YAW_out=PID_Theta_Adj(Theta_Actual);
               if(count%50==0)
                {
                     printf("Rate_Yaw= %f \n",Rate_YAW_out);
                }
                if(Rate_YAW_out>=20)
                {
                    drone_ctrl_data.w=20;
                }
                else if(Rate_YAW_out<=-20)
                {
                    drone_ctrl_data.w=-20;
                }
                else
                {
                    drone_ctrl_data.w=Rate_YAW_out;
                }


               /*******Rho_Adj************/
//                SpeedY_out=PID_Rho_Adj(Rho_Actual);
//                if(count%50==0)
//                {
//                    printf("SpeedY_out= %f \n",SpeedY_out);
//                }
//                if(SpeedY_out>0.8)
//                {
//                    drone_ctrl_data.y=0.8;
//                }
//                if(SpeedY_out<-0.8)
//                {
//                    drone_ctrl_data.y=-0.8;
//                }
//                else
//                {
//                    drone_ctrl_data.y=SpeedY_out;
//                }
                SpeedY_out=PID_Rho_Adj(Rho_Actual);
                Y_Atti=PID_Hori_Vel_Adj(SpeedY_out,Act_Spe_Y);
                if(count%50==0)
                {
                    printf("Y_Atti= %f \n",Y_Atti);
                }
                if(Y_Atti>=2.4)
                {
                    drone_ctrl_data.y=2.4;
                }
                if(Y_Atti<=-2.4)
                {
                    drone_ctrl_data.y=-2.4;
                }
                else
                {
                    drone_ctrl_data.y=Y_Atti;
                }
                /***************************/


 		Control_Cmd_Send();
//        if(count%50==0)
//        {
//            printf("***********************************Image_Data.bestrho=%d\n",drone_image_data.bestrho);
//            printf("***********************************Image_Data.besttheta=%d\n",drone_image_data.besttheta);
//        }

        usleep(20000);  //run at 50Hz
 	}

 	return (void *)NULL;
 }

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
	{
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
	pr[0] = r*r+x*x-y*y-z*z;
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
int Test_Control(void)
{
    drone_ctrl_data.x =0.3;
    drone_ctrl_data.y = 0;
    drone_ctrl_data.z = 0;
    drone_ctrl_data.w = 0;

    return -1;
}


/********PID**********/
Height_PID height_adj ;
Hori_Vel_PID Hori_Vel_Adj;
void  Height_PIDinit()
{
    height_adj.err=0.0;
    height_adj.err_last=0.0;
//  height_adj.err_next=0.0;
    height_adj.Kp=5;
    height_adj.Kd=30;
    height_adj.Ki=0;
}

void Hori_Vel_PIDinit()
{
    Hori_Vel_Adj.Actual_Hori_Vel=0.0;
    Hori_Vel_Adj.Set_Hori_Vel=0.0;
    Hori_Vel_Adj.err=0.0;
    Hori_Vel_Adj.err_last=0.0;
    Hori_Vel_Adj.err_next=0.0;
    Hori_Vel_Adj.Kp;
    Hori_Vel_Adj.Kd;
//  Hori_Vel_Adj.Ki;

}
float PID_Height_Adj(float Height_Set,float Height_Actual)
{
    float Height_Increment,PID_intergral;
    height_adj.err=Height_Set-Height_Actual;
//  PID_intergral+=height_adj.err;
    Height_Increment=height_adj.Kp*height_adj.err+height_adj.Kd*(height_adj.err-height_adj.err_last);
    height_adj.err_last=height_adj.err;

    return Height_Increment;
}


/*****   PIDTEST*******/
float PID_Hori_Vel_Adj(float Hori_Vel_Set,float Hori_Vel_Actual)
{
    float Hori_Vel_Increment;
    Hori_Vel_Adj.Set_Hori_Vel=Hori_Vel_Set;
    Hori_Vel_Adj.Actual_Hori_Vel=Hori_Vel_Actual;
    Hori_Vel_Adj.err=Hori_Vel_Adj.Set_Hori_Vel-Hori_Vel_Adj.Actual_Hori_Vel;
    Hori_Vel_Increment=Hori_Vel_Adj.Kp*(Hori_Vel_Adj.err-Hori_Vel_Adj.err_next)+Hori_Vel_Adj.Kd*(Hori_Vel_Adj.err-2*Hori_Vel_Adj.err_next+Hori_Vel_Adj.err_last);
    Hori_Vel_Adj.Actual_Hori_Vel+=Hori_Vel_Increment;
    Hori_Vel_Adj.err_last=Hori_Vel_Adj.err_next;
    Hori_Vel_Adj.err_next=Hori_Vel_Adj.err;

    return Hori_Vel_Adj.Actual_Hori_Vel;
}




void Theta_PIDinit()
{
    Theta.err=0.0;
    Theta.err_last=0.0;
//    Theta.err_next=0.0;
//    Theta.Kp=0.3;
//    Theta.Ki;
//    Theta.Kd=5;
}

void Rho_PIDinit()
{
    Rho.err=0.0;
    Rho.err_last=0.0;
//  Rho.err_next=0.0;
//  Rho.Kp=0.03;
//  Rho.Ki=0;
//  Rho.Kd=2;
}

float PID_Theta_Adj(float theta)
{
    float Theta_Increment;
    Theta.err=theta;
    Theta_Increment=Theta.Kp*Theta.err+Theta.Kd*(Theta.err-Theta.err_last);
    Theta.err_last=Theta.err;

    return Theta_Increment;
}


float PID_Rho_Adj(float rho)
{
    float Rho_Increment;
    Rho.err=rho;
    Rho_Increment=Rho.Kp*Rho.err+Rho.Kd*(Rho.err-Rho.err_last);
    Rho.err_last=Rho.err;

    return Rho_Increment;
}
/*****   PIDTEST*******
float Posture_Adj(float rho)
{
    Rho_PIDinit();

    while(!(fabs(rho)<2))
    {
        PID_Rho_Adj(rho);
    }

}*/
