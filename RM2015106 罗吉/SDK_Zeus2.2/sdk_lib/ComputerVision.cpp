
///////////////////////////////////////////////////////////////////////////////
// File generated by HDevelop for HALCON/C++ Version 12.0.1
///////////////////////////////////////////////////////////////////////////////


#include"iostream"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"time.h"

#include <libv4l2.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <errno.h>

#include "sdk_lib/DJI_Pro_Hw.h"

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif
#include "sdk_lib/DJI_Control.h"
#include "sdk_lib/DJI_Pro_App.h"
#include "sdk_lib/DJI_Pro_Link.h"
#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/Logic_CV_Thread.h"

#include "CV_header.h"
using namespace HalconCpp;
using namespace cv;
using namespace std;

//image from halcon to opencv
Mat halcon2opencv(HObject &img);
HObject opencv2halcon(Mat img);
// Procedures
// External procedures
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'.

HObject  ho_Img, ho_R, ho_G, ho_B, ho_Y, ho_U;
HObject  ho_V, ho_Img_yuv, ho_R_regions, ho_R_regionsDilation;
HObject  ho_RegionTrans, ho_Intersection1, ho_Img_yuvChannel21;
HObject  ho_Img_yuvChannel31, ho_opening, ho_closing, ho_Skeleton;
HObject  ho_LinesHNF, ho_SudokuErosion, ho_LinesSelected;
HObject  ho_LinesUnion, ho_LinesCircle, ho_ImageResult;
HObject   Hobj;
// Local control variables

HTuple  hv_FirstFlag, hv_PreAngle, hv_PreDist;
HTuple  hv_index, hv_Angle, hv_Dist, hv_SudokuArea, hv_SudokuRow;
HTuple  hv_SudokuColumn, hv_counter, hv_Angle_Norm, hv_Dist_Norm;
HTuple  hv_PreAngle_Norm, hv_PreDist_Norm, hv_PreAngle_Tuple;
HTuple  hv_PreDist_Tuple, hv_Distance, hv_Min, hv_TargetIndex;
HTuple  hv_exception,hv_CameraParam;
 HTuple  hv_AcqHandle;
 HTuple hv_pose ;
 HTuple hv_wcs_x,hv_wcs_y;
 HTuple hv_x0,hv_x1,hv_y0,hv_y1 ;

  Mat Imageresult;
  VideoCapture m_cap,m_cap1;
  float X,Y;
  //*************************************************************************************//
// Return Value To main thread
  float Angle_Return = 0;
  float Vertical_Return = 0 ;

  float Blk_Vertical_Return = 0;
    float Blk_Dist_Return = 0;
    float Blk_Angle_Return = 0;
float Dist_Return = 0 ;
  //*************************************************************************************//
float x3,y3,x4,y4;
HObject opencv2halcon(Mat img)
{
  //HObject   Hobj;
  int height=img.rows;
  int width=img.cols;

//  printf("Channels = %d", img.channels()) ;
  if(img.channels()==1)
  {

  uchar *dataGray=new uchar[width*height];
  for(int i=0; i<height; i++)
  {
  memcpy(dataGray+width*i, img.datastart+width*i,width);
  }

  GenImage1(&Hobj,"byte",width,height,(Hlong)(dataGray));
  delete[ ] dataGray;
  return Hobj;
  }

  if(img.channels()==3)
  {
  Mat  IplImageRed(width,height,CV_8UC1), IplImageGreen(width,height,CV_8UC1), IplImageBlue(width,height,CV_8UC1);
  vector<Mat> channels;
  split(img,channels);
  IplImageBlue=channels.at(0);
  IplImageGreen=channels.at(1);
  IplImageRed=channels.at(2);
  uchar*  dataRed=new uchar[width*height];
  uchar*  dataGreen=new uchar[width*height];
  uchar*  dataBlue=new uchar[width*height];
  for(int i=0; i<height; i++)
  {
  memcpy(dataRed+width*i, IplImageRed.datastart+width*i,width);
  memcpy(dataGreen+width*i, IplImageGreen.datastart+width*i,width);
  memcpy(dataBlue+width*i, IplImageBlue.datastart+width*i,width);
  }

  GenImage3(&Hobj,"byte",width,height,(Hlong)(dataRed),(Hlong)(dataGreen),(Hlong)(dataBlue));
  delete[ ]  dataRed;
  delete[ ]  dataGreen;
  delete[ ]  dataBlue;
  }
  return Hobj;
}

//image from halcon to opencv
Mat halcon2opencv(HObject &img)
{
    HTuple hv_PointerRed,hv_PointerGreen,hv_PointerBlue,hv_Pointer;
    HTuple hv_Type,hv_width,hv_Height;
    HTuple num_channel;
    CountChannels(img,&num_channel);
    if(num_channel==3)
    {
    GetImagePointer3(img,&hv_PointerRed,&hv_PointerGreen,&hv_PointerBlue,&hv_Type,&hv_width,&hv_Height);
    int width,height;
    width=*hv_width.LArr();
    height=*hv_Height.LArr();

    Mat IplImage(height,width,CV_8UC3), IplImageRed(height,width,CV_8UC1), IplImageGreen(height,width,CV_8UC1), IplImageBlue(height,width,CV_8UC1);
    for(int i=0; i<height; i++)
    {
    memcpy(IplImageRed.datastart+width*i,(char *)hv_PointerRed[0].L()+width*i,width);
    memcpy(IplImageGreen.datastart+width*i,(char *)hv_PointerGreen[0].L()+width*i,width);
    memcpy(IplImageBlue.datastart+width*i,(char *)hv_PointerBlue[0].L()+width*i,width);
    }
    vector<Mat>channels(3);
    channels.at(0)=IplImageBlue;
    channels.at(1)=IplImageGreen;
    channels.at(2)=IplImageRed;
    merge(channels,IplImage);

    return IplImage;

    }
    else
    {
        GetImagePointer1(img,&hv_Pointer,&hv_Type,&hv_width,&hv_Height);
        int width,height;
        width=*hv_width.LArr();
        height=*hv_Height.LArr();
        Mat IplImage(height,width,CV_8UC1);
        for(int i=0; i<height; i++)
        {
        memcpy(IplImage.datastart+width*i,(char *)hv_Pointer[0].L()+width*i,width);
        }
        //imshow("gray",IplImage);
        return IplImage;
    }
    //imshow("G",IplImageGreen);
    //imshow("B",IplImageBlue);
    //waitKey(0);
}

void CV_Init()
{
    int m_width=640; // image size in pixels
    int m_height=480;
    Mat testimg;
 int ep;
 int VideoNum = 0;
    int m_deviceId=0; // camera id (in case of multiple cameras)

      hv_FirstFlag = 1;
    hv_PreAngle = (0/180.0)*3.1415926;
    hv_PreDist = 320.0;
/*Open Camera*/
    while(1)
  {
   printf("Input Video Num = \n") ;
   printf("0 Or 1 \n");
     scanf("%d",&VideoNum) ;
     ReadCamPar("/home/rm12/桌面/新建文件夹 (2)/RM_Camera.cal", &hv_CameraParam);

   if(VideoNum == 0)
     {
       cout << "1 \n"<<endl ;
       m_cap=VideoCapture(0);
         break ;
       }

       else if(VideoNum == 1)
      {
       m_cap=VideoCapture(1);
       break ;
     }
    printf("Error Plear Input the data again \n") ;
   }
        if(!m_cap.isOpened()) {
      cerr << "ERROR: Can't find video device " << m_deviceId << "\n";
      exit(1);
    }

  /*       m_cap >> testimg;

             imshow("1",testimg) ;
             waitKey(10) ;
         cout<<"test channels = "<<testimg.channels()<<endl;
         if(testimg.channels() == 1)
             {
             cout<<"Open Video 0 Error ,Try to Open Video 1"<<endl ;
              m_cap=VideoCapture(1);

         } */

        m_cap.set(CV_CAP_PROP_FRAME_WIDTH, m_width);
    m_cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_height);
    cout << "Camera successfully opened (ignore error messages above...)" << endl;
    cout << "Actual resolution: "
         << m_cap.get(CV_CAP_PROP_FRAME_WIDTH) << "x"
         << m_cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

}
extern sdk_std_msg_t recv_sdk_std_msgs;

// flag == 1 终止条件为 九宫格
//
//
void ResetLineDetect()
{
    hv_PreAngle = (0/180.0)*3.1415926;
    hv_PreDist = 320.0;
}
int LineDetect(int flag)
{
    float q_[4] ;
    float Euler[4] ;
    float x1,x2,y1=0,y2=480 ;
    int BLK_flag;

     Decompose3(ho_Img, &ho_R, &ho_G, &ho_B);
    TransFromRgb(ho_R, ho_G, ho_B, &ho_Y, &ho_U, &ho_V, "yuv");
    Compose3(ho_Y, ho_U, ho_V, &ho_Img_yuv);
    Threshold(ho_Y, &ho_R_regions, 15, 250);
    DilationCircle(ho_R_regions, &ho_R_regionsDilation, 10);
    ShapeTrans(ho_R_regionsDilation, &ho_RegionTrans, "convex");

    Threshold(ho_Img_yuv, &ho_Intersection1, 192, 255);
    AccessChannel(ho_Img_yuv, &ho_Img_yuvChannel21, 2);
    //Note that the application of change_domain in the following is
    //safe because the new domains are calculated from the input image
    //and therefore are automatically consistent!
    ChangeDomain(ho_Img_yuvChannel21, ho_Intersection1, &ho_Img_yuvChannel21);
    Threshold(ho_Img_yuvChannel21, &ho_Intersection1, 102, 255);
    AccessChannel(ho_Img_yuv, &ho_Img_yuvChannel31, 3);
    ChangeDomain(ho_Img_yuvChannel31, ho_Intersection1, &ho_Img_yuvChannel31);
    Threshold(ho_Img_yuvChannel31, &ho_Intersection1, 0, 140);
    OpeningCircle(ho_Intersection1, &ho_opening, 3);
    OpeningCircle(ho_opening, &ho_opening, 3);

    ClosingCircle(ho_opening, &ho_closing, 3);
    OpeningCircle(ho_closing, &ho_opening, 3);
    Skeleton(ho_opening, &ho_Skeleton);
      //模式切换判断

     if(flag == 1)  //九宫格
    {
        ErosionCircle(ho_opening, &ho_SudokuErosion, 50);
        AreaCenter(ho_SudokuErosion, &hv_SudokuArea, &hv_SudokuRow, &hv_SudokuColumn);
        CountObj(ho_SudokuErosion, &hv_counter);
        if (0 != (HTuple(hv_SudokuArea[0])>2000))
        {
            Imageresult=halcon2opencv(ho_Img);

          return  1;
        }
    }
    else if(flag == 2) // 黑盒子
    {

        BLK_flag = BlackBoxCheck()  ;


        if(BLK_flag == 1 && Blk_Vertical_Return < 0.3    )  // 检测到黑盒子且距离够近了，模式切换
            return 1;
    }




    //hough_lines (Skeleton, 4, 1000, 10, 500, Angle, Dist)
  //  printf("4 \n") ;
    try
    {
      HoughLines(ho_Skeleton, 4, 100, 200, 100, &hv_Angle, &hv_Dist);
      GenRegionHline(&ho_LinesHNF, hv_Angle, hv_Dist);

      if (0 != hv_FirstFlag)
      {
        //*             for i := 0 to |Angle|-1 by 1
        //均一化

        hv_Angle_Norm = hv_Angle/(2*3.1415926);
        hv_Dist_Norm = hv_Dist/800;
        hv_PreAngle_Norm = hv_PreAngle/(2*3.1415926);
        hv_PreDist_Norm = hv_PreDist/800.0;
        TupleGenConst(hv_Angle.TupleLength(), hv_PreAngle_Norm, &hv_PreAngle_Tuple);
        TupleGenConst(hv_Dist.TupleLength(), hv_PreDist_Norm, &hv_PreDist_Tuple);

        DistancePp(hv_Angle_Norm, hv_Dist_Norm, hv_PreAngle_Tuple, hv_PreDist_Tuple,
            &hv_Distance);
        TupleMin(hv_Distance, &hv_Min);

     //   cout<<*hv_Min.DArr()<<endl;

        if(    *hv_Min.DArr() >0.20 )
        {

            hv_PreAngle = (0/180.0)*3.1415926;
            hv_PreDist = 320.0;
            hv_PreAngle_Norm = hv_PreAngle/(2*3.1415926);
            hv_PreDist_Norm = hv_PreDist/800.0;
            TupleGenConst(hv_Angle.TupleLength(), hv_PreAngle_Norm, &hv_PreAngle_Tuple);
            TupleGenConst(hv_Dist.TupleLength(), hv_PreDist_Norm, &hv_PreDist_Tuple);

            DistancePp(hv_Angle_Norm, hv_Dist_Norm, hv_PreAngle_Tuple, hv_PreDist_Tuple,
                &hv_Distance);
            TupleMin(hv_Distance, &hv_Min);

         }

        q_[0] =recv_sdk_std_msgs.q.q0;
        q_[1] =recv_sdk_std_msgs.q.q1;
        q_[2] =recv_sdk_std_msgs.q.q2;
        q_[3] =recv_sdk_std_msgs.q.q3;
        q2e(q_, Euler   ) ;

        TupleFind(hv_Distance, hv_Min, &hv_TargetIndex);
        GenRegionHline(&ho_LinesSelected, HTuple(hv_Angle[hv_TargetIndex]), HTuple(hv_Dist[hv_TargetIndex]));

     hv_PreAngle =  ((const HTuple&)hv_Angle)[hv_TargetIndex];
     hv_PreDist = ((const HTuple&)hv_Dist)[hv_TargetIndex];

    //    hv_PreAngle = (0/180.0)*3.1415926;
   //     hv_PreDist = 320.0;
// (e[0]:yaw e[1]:pitch e[2]:roll)
        hv_pose.Clear() ;
        hv_pose[0] = 0 ;
        hv_pose[1] = 0;
        hv_pose[2] =  recv_sdk_std_msgs.pos.height + 0.2;
        hv_pose[3] = Euler[1] ; //pitch
        hv_pose[4] = Euler[2] ; //roll
        hv_pose[5] = 0 ;
        hv_pose[6] = 0 ;
        x1 = (hv_PreDist - y1 * sin(*hv_PreAngle.DArr()) ) / cos(*hv_PreAngle.DArr()) ;
        x2 = (hv_PreDist - y2 * sin(*hv_PreAngle.DArr()) ) / cos(*hv_PreAngle.DArr()) ;


        ImagePointsToWorldPlane(hv_CameraParam,hv_pose,(HTuple(y1).Append(y2)) ,(HTuple(x1).Append(x2)),"m",&hv_wcs_x,&hv_wcs_y);
      hv_x0 = ((const HTuple&)hv_wcs_x)[0];
         hv_x1 = ((const HTuple&)hv_wcs_x)[1];
         hv_y0 = ((const HTuple&)hv_wcs_y)[0];
            hv_y1 = ((const HTuple&)hv_wcs_y)[1];
    //    cout<<"Point 0 :"<<* hv_x0.DArr()<< "  " <<* hv_y0.DArr() <<"Point 1 :"<<* hv_x1.DArr()<< "  " <<* hv_y1.DArr() <<endl;
        x3=* hv_x0.DArr();
        y3=* hv_y0.DArr();
        x4=* hv_x1.DArr();
        y4=* hv_y1.DArr();

        Union1(ho_LinesSelected, &ho_LinesUnion);
        DilationCircle(ho_LinesUnion, &ho_LinesCircle, 3);

        PaintRegion(ho_LinesCircle, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
            "fill");
        Imageresult=halcon2opencv(ho_ImageResult);
        Dist_Return = (x3+x4)/2 ;
        Angle_Return = *hv_PreAngle.DArr() *180 / 3.1415926;
//cout<< "Dist ="<<Dist_Return<< "          Angle = "<<Angle_Return <<endl ;

        if(flag == 1)
         return 0 ;

     //   cout<<*hv_wcs_y[0]<< "" <<*hv_wcs_y[1]<<endl;

        //cout<<((const HTuple&)hv_wcs_x)[0]<< " "<< ((const HTuple&)hv_wcs_x)[1]<<endl;
        //cout<<((const HTuple&)hv_wcs_y)[0]<< " "<< ((const HTuple&)hv_wcs_x)[1]<<endl;
      }
      Union1(ho_LinesSelected, &ho_LinesUnion);
      DilationCircle(ho_LinesUnion, &ho_LinesCircle, 3);

      PaintRegion(ho_LinesCircle, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
          "fill");


     // Dist_Return =( *hv_PreDist.DArr()  - sin(*hv_PreAngle.DArr() ) * 240 ) / cos(*hv_PreAngle.DArr() ); //x
        Dist_Return = (x3+x4)/2 ;
        Angle_Return = *hv_PreAngle.DArr() *180 / 3.1415926;
//cout<< "Dist = "<< Dist_Return<<"          Angle = "<<Angle_Return <<endl ;


      Imageresult=halcon2opencv(ho_ImageResult);
      if(flag == 1)
       return 0 ;




   //    cout<<"Angle:  "<< Angle_Return <<"   Dist:  "<< (Dist_Return-320) <<endl;


      //cout<<Imageresult.rows<<"  "<<Imageresult.cols<<endl;

    }

    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        HDevExpDefaultException.ToHTuple(&hv_exception);
    // cout<<hv_exception<<endl ;
        Imageresult=halcon2opencv(ho_Img);
//cout <<"No line Detected"<<endl ;
  return  0;
 //     HDevExpDefaultException.ToHTuple(&hv_exception);
    }
    return 0 ;

}
void CV_End()
{
       CloseAllFramegrabbers();
 }
char img_ho[64];
void Grab_Image()
{
    static int frame = 0;
    static int frame1 = 0;

    Mat img,img1;
    char imgname[64];
    m_cap >> img;

    if(img.empty())
    {
        cout<< "  NI MA BI"<<endl ;
        cout<<"Image Channels ="<<img.channels()<<endl ;
        return;
    }
   sprintf(imgname,"/home/rm12/图片/Image12/img_%.5d.jpg",frame);
 cv::imwrite(imgname,img);
 ho_Img=opencv2halcon(img);
 frame ++ ;
//cout<< "**************************************************************"<<endl ;
//cout<<"Frame = " <<frame1<<endl ;
//sprintf(img_ho,"/home/rm12/图片/Image11/img_%.5d.jpg",frame1);
//img1=imread(img_ho);
//ho_Img=opencv2halcon(img1);
//frame1++ ; /**/
    //cout<< "  NI MA BI"<<endl ;
}
void CV_Show(int state)
{
/*char line[32];
sprintf(line,"line=%.4f,%.4f",Angle_Return,Dist_Return);
Point point_line=Point(400,100);
putText(Imageresult,line,point_line,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,255),2,8);
char point_real1[32];
sprintf(point_real1,"point_real1=(%.4f,%.4f)",* hv_x0.DArr(),* hv_y0.DArr());
Point real_point1=Point(400,150);
putText(Imageresult,point_real1,real_point1,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,255),2,8);
char point_real2[32];
sprintf(point_real2,"point_real2=(%.4f,%.4f)",* hv_x1.DArr(),* hv_y1.DArr());
Point real_point2=Point(400,200);
putText(Imageresult,point_real2,real_point2,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,255),2,8);
*/
 //     cout<<Imageresult.cols<<"     11111"<<endl;
      imshow("result",Imageresult);
      waitKey(10) ;

}

void ExposureChange(int value)
{
    int m_exposure=value;
int m_deviceId=0;
      string video_str = "/dev/video0";
      video_str[10] = '0' + m_deviceId;

   int device = v4l2_open(video_str.c_str(), O_RDWR | O_NONBLOCK);
      if (m_exposure >= 0) {
        // not sure why, but v4l2_set_control() does not work for
        // V4L2_CID_EXPOSURE_AUTO...
        struct v4l2_control c;
        c.id = V4L2_CID_EXPOSURE_AUTO;
        c.value = 1; // 1=manual, 3=auto; V4L2_EXPOSURE_AUTO fails...
        if (v4l2_ioctl(device, VIDIOC_S_CTRL, &c) != 0) {
          cout << "Failed to set... " << strerror(errno) << endl;
        }
        cout << "exposure: " << m_exposure << endl;
        v4l2_set_control(device, V4L2_CID_EXPOSURE_ABSOLUTE, m_exposure*6);
      }

      v4l2_close(device);




}


void Camera_Serial_Close(void)
{

    m_cap.release();
    SerialClose();

}
int  SodukuCheck(void)
{
    float q_[4] ;
  float Euler[4] ;
    HObject ho_Union,ho_ROI_0,ho_ImageMean,ho_Dyn,ho_Dyn_skeleton,ho_Dyn_Connection,ho_RegionFillUp;
  HObject ho_SelectedRegions,ho_TMP_Region,ho_SodukuUnion,ho_UnionClosing,ho_c,ho_RegionDifference,ho_ObjectSelected ;
  HObject ho_WithOutLine ;
    HTuple hv_x,hv_y ,hv_Area,hv_Row,hv_Column,hv_AreaMax, hv_Indices,hv_DistanceMin;

    HTuple hv_Indices_MinIndex, hv_Row_const, hv_Column_const, hv_NumOfPose1,hv_Pose1 ;
   static HTuple hv_Row_pre = 240;
   static HTuple hv_Column_pre = 320;
   float PreSoduku_Dist_Return = -100 ,PreSoduku_Angle_Return=-100,PreSoduku_Vertical_Return=-100 ;
  static int FirstArrivedFlag = 1;
int i ;
        HTuple hv_Row1 ,hv_Column1,hv_Phi,hv_Length1,hv_Length2,hv_CovPose,hv_Error,hv_Pose ;
        HObject ho_Rectangle ,ho_Contours ;
    try
    {
        Decompose3(ho_Img, &ho_R, &ho_G, &ho_B);
      TransFromRgb(ho_R, ho_G, ho_B, &ho_Y, &ho_U, &ho_V, "yuv");
      Compose3(ho_Y, ho_U, ho_V, &ho_Img_yuv);
      MeanImage(ho_V, &ho_ImageMean, 20, 20);
      DynThreshold(ho_ImageMean, ho_V, &ho_Dyn, 15, "dark");
      Skeleton(ho_Dyn, &ho_Dyn_skeleton);
      Connection(ho_Dyn, &ho_Dyn_Connection);

      SelectShape(ho_Dyn_Connection, &ho_SelectedRegions, "area", "and", 200, 200000);
      //Union1(ho_SelectedRegions, &ho_SodukuUnion);

      GenRectangle1(&ho_ROI_0, 0, 10, 2, 630);
      GenRectangle1(&ho_TMP_Region, 10, 638, 470, 640);
      Union2(ho_ROI_0, ho_TMP_Region, &ho_ROI_0);
      GenRectangle1(&ho_TMP_Region, 478, 10, 480, 630);
      Union2(ho_ROI_0, ho_TMP_Region, &ho_ROI_0);
      GenRectangle1(&ho_TMP_Region, 417.911, -0.5, 418.889, -0.5);
      Union2(ho_ROI_0, ho_TMP_Region, &ho_ROI_0);
      GenRectangle1(&ho_TMP_Region, 10, 0, 470, 2);
      Union2(ho_ROI_0, ho_TMP_Region, &ho_ROI_0);


      Union1(ho_SelectedRegions, &ho_Union);
      ClosingCircle(ho_Union, &ho_UnionClosing, 50);
      ConcatObj(ho_UnionClosing, ho_ROI_0, &ho_WithOutLine);

      Union1(ho_WithOutLine, &ho_Union);

      FillUp(ho_Union, &ho_RegionFillUp);
      Difference(ho_RegionFillUp, ho_Union, &ho_RegionDifference);
      Connection(ho_RegionDifference, &ho_c);
      AreaCenter(ho_c, &hv_Area, &hv_Row, &hv_Column);
      cout << "1" <<endl ;

      SmallestRectangle2(ho_c, &hv_Row1, &hv_Column1, &hv_Phi, &hv_Length1, &hv_Length2);
      GenRectangle2(&ho_Rectangle, hv_Row1, hv_Column1, hv_Phi, hv_Length1, hv_Length2);
      GenContourRegionXld(ho_Rectangle, &ho_Contours, "border");
      GetRectanglePose(ho_Contours, hv_CameraParam, 0.46, 0.46, "nonweighted",2, &hv_Pose1, &hv_CovPose, &hv_Error);

      HTuple hv_NumOfPose1 ;
      float Dist = 0 ,MinDist = 10000000 ;
      int MinDistIndex ;
      float X_Delta , Y_Delta ,Dist_Now2Last ;

      hv_NumOfPose1 = hv_Pose1.TupleLength();

      for (i=0;(i<(*hv_NumOfPose1.LArr()/7 ));i++   )
      {
         // X_Delta = hv_Pose1[0 + 7*i] ;
         Dist = hv_Pose1[0 + 7*i] * hv_Pose1[0+7*i]  + hv_Pose1[1+7*i] * hv_Pose1[1+7*i];
    //     cout<<"Dist =  "<<Dist<<endl;
        if( MinDist > Dist )
        {
          MinDistIndex = i ;
          MinDist = Dist ;
        }
      }
      cout <<"MinDistIndex = "<<MinDistIndex<<endl ;
     if( FirstArrivedFlag == 1)  //首发 的情况
         {
             Vertical_Return= hv_Pose1[1 + MinDistIndex * 7] ;
            Dist_Return = hv_Pose1[0+ MinDistIndex * 7]   ;
            FirstArrivedFlag  = 0 ;
               HTuple number ;
            CountObj(ho_Contours,&number);

            SelectObj(ho_Contours, &ho_ObjectSelected,MinDistIndex  +1 );// MinDistIndex+1
            PaintXld(ho_ObjectSelected, ho_Img, &ho_ImageResult, ((HTuple(255).Append(255)).Append(0))) ;
            cout  <<" First Flag"<<endl ;
            Imageresult=halcon2opencv(ho_ImageResult);
         }
     else
         {
            Y_Delta = Vertical_Return -  hv_Pose1[1 + MinDistIndex * 7] ;
            X_Delta = Dist_Return - hv_Pose1[0+ MinDistIndex * 7] ;
            Dist_Now2Last =sqrt( X_Delta *X_Delta + Y_Delta * Y_Delta) ;
            if(Dist_Now2Last >  0.2 )  //  限制 与上次找到的目标的距离 ，如果距离在 0.2米以内则是正常的数据，但是如果距离在限制以外，则可能是天空飘来的一朵云
             {
                Vertical_Return  ;//= hv_Pose1[1 + MinDistIndex * 7] ;
                Dist_Return      ;//= hv_Pose1[0 + MinDistIndex * 7]   ;
                   Imageresult=halcon2opencv(ho_Img);
            }
            else  // 低通他妈的一个
            {
                cout <<"MinIndexDist ="<< MinDistIndex<<endl;
               Vertical_Return = Vertical_Return *0.2 +  hv_Pose1[1 + MinDistIndex * 7]*0.8 ; //Remain
               Dist_Return     = Dist_Return * 0.2 + hv_Pose1[0+ MinDistIndex * 7] * 0.8     ;
                         SelectObj(ho_Contours, &ho_ObjectSelected, MinDistIndex+1);
               //PaintRegion(ho_ObjectSelected, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
                 //  "fill");
               PaintXld(ho_ObjectSelected, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0))) ;
               Imageresult=halcon2opencv(ho_ImageResult);
                         //Remain
            }

         }

     cout<< "Vertical_Return = "<<Vertical_Return<<endl;
     cout<<"Dist_Return ="<<Dist_Return<<endl;

  return 1 ;

    /*   if (0 != (HTuple(hv_Area[0])>0))
       {

          TupleMax(hv_Area, &hv_AreaMax);
          if (0 != (hv_AreaMax>200))
          {
              TupleGenConst(hv_Area.TupleLength(), hv_Row_pre, &hv_Row_const);
               TupleGenConst(hv_Area.TupleLength(), hv_Column_pre, &hv_Column_const);
              DistancePp(hv_Row, hv_Column, hv_Row_const, hv_Column_const, &hv_Distance);

              TupleMin(hv_Distance, &hv_DistanceMin);
              TupleFind(hv_Distance, hv_DistanceMin, &hv_Indices_MinIndex);
              hv_Row_pre = ((const HTuple&)hv_Row)[hv_Indices_MinIndex];
              hv_Column_pre = ((const HTuple&)hv_Column)[hv_Indices_MinIndex];

              SelectObj(ho_c, &ho_ObjectSelected, hv_Indices_MinIndex+1);

                     // 找到目标

              q_[0] =recv_sdk_std_msgs.q.q0;
              q_[1] =recv_sdk_std_msgs.q.q1;
              q_[2] =recv_sdk_std_msgs.q.q2;
              q_[3] =recv_sdk_std_msgs.q.q3;
              q2e(q_, Euler   ) ;
              hv_pose.Clear() ;
              hv_pose[0] = 0 ;
              hv_pose[1] = 0;
              hv_pose[2] =  recv_sdk_std_msgs.pos.height - 0.5 ;
              hv_pose[3] = Euler[1]*180/3.1415926 ;
              hv_pose[4] = Euler[2]*180/3.1415926 ;
              hv_pose[5] = 0 ;
              hv_pose[6] = 0 ;

              ImagePointsToWorldPlane(hv_CameraParam,hv_pose, HTuple(hv_Row[hv_Indices_MinIndex]) , HTuple(hv_Column[hv_Indices_MinIndex]),"m",&hv_wcs_x,&hv_wcs_y);
                cout <<"X ="<<* hv_wcs_x.DArr()<< "   Y="<<* hv_wcs_y.DArr()<<"    Height = "<<recv_sdk_std_msgs.pos.height<<endl ;
                X=* hv_wcs_x.DArr();
                Y=* hv_wcs_y.DArr();

              PaintRegion(ho_ObjectSelected, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
                  "fill");
                Imageresult=halcon2opencv(ho_ImageResult);

              if(abs(*hv_wcs_x.DArr() - Dist_Return) < 0.2 && abs(*hv_wcs_y.DArr()- Vertical_Return)<0.2 )
              {
                  Dist_Return=* hv_wcs_x.DArr();
                  Vertical_Return = * hv_wcs_y.DArr();
              }
              return 1 ;

          }
       }  */
   //       Imageresult=halcon2opencv(ho_Img);
      return 0 ;
    }
    // catch (exception)
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
              Imageresult=halcon2opencv(ho_Img);
      HDevExpDefaultException.ToHTuple(&hv_exception);
      cout<<hv_exception<<endl ;
  //    PaintRegion(ho_Rectangle, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
  //        "fill");
 PaintXld(ho_Contours, ho_Img, &ho_ImageResult, ((HTuple(255).Append(255)).Append(0))) ;
        Imageresult=halcon2opencv(ho_ImageResult);
        HTuple num ;
        ContourPointNumXld(ho_Contours,&num) ;
        cout <<*num.LArr()<<endl;

      return 0 ;
    }


return 0 ;

}
int BlackBoxCheck(void)
{
    HObject BlkBox_regions, ho_LowY  ,ho_RegionOpening ,ho_BLKRegionFillUp ,ho_BLKSelectedRegions ,ho_BLKRecRegionOpening,ho_TarBox   ;
    HObject ho_c,ho_RegionConn;
    HObject  ho_SelectedObj, ho_Outter, ho_Inner, ho_RegionDifference;
    HObject  ho_ImageReduced, ho_Edges, ho_UnionEdges, ho_SelectedXLD;
    HObject  ho_ClosedXLD, ho_SelectedXLD1, ho_Rectangle, ho_Contours;
      HTuple  hv_PoseCov, hv_Error, hv_Row;
     HTuple  hv_Column, hv_Phi, hv_Length1, hv_Length2, hv_Pose;
      HTuple hv_BlkArea, hv_row,hv_column,hv_area,hv_Indices ;
    float q_[4] ;
    float Euler[4] ;
    float PreBlk_Dist_Return = -100 ,PreBlk_Angle_Return=-100,PreBlk_Vertical_Return=-100 ;
    try
    {

      Decompose3(ho_Img, &ho_R, &ho_G, &ho_B);
    TransFromRgb(ho_R, ho_G, ho_B, &ho_Y, &ho_U, &ho_V, "yuv");
    Compose3(ho_Y, ho_U, ho_V, &ho_Img_yuv);

/*    Threshold(ho_Y, &BlkBox_regions, 0, 45);
    Connection(BlkBox_regions, &ho_LowY);
    OpeningRectangle1(ho_LowY, &ho_RegionOpening, 25, 25);
    FillUp(ho_RegionOpening, &ho_BLKRegionFillUp);
    SelectShape(ho_BLKRegionFillUp, &ho_BLKSelectedRegions, "area", "and", 5000, 50000);

    OpeningRectangle1(ho_BLKSelectedRegions, &ho_BLKRecRegionOpening, 20, 20);
    Connection(ho_BLKRecRegionOpening, &ho_c);
    AreaCenter(ho_c, &hv_area, &hv_row, &hv_column);
    TupleMax(hv_area, &hv_BlkArea);
    TupleFind(hv_area, hv_BlkArea, &hv_Indices); */



    Threshold(ho_Y, &BlkBox_regions, 0, 70);
    Connection(BlkBox_regions, &ho_LowY);
    OpeningRectangle1(ho_LowY, &ho_RegionOpening, 25, 25);
    Connection(ho_RegionOpening, &ho_RegionConn);
    FillUp(ho_RegionConn, &ho_BLKRegionFillUp);
    SelectShape(ho_BLKRegionFillUp, &ho_BLKSelectedRegions, "area", "and",10000, 500000);
    OpeningRectangle1(ho_BLKSelectedRegions, &ho_BLKRecRegionOpening, 20, 20);
  AreaCenter(ho_BLKRecRegionOpening, &hv_area, &hv_row, &hv_column);
    TupleMax(hv_area, &hv_BlkArea);
    TupleFind(hv_area, hv_BlkArea, &hv_Indices);

    SelectObj(ho_BLKRecRegionOpening, &ho_SelectedObj, hv_Indices+1);
    DilationCircle(ho_SelectedObj, &ho_Outter, 5);
    ErosionCircle(ho_SelectedObj, &ho_Inner, 10);
    Difference(ho_Outter, ho_Inner, &ho_RegionDifference);
    ReduceDomain(ho_Y, ho_RegionDifference, &ho_ImageReduced);
    EdgesSubPix(ho_ImageReduced, &ho_Edges, "canny", 1, 20, 40);
    UnionAdjacentContoursXld(ho_Edges, &ho_UnionEdges, 10, 1, "attr_keep");
    SelectShapeXld(ho_UnionEdges, &ho_SelectedXLD, "contlength", "and", 401.3,
        1000000);//500
    CloseContoursXld(ho_SelectedXLD, &ho_ClosedXLD);
    SelectShapeXld(ho_ClosedXLD, &ho_SelectedXLD1, "area", "and", 10102.4, 500000);
    SmallestRectangle2Xld(ho_SelectedXLD1, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
        &hv_Length2);
    GenRectangle2(&ho_Rectangle, hv_Row[0], hv_Column[0], hv_Phi, hv_Length1, hv_Length2);
    GenContourRegionXld(ho_Rectangle, &ho_Contours, "border");
    //gen_rectangle2_contour_xld (Rectangle_XLD, Row, Column, Phi, Length1, Length2)
    //gen_region_polygon_xld (Rectangle_XLD, Rectangle_Region, 'filled')

    GetRectanglePose(ho_Contours, hv_CameraParam, 0.48, 0.48, "tukey", 2, &hv_Pose,
        &hv_PoseCov, &hv_Error);
    AreaCenter(ho_Rectangle, &hv_area, &hv_row, &hv_column);




    q_[0] =recv_sdk_std_msgs.q.q0;
    q_[1] =recv_sdk_std_msgs.q.q1;
    q_[2] =recv_sdk_std_msgs.q.q2;
    q_[3] =recv_sdk_std_msgs.q.q3;
    q2e(q_, Euler   ) ;
    hv_pose.Clear() ;
    hv_pose[0] = 0 ;
    hv_pose[1] = 0;
    hv_pose[2] =  recv_sdk_std_msgs.pos.height - 0.5 ;
    hv_pose[3] = Euler[1] * 180 / 3.1415926 ;
    hv_pose[4] = Euler[2] * 180 / 3.1415926;
    hv_pose[5] = 0 ;
    hv_pose[6] = 0 ;

    ImagePointsToWorldPlane(hv_CameraParam,hv_pose, HTuple(hv_row[hv_Indices]) , HTuple(hv_column[hv_Indices]),"m",&hv_wcs_x,&hv_wcs_y);


    if( (PreBlk_Dist_Return!=-100) && PreBlk_Angle_Return != -100 && PreBlk_Vertical_Return != -100)
   {
        if(abs(Blk_Dist_Return - PreBlk_Dist_Return) < 1)
          Blk_Dist_Return=  hv_Pose[0] *0.2 + PreBlk_Dist_Return*0.8     ; // * hv_wcs_x.DArr();
         else
          Blk_Dist_Return = PreBlk_Dist_Return;

        if(abs(Blk_Vertical_Return - PreBlk_Vertical_Return) < 1)
          Blk_Vertical_Return =  hv_Pose[1]*0.2+ PreBlk_Vertical_Return*0.8 ; //* hv_wcs_y.DArr();
        else
          Blk_Vertical_Return = PreBlk_Vertical_Return;

        if(abs(Blk_Angle_Return - PreBlk_Angle_Return) < 1)
          Blk_Angle_Return =  hv_Pose[5]*0.2 + PreBlk_Angle_Return*0.8 ;
          else
          Blk_Angle_Return = PreBlk_Angle_Return;
    }
    else
    {
            Blk_Dist_Return=  hv_Pose[0] ;//*0.2 + PreBlk_Dist_Return*0.8     ; // * hv_wcs_x.DArr();
            Blk_Vertical_Return =  hv_Pose[1];// + PreBlk_Vertical_Return*0.8 ; //* hv_wcs_y.DArr();
            Blk_Angle_Return =  hv_Pose[5];//+ PreBlk_Angle_Return*0.8 ;
    }
    PreBlk_Vertical_Return =  Blk_Vertical_Return ;

 PreBlk_Dist_Return  =  Blk_Dist_Return ;
 PreBlk_Angle_Return =   Blk_Angle_Return;




    PaintRegion(ho_Rectangle, ho_Img, &ho_ImageResult, ((HTuple(255).Append(0)).Append(0)),
        "fill");
      Imageresult=halcon2opencv(ho_ImageResult);

//cout << "BLK 1"<<endl ;
cout << " Dist="<<Blk_Dist_Return<<" Vertical="<<Blk_Vertical_Return<<"  Angle="<<Blk_Angle_Return<<endl ; ;


//PreBlk_Dist_Return = Blk_Dist_Return ;
//PreBlk_Vertical_Return = Blk_Vertical_Return ;
//PreBlk_Angle_Return = Blk_Angle_Return;
    return 1 ;

    }
    // catch (exception)
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        HDevExpDefaultException.ToHTuple(&hv_exception);
   //     cout<<hv_exception<<endl ;

cout << "No BLK"<<endl ;


      return 0 ;
    }

  return 0 ;






}
