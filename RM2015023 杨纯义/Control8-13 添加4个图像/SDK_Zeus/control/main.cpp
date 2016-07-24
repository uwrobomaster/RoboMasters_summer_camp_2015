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
#include <sys/stat.h>


using namespace cv;
using namespace std;
float Rho_Actual;
float Theta_Actual;
 float deltaCam;
float R_kp,R_kd,T_kp,T_kd,x,Height_Set;
Rho_PID Rho;
Theta_PID Theta;
char filename[20];
char videoname[20];
VideoWriter wrVideo;
extern bool cubesDetected;
float SpeedZ_out,Rate_YAW_out,SpeedY_out;
void parameterSet();
int initPlane();

///////////////////////////////////onboard SDK/////////////////////////////////////////////////////
// int system(const char * string);
int main()
{

    cout<<"\n\n========================"<<endl;
    cout<<"please enter the filename: "<<endl;
    cout<<"=========================="<<endl;

     Start_ImagDect_Thread();
     while(1){
          usleep(10000);
     }


    cout<<"\n\n========================"<<endl;
    cout<<"please enter the filename: "<<endl;
    cout<<"=========================="<<endl;

    cin>>filename;
    sprintf(videoname,"%s.avi",filename);
    wrVideo.open(videoname,CV_FOURCC('M','J','P','G'),25,Size(150,100));
   // mkdir(filename,777);
    char filename1[20];
    char filename2[20];
    char filename3[20];
    char filename4[20];
    sprintf(filename1,"%s%s.txt","yaw",filename);
    sprintf(filename2,"%s%s.txt","speed",filename);
    sprintf(filename3,"%s%s.txt","theta",filename);
    sprintf(filename4,"%s%s.txt","rho",filename);
    ofstream file_yaw(filename1,ios::out);//文件以输出的模式打开
    ofstream file_speed(filename2,ios::out);
    ofstream file_theta(filename3,ios::out);
    ofstream file_rho(filename4,ios::out);


    initPlane();//初始化


    //float time=getTickCount();
    while(1)//輸出信息至文件中
    {
        /*****PID_RHO&&Theta********/
       Theta_Actual=drone_image_data.besttheta;
       Rho_Actual=drone_image_data.bestrho;

       file_yaw<<Rate_YAW_out<<' '<<endl;
       file_speed<<SpeedY_out<<' '<<endl;
       file_rho<<drone_image_data.bestrho<<' '<<endl;
       file_theta<<drone_image_data.besttheta<<' '<<endl;

       if(cubesDetected){
           drone_ctrl_data.x=-0.2;
           break;
       }
       usleep(10000);
    }

    //DJI_Onboard_API_UAV_Control(6);
    while(1);

    wrVideo.release();
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
    Start_ImagDect_Thread();
    usleep(1000000);
}


// // parse command line options to change default behavior
//   /*通过指令来修改默认的参数
//   h:
//   h-->指令代碼
//   :-->指令參數
//   形式：-h 参数 -a 参数 -d 参数*/
//void parseOptions(int argc, char* argv[]) {
//    int c;
//    while ((c = getopt(argc, argv, ":h?adtC:F:H:S:W:E:G:B:D:")) != -1) {
//      // Each option character has to be in the string in getopt();
//      // the first colon changes the error character from '?' to ':';
//      // a colon after an option means that there is an extra
//      // parameter to this option; 'W' is a reserved character
//      switch (c) {
//      case 'h':
//      case '?':
//        cout << usage;
//        exit(0);
//        break;
//      case 'a':
//        m_arduino = true;
//        break;
//      case 'd':
//        m_draw = false;
//        break;
//      case 't':
//        m_timing = true;
//        break;
//      case 'C':
//        setTagCodes(optarg);
//        break;
//      case 'F':
//        m_fx = atof(optarg);
//        m_fy = m_fx;
//        break;
//      case 'H':
//        m_height = atoi(optarg);
//        m_py = m_height/2;
//         break;
//      case 'S':
//        m_tagSize = atof(optarg);
//        break;
//      case 'W':
//        m_width = atoi(optarg);
//        m_px = m_width/2;
//        break;
//      case 'E':
//#ifndef EXPOSURE_CONTROL
//        cout << "Error: Exposure option (-E) not available" << endl;
//        exit(1);
//#endif
//        m_exposure = atoi(optarg);
//        break;
//      case 'G':
//#ifndef EXPOSURE_CONTROL
//        cout << "Error: Gain option (-G) not available" << endl;
//        exit(1);
//#endif
//        m_gain = atoi(optarg);
//        break;
//      case 'B':
//#ifndef EXPOSURE_CONTROL
//        cout << "Error: Brightness option (-B) not available" << endl;
//        exit(1);
//#endif
//        m_brightness = atoi(optarg);
//        break;
//      case 'D':
//        m_deviceId = atoi(optarg);
//        break;
//      case ':': // unknown option, from getopt
//        cout << intro;
//        cout << usage;
//        exit(1);
//        break;
//      }
//    }

//    if (argc > optind) {
//      for (int i=0; i<argc-optind; i++) {
//        m_imgNames.push_back(argv[optind+i]);
//      }
//    }
//  }
