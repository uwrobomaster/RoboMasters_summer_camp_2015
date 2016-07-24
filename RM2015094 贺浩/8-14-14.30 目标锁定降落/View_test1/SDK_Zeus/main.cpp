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
extern float Act_Hei;
float X_Offset_Actual;
extern float SpeedZ_out,Rate_YAW_out,SpeedY_out;
float fixtime;
Rho_PID Rho;
Theta_PID Theta;
X_Offset_PID X_Offset;
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
        if(drone_image_data.blackDetected)
        {
            x=0;
            X_Offset_Actual=-(drone_image_data.centers[0].y-50);
            Rho_Actual=0.2*(drone_image_data.centers[0].x-75);
           cout<<"X_Offset_Actual:  "<<X_Offset_Actual<<endl;
           if(X_Offset_Actual<=2&&Rho_Actual<=2){
                DJI_Onboard_API_UAV_Control(6);
               //Height_Set=0;
//               sleep(1);
//                if(X_Offset_Actual<=2&&Rho_Actual<=2){
//                    DJI_Onboard_API_UAV_Control(6);
//                }
//               if(fixtime==0) {
//                   fixtime =getTickCount();
//               }
//               else if( 1000 * ((double)getTickCount() - fixtime) / getTickFrequency()>1000) {
//                   DJI_Onboard_API_UAV_Control(6);
//                   Height_Set=0;
//               }
           }
        }

        if(!drone_image_data.blackDetected)  Rho_Actual=drone_image_data.bestrho;
        Theta_Actual=drone_image_data.besttheta;

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
    x=0.2;
    R_kp=0.003;
    R_kd=0.025;
    T_kp=1;
    T_kd=2;
    deltaCam=0.5;
    Height_Set=1.2;
    while(true){
        cout<<"\n\n=============================="<<endl;
        cout<<"please enter parameters to set:"<<endl;
        cout<<"a. R_kp"<<endl;
        cout<<"b. R_kd"<<endl;
        cout<<"c. x"<<endl;
        cout<<"d. T_kp"<<endl;
        cout<<"e. T_kd"<<endl;
        cout<<"f.  deltaCam"<<endl;
        cout<<"g. target height"<<endl;
        cout<<"h. exit"<<endl;
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
            cout<<"deltaCam: ";
            cin>>deltaCam;
        }
        if(choice=='g'){
            cout<<"set height: ";
            cin>>Height_Set;
        }
        if(choice=='h'){
            break;
        }
        cout<<"\nparameters: R_kp:"<<R_kp<<"  ,R_kd:"<<R_kd<<"  , x:"<<x<<"  ,T_kp:"<<T_kp<<"  ,T_kd:"<<T_kd<<"  ,deltaCam:"<<deltaCam<<"  ,Height:"<<Height_Set<<endl;
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

//    while(1)
//    {
//        if(Act_Hei>=0.8) break;
//        printf("Act Height: %f\n",Act_Hei);
//        waitKey(20);
//    }
    Start_ImagDect_Thread();


    usleep(1000000);
}
