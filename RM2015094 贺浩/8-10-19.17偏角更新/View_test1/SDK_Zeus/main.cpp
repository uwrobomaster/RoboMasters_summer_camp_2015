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
float R_kp,R_kd,T_kp,T_kd,x,Height_Set;
Rho_PID Rho;
Theta_PID Theta;
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
        cout<<"c. x"<<endl;
        cout<<"d. T_kp"<<endl;
        cout<<"e. T_kd"<<endl;
        cout<<"f. target height"<<endl;
        cout<<"g. exit"<<endl;
        cout<<"=============================="<<endl;
        char choice;
        cin>>choice;
        if(choice=='a'){
            cout<<"set R_kp: ";
            cin>>R_kp;
        }
        if(choice=='b'){
            cout<<"set R_kd: ";
            cin>>R_kd;
        }
        if(choice=='c'){
            cout<<"set x:";
            cin>>x;
        }
        if(choice=='d'){
            cout<<"set T_kp: ";
            cin>>T_kp;
        }
        if(choice=='e'){
            cout<<"set T_kd: ";
            cin>>T_kd;
        }
        if(choice=='f'){
            cout<<"set height: ";
            cin>>Height_Set;
        }
        if(choice=='g'){
            break;
        }
        cout<<"\nparameters: R_kp: "<<R_kp<<"  ,R_kd: "<<R_kd<<"  , x: "<<x<<"   ,T_kp: "<<T_kp<<",T_kd:"<<T_kd<<",Height:"<<Height_Set<<endl;

    }
    cout<<"exit parameter settings"<<endl;

    drone_ctrl_data.x=x;
    Rho.Kp=R_kp;
    Rho.Kd=R_kd;
    Theta.Kp=T_kp;
    Theta.Kd=T_kd;
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
