/*
 * DJI_Pro_App.cpp
 *
 *  Created on: July 25, 2015
 *      Author: Zeus Zhou
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/DJI_Control.h"
#include<stdlib.h>
#include<sdk_lib/DJI_Imag_Dect.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
float Rho_Actual;
float Theta_Actual;
 float deltaCam;
float R_kp,R_kd,T_kp,T_kd,x,Height_Set;
Rho_PID Rho;
Theta_PID Theta;
Hori_Vel_PID Hori_Vel_Adj;
float SpeedZ_out,Rate_YAW_out,SpeedY_out;
void parameterSet();
int initPlane();

///////////////////////////////////onboard SDK/////////////////////////////////////////////////////
 int system(const char * string);
int main()
{
    cout<<"\n\n========================"<<endl;
    cout<<"please enter the filename: "<<endl;
    cout<<"=========================="<<endl;

    char filename[20];
    cin>>filename;

    char filename1[20];
    char filename2[20];
    char filename3[20];
    char filename4[20];
    sprintf(filename1,"%s%s.txt","yaw",filename);
    sprintf(filename2,"%s%s.txt","speed",filename);
    sprintf(filename3,"%s%s.txt","theta",filename);
    sprintf(filename4,"%s%s.txt","rho",filename);
    ofstream file_yaw(filename1,ios::out);
    ofstream file_speed(filename2,ios::out);
    ofstream file_theta(filename3,ios::out);
    ofstream file_rho(filename4,ios::out);


    initPlane();


    //float time=getTickCount();
    while(1)
    {
        /*****PID_RHO********/
       Theta_Actual=drone_image_data.besttheta;
       Rho_Actual=drone_image_data.bestrho;

       file_yaw<<Rate_YAW_out<<' '<<endl;
       file_speed<<SpeedY_out<<' '<<endl;
       file_rho<<drone_image_data.bestrho<<' '<<endl;
       file_theta<<drone_image_data.besttheta<<' '<<endl;


       usleep(10000);

    }

    DJI_Onboard_API_UAV_Control(6);
    while(1);
    return -1;
}


void parameterSet(){
    while(true){
        cout<<"\n\n=============================="<<endl;
        cout<<"please enter parameters to set:"<<endl;
        cout<<"a. R_kp"<<endl;
        cout<<"b. R_kd"<<endl;
        cout<<"c. deltaCam"<<endl;
        cout<<"d. T_kp"<<endl;
        cout<<"e. T_kd"<<endl;
        cout<<"f. H_kp"<<endl;
        cout<<"g. H_kd"<<endl;
        cout<<"h.  x"<<endl;
        cout<<"i. target height"<<endl;
        cout<<"j. exit"<<endl;
        cout<<"=============================="<<endl;
        char choice;
        cin>>choice;
        if(choice=='a'){
            cout<<"set R_kp: ";
            cin>>Rho.Kp;
        }
        if(choice=='b'){
            cout<<"set R_kd: ";
            cin>>Rho.Kd;
        }
        if(choice=='c'){
            cout<<"deltaCam: ";
            cin>>deltaCam;
        }
        if(choice=='d'){
            cout<<"set T_kp: ";
            cin>>Theta.Kp;
        }
        if(choice=='e'){
            cout<<"set T_kd: ";
            cin>>Theta.Kd;
        }
        if(choice=='f'){
            cout<<"set H_kp: ";
            cin>>Hori_Vel_Adj.Kp;
        }
        if(choice=='g'){
            cout<<"set H_kd: ";
            cin>>Hori_Vel_Adj.Kd;
        }
        if(choice=='h'){
            cout<<"set x:";
            cin>>drone_ctrl_data.x;
        }
        if(choice=='i'){
            cout<<"set height: ";
            cin>>Height_Set;
        }
        cout<<"\nparameters: R_kp:"<<Rho.Kp<<"  ,R_kd:"<<Rho.Kd<<"  ,deltaCam:"<<deltaCam<<"  ,T_kp:"<<Theta.Kp<<"  ,T_kd:"<<Theta.Kd<<endl;
        cout<<"H_kp:"<<Hori_Vel_Adj.Kp<<"  H_kd:"<<Hori_Vel_Adj.Kd<<"  x:"<<drone_ctrl_data.x<<"  ,Height:"<<Height_Set<<endl;
        if(choice=='j'){
            break;
        }
    }
    cout<<"exit parameter settings"<<endl;
}


int initPlane(){
    if(DJI_Pro_Test_Setup() < 0)
    {
        printf("Serial Port Open ERROR\n");
        return 0;
    }
    else
    {
        memset(&drone_ctrl_data, 0, sizeof(drone_ctrl_data));
           printf("Serial Port Open ok\n");
    }

    usleep(500000);
    DJI_Onboard_API_Activation();
    usleep(500000);
    DJI_Onboard_API_Control(1);
    usleep(500000);

    parameterSet();

    DJI_Onboard_API_UAV_Control(4);
    usleep(500000);

    Start_Control_Thread();
    Start_ImagDect_Thread();
    usleep(1000000);
}
