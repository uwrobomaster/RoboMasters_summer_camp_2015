
#include "DJI_Imag_Dect.h"
#include <stdio.h>
#include<unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include "DJI_Pro_Test.h"

#include <sys/time.h>
#include <time.h>
#include <pthread.h>

Statues statues =detect_line;//全局设置图像检测的状态位，从而改变图像算法
image_data   drone_image_data;//图像检测返回的数据
gimbal_data  drone_gimbal_data={0,0,1};//定义角度信息 为方格图像检测输入信息
/*图像捕获线程相关变量*/
  pthread_cond_t mycond = PTHREAD_COND_INITIALIZER; /*初始化条件变量*/
  pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
  pthread_mutex_t myimshow = PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
  struct timespec ts;
  int rv;



double tic() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return ((double)t.tv_sec + ((double)t.tv_usec)/1000000.);
}



// Mat srcImage, midImage, dstImage,src_Image2;
 Mat src_Image2;
 Mat src_Image1;
 Mat src_Image;
 vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
 int returntheta, returnrho;
 vector<vector<Point> > contours;
 vector<Vec4i> hierarchy;
 vector<Point> centers;
int para = 0;
static int para_max = 10;

//variables for hists
//Mat v_hist;
//vector<Mat> hsv_planes;
//int returntheta, returnrho;
//float bestrho;
//float besttheta;
//float prerho=0;
//float pretheta=0;
bool cubesDetected=false;
Mat thinImage(Mat srcImage, int maxIterations=-1);


// vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
//vector<Mat> bgr_planes;
//vector<vector<Point> > contours;
//vector<Vec4i> hierarchy;
//Mat srcImage, midImage, dstImage;
//Mat img;
int threshval = 20;
//int arealength;
//vector<Point> centers;



class DJI_Imag_Dect
{

  bool m_timing; // print timing information for each tag extraction call
  int m_width; // image size in pixels
  int m_height;
  int my_fps;
  double m_fx; // camera focal length in pixels
  double m_fy;
  double m_px; // camera principal point
  double m_py;
  int m_deviceId; // camera id (in case of multiple cameras)


  int m_exposure;
  int m_gain;
  int m_brightness;
public:
     bool m_draw; // draw image and April tag detections?
      VideoCapture capture;
public:
    DJI_Imag_Dect()://初始化默认设置
    // default settings, most can be modified through command line options (see below)
    m_draw(false),
    m_timing(false), 

    m_width(320),
    m_height(240),
    my_fps(40),
//    m_tagSize(0.166),
    m_fx(600),
    m_fy(600),
    m_px(m_width/2),
    m_py(m_height/2),

    m_exposure(-1),
    m_gain(-1),
    m_brightness(-1),

    m_deviceId(1)
  {}
  
/*
*输入 i：0 选择摄像头 1选择视频
* 输入 j： 对应的序号    -1默认
*
*/
    void setupVideo(int i ,int  j) {
        #ifdef EXPOSURE_CONTROL
        // manually setting camera exposure settings; OpenCV/v4l1 doesn't
        // support exposure control; so here we manually use v4l2 before
        // opening the device via OpenCV; confirmed to work with Logitech
        // C270; try exposure=20, gain=100, brightness=150
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
        if (m_gain >= 0) {
          cout << "gain: " << m_gain << endl;
          v4l2_set_control(device, V4L2_CID_GAIN, m_gain*256);
        }
        if (m_brightness >= 0) {
          cout << "brightness: " << m_brightness << endl;
          v4l2_set_control(device, V4L2_CID_BRIGHTNESS, m_brightness*256);
        }
    //    v4l2_set_control(device, V4L2_CID_BRIGHTNESS, m_brightness*256);
          v4l2_close(device);
        #endif
        // find and open a USB camera (built in laptop camera, web cam etc)
//        string video_path="/home/exbot/QT_projects/Control/SDK_Zeus/9cube01-7.avi";
        string video_path[3]={"/home/exbot/QT_projects/Control/SDK_Zeus/video1.avi",
                              "/home/exbot/QT_projects/Control/SDK_Zeus/9cube01-7.avi",
                             "/home/exbot/QT_projects/Control/SDK_Zeus/video2.avi"};
        if(i){
        capture = cv::VideoCapture(video_path[j]);
        }else{
        if(j==-1)
             capture = cv::VideoCapture(m_deviceId);
        else
              capture = cv::VideoCapture(j);
        }

        if(!capture.isOpened()) {
        cerr << "ERROR: Can't find video device " << m_deviceId << "\n";
        exit(1);
        }
        else{
            cout<<"open video"<<endl;

        }
        capture.set(CV_CAP_PROP_FRAME_WIDTH, m_width);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_height);
        cout << "Camera successfully opened (ignore error messages above...)" << endl;
        cout << "Actual resolution: "
             << capture.get(CV_CAP_PROP_FRAME_WIDTH) << "x"
             << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    }
     void set_CamParameter( int exposure,int gain,int brightness){
        if ( (exposure!=-1)||(gain!=-1)||(brightness!=-1))//参数不为-1
        {
             if ( (m_exposure!=exposure)||(m_gain!=gain)||(m_brightness!=brightness) )//参数不为当前参数
             {
             m_exposure   =exposure;
             m_gain       =gain;
             m_brightness =brightness;
             effect_parameter();
             }
        }
       
     }
     void set_CamSize( int width,int height){
        if ( (m_width!=width )|| (m_height!=height) )
        {
         m_width  =width;
         m_height =height;
         effect_size();
        }  
     }
     void effect_size(){
         capture.set(CV_CAP_PROP_FRAME_WIDTH, m_width);
         capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_height);
         capture.set(CV_CAP_PROP_FPS, my_fps);
         cout << "Camera successfully opened (ignore error messages above...)" << endl;
         cout << "Actual resolution: "
         << capture.get(CV_CAP_PROP_FRAME_WIDTH) << "x"
         << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
     }
     void effect_parameter(){
         string video_str = "/dev/video0";
         video_str[10]    = '0' + m_deviceId;
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
         if (m_gain >= 0) {
         cout << "gain: " << m_gain << endl;
         v4l2_set_control(device, V4L2_CID_GAIN, m_gain*256);
         }
         if (m_brightness >= 0) {
         cout << "brightness: " << m_brightness << endl;
         v4l2_set_control(device, V4L2_CID_BRIGHTNESS, m_brightness*256);
         }
         v4l2_close(device);
     }
    
};
DJI_Imag_Dect Image;


void on_mouse(int event, int x, int y, int flags, void* ustc ,Mat srcImage,Mat midImage){
    if (event == CV_EVENT_LBUTTONDOWN){
        cout << "src point:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y, x)[1] << " R:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;

    }
    else if (event == CV_EVENT_RBUTTONDOWN){
        cout << "src point:(" << x << ", " << y << "): H:" << (int)midImage.at<Vec3b>(y, x)[0] << " S:" << (int)midImage.at<Vec3b>(y, x)[1] << " V:" << (int)midImage.at<Vec3b>(y, x)[2] << endl;
    }
}
/**
 * [Image_Dect_Ball 检测起飞区和球]
 * @param srcimage 图像数据
 */
  void Image_Dect_Ball(Mat srcimage)
{
     Mat img;
   vector<Point> centers;
    cout<<"Image_Dect_Ball"<<endl;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    Mat srcImage, midImage, dstImage,ballImage;
    int arealength;
    srcImage=srcimage.clone();
    ballImage = srcImage.clone();
    resize(srcImage, srcImage, Size(150, 100));
    cvtColor(srcImage, midImage, CV_BGR2HSV);
    dstImage = midImage.clone();
    for (int i = 0; i < srcImage.rows; i++){
      uchar * pt = srcImage.ptr<uchar>(i);
      uchar * dstpt = dstImage.ptr<uchar>(i);
      for (int j = 0; j < srcImage.cols; j++){
        //detecting blackrectangle.avi
        if (dstpt[2] < 60){
          dstpt[0] = 255;
          dstpt[1] = 0;
          dstpt[2] = 0;
        }
        else{
          dstpt[0] = 0;
          dstpt[1] = 0;
          dstpt[2] = 0;
        }
        pt += 3; dstpt += 3;
      }
    }
    cvtColor(dstImage, img, CV_BGR2GRAY);

    Mat bw = (img> threshval);
    //imshow("bw", bw);
    //find the black rectangle
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
    int maxid = 0; int maxlength = 0;
   if (contours.empty() || hierarchy.empty()) return;
    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
      Mat dst = Mat::zeros(img.size(), CV_8UC1);
      drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);
      Moments mt = moments(dst, true);
      int sizeOfArea = mt.m00 / cos(drone_gimbal_data.roll / 180 * CV_PI) / cos(drone_gimbal_data.pitch / 180 * CV_PI);
      arealength = sqrt(sizeOfArea)*drone_gimbal_data.height;
      if (arealength < 30) continue;
      if (arealength >= maxlength){
        maxid = idx;
        maxlength = arealength;
      }

      Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
      centers.push_back(pt);
      /*cout << "width of area: " << arealength << endl;
      cout << "center: " << centers[centers.size() - 1].x << ", " << centers[centers.size() - 1].y << endl;
      cout << endl;*/
    }
    for (int i = 0; i < centers.size(); i++){
      circle(srcImage, centers[i], 3, Scalar(0, 0, 255), 3);
      circle(bw, centers[i], 3, 50, 3);
    }
    centers.clear();
    //look for balls in the big rectangle
    Mat dst = Mat::zeros(img.size(),CV_8UC1);
    drawContours(dst, contours, maxid, 255, CV_FILLED, 8, hierarchy);
    imshow("the contour",dst);

    Mat showcircle = Mat::zeros(img.size(), CV_8UC1);
    for (int i = 0; i < img.rows; i++){
      uchar* scpt = showcircle.ptr<uchar>(i);
      uchar* bwpt = bw.ptr<uchar>(i);
      uchar* dstpt = dst.ptr<uchar>(i);
      for (int j = 0; j < img.cols; j++){
        if (dstpt[0]==0){
          scpt[0] = 0;
        }
        else if(bwpt[0]!=0){
          scpt[0] = 0;
        }
        else{
          scpt[0] = 255;
        }
        scpt++;
        bwpt++;
        dstpt++;
      }
    }

    int erosion_size = 3;
    Mat element1 = getStructuringElement(MORPH_ELLIPSE,
    Size(2 * erosion_size + 1, 2 * erosion_size + 1),
    Point(erosion_size, erosion_size));

    int dilation_size = 8;
    Mat element2 = getStructuringElement(MORPH_ELLIPSE,
      Size(2 * dilation_size + 1, 2 * dilation_size + 1),
      Point(dilation_size, dilation_size));
    /// Apply the erosion operation
    erode(showcircle, showcircle, element1);
    dilate(showcircle, showcircle, element2); 
    imshow("showcircle", showcircle);
    //////////////////////////////////////////////
    bw = (showcircle> threshval);
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
   if (contours.empty() || hierarchy.empty()) return;
   drone_image_data.centers.clear();
    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
      Mat dst = Mat::zeros(img.size(), CV_8UC1);
      drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);
      Moments mt = moments(dst, true);
      if (mt.m00 < 100) continue;
      Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
      centers.push_back(pt);
      drone_image_data.centers.push_back(pt);
      /*cout << "width of area: " << arealength << endl;
      cout << "center: " << centers[centers.size() - 1].x << ", " << centers[centers.size() - 1].y << endl;
      cout << endl;*/
    }
    for (int i = 0; i < centers.size(); i++){
      circle(srcImage, centers[i], 3, Scalar(255, 0, 0), 3);
      circle(bw, centers[i], 3, 100, 3);
    }
    centers.clear();
    if(Image.m_draw)
    {
        pthread_mutex_lock(&myimshow);
        imshow("Ball_BGR", srcImage);
        pthread_mutex_unlock(&myimshow);
    }
  
//    char a = waitKey(10);
//    if (a == ' '){
//      continue;
//    }
//    else if (a == 'q'){
//      capture.release();
//      break;
//    }
}
/**
 * [Imag_Dect_Square_Function 检测方块]
 *需要启动时传入全局变量drone_gimbal_data中三个参数 height yaw roll
 * @param  color [1 九宫格][0 起飞区方块]
 * @return       [description]
 */
 void  Imag_Dect_Square_Function(int color,Mat srcimage){
        Mat srcImage, midImage, dstImage;
        Mat img;

        int arealength;
        srcImage=srcimage.clone();
        resize(srcImage, srcImage, Size(150, 100));
        cvtColor(srcImage, dstImage, CV_BGR2HSV);

        for (int i = 0; i < srcImage.rows; i++){
            uchar * pt = srcImage.ptr<uchar>(i);
            uchar * dstpt = dstImage.ptr<uchar>(i);

            for (int j = 0; j < srcImage.cols; j++){
              
              if (color==0)
              {
                 if (dstpt[2] < 60){
                    dstpt[0] = 255;
                    dstpt[1] = 0;
                    dstpt[2] = 0;
                }
                else{
                    dstpt[0] = 0;
                    dstpt[1] = 0;
                    dstpt[2] = 0;
                }

              }
            else if(color==1){
                if (pt[2]>180&& (float)pt[0] / (pt[0] + pt[1])>=0.44&&pt[1]-pt[0]<30&&dstpt[1]>65){
                  dstpt[0] = 255;
                  dstpt[1] = 0;
                  dstpt[2] = 0;
                }
                else{
                  dstpt[0] = 0;
                  dstpt[1] = 0;
                  dstpt[2] = 0;
                }
            }
               pt += 3; dstpt += 3;
            }
        }
        cvtColor(dstImage, img, CV_BGR2GRAY);

        int erosion_size = 3;
        Mat element = getStructuringElement(MORPH_ELLIPSE,
            Size(2 * erosion_size + 1, 2 * erosion_size + 1),
            Point(erosion_size, erosion_size));
        /// Apply the erosion operation
        erode(img, img, element);

        Mat bw = (img> threshval);
       if( Image.m_draw)
       {
        pthread_mutex_lock(&myimshow);
        imshow("Square_bw", bw);
       pthread_mutex_unlock(&myimshow);
       }
        findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);

        if (!contours.empty() && !hierarchy.empty())
                {
                    drone_image_data.centers.clear();//每次检测清除返回的中心点
                    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
                    {
                        Mat dst = Mat::zeros(img.size(), CV_8UC1);
                        drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);

                        Moments mt = moments(dst, true);
                        int sizeOfArea = mt.m00 / cos(drone_gimbal_data.roll / 180 * CV_PI) / cos(drone_gimbal_data.pitch / 180 * CV_PI);
                        arealength = sqrt(sizeOfArea*0.4)*drone_gimbal_data.height ;

                       if (arealength <100) continue;

                        Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
                        centers.push_back(pt);
                        drone_image_data.centers.push_back(pt);
                        if(Image.m_draw)
                        {
                        cout << "width of area: " << arealength<< endl;
                        cout << "center: " << centers[centers.size() - 1].x << ", " << centers[centers.size() - 1].y << endl;
                        cout << endl;
                        cout << "drone_image_data.center: " << drone_image_data.centers[drone_image_data.centers.size() - 1].x<< ", " <<  drone_image_data.centers[drone_image_data.centers.size() - 1].y << endl;
                        cout << endl;
                        }


                    }
                }

             //  cout<<"size"<<centers.size()<<endl;
                for (int i = 0; i < centers.size(); i++){

                    circle(srcImage, centers[i],5,Scalar(0,0,255),3);
                    circle(bw, centers[i], 3, 0, 3);
                }
        if(Image.m_draw){
          pthread_mutex_lock(&myimshow);
          imshow("Square_center", srcImage);
          pthread_mutex_unlock(&myimshow);
        }
//        setMouseCallback("center", on_mouse);
            contours.clear();
            hierarchy.clear();
            centers.clear();
            waitKey(10);


    // }

    // return 0;
}

/**
 * [Imag_Dect_Function 线检测入口]
 * @param  arg [description]
 * @return     [description]
 */
  void Imag_Dect_Function( Mat srcimage){//修改为inline 函数？？？？？？
       Mat srcImage, midImage, dstImage;

       srcImage=srcimage.clone();
//       if (srcImage.empty()) return;
        //【1】载入原始图和Mat变量定义
        resize(srcImage, srcImage, Size(150, 100));
        midImage = srcImage.clone();
        cvtColor(midImage, midImage, CV_BGR2HSV);

        for (int i = 0; i < midImage.rows; i++){
            uchar* pt = midImage.ptr<uchar>(i);
            for (int j = 0; j < midImage.cols; j++){
                int imgH = pt[0];
                int imgS = pt[1];
                int imgV = pt[2];
                if (imgV<120 || imgS>60){
                    pt[0] = 0;
                    pt[1] = 0;
                    pt[2] = 0;
                }
                else{
                    pt[0] = 0;
                    pt[1] = 0;
                    pt[2] = 255;
                }
                pt += 3;
            }
        }

        //Color filtered image
        cvtColor(midImage, midImage, CV_HSV2BGR);
        cvtColor(midImage, midImage, CV_BGR2GRAY);

        //erosion for the image
        int erosion_size = 2;
        Mat element = getStructuringElement(MORPH_ELLIPSE,
            Size(2 * erosion_size + 1, 2 * erosion_size + 1),
            Point(erosion_size, erosion_size));
        // Apply the erosion operation
        erode(midImage, dstImage, element);

//        //图像细化
//        threshold(dstImage,dstImage,100,1,CV_THRESH_BINARY);
//        dstImage = thinImage(dstImage,4).clone();
//       // imshow("thined image", dstImage);

        //【3】进行霍夫线变换

        HoughLines(dstImage, lines, 1, CV_PI / 180, 90, 0, 0);
        cvtColor(dstImage, dstImage, CV_GRAY2BGR);
        //【4】依次在图中绘制出每条线段

        float rho, theta;
       if(lines.size()==0)return;
        for (size_t i = 0; i < lines.size(); i++)
        {
            rho = lines[i][0];
            theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * (a));
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * (a));

            if (theta < CV_PI/4){
                returnrho = rho / cos(theta) - 50 * tan(theta)-75;
                returntheta = theta * 180 / CV_PI;
            }
            else if(theta>CV_PI*3/4){
                returnrho = -rho / cos(CV_PI - theta) + 50 * tan(CV_PI - theta)-75;
                returntheta = -(CV_PI - theta) * 180 / CV_PI;
            }
            else{
                continue;
            }
            line(dstImage, pt1, pt2, Scalar(0, 0, 255), 2, CV_AA);

        }
       // cout << "斜率： " << returntheta << " 截距： " << returnrho << endl;


        if(lines.size()>200){
             drone_image_data.bestrho=0;
             drone_image_data.besttheta=0;
        }else{
            drone_image_data.bestrho=returnrho+tan(mye[2]*CV_PI/180)*Act_Hei*deltaCam;
            drone_image_data.besttheta=returntheta;
        }
         cout << "斜率： " << drone_image_data.besttheta << " 截距： " << drone_image_data.bestrho << endl;


//        time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
//       printf("time spent:  %f  ms\n",time);
        if(Image.m_draw){
            pthread_mutex_lock(&myimshow);
         imshow("line_dst",dstImage);
         pthread_mutex_unlock(&myimshow);
        }
//        wrVideo.write(dstImage);
        /* char fileImage[20];
         sprintf(fileImage,"%s/img%d.jpg",filename,i);
         cout<<fileImage<<endl;
        imwrite(fileImage,dstImage);*/
        if (cubesDetected=gethist(dstImage)){
//            cout << "already detected destination!" << endl;
            drone_image_data.bestrho=0;
            drone_image_data.besttheta=0;
            //return 0;
        }

//        if (waitKey(10) == ' ') continue;
    // }
    // capture.release();//释放摄像权限
//    return 0;
}




/**
 * [gethist 判断？？？]
 * @return [description]
 */
 bool gethist(Mat dstImage){
    Mat partimg = dstImage.rowRange(0, dstImage.rows / 2);
    cvtColor(partimg,partimg,CV_BGR2HSV);
    long count = 0;
    for (int i = 0; i < partimg.rows; i++){
        for (int j = 0; j < partimg.cols; j++){
            uchar* pt = partimg.ptr<uchar>(i);
            if (pt[2] == 255) count++;
            pt += 3;
        }
    }
 //   cout << "hist count: " << count << endl;
  //  imshow("part", partimg);
if(count>partimg.cols*partimg.rows*2/3){
//    cout << "hist count: " << count << endl;
    return true;
}
    return false ;
}
/**
 * [检测函数]
 * @param  srcImage      输入的图像数据
 * @param  maxIterations [description]
 * @return               返回图像数据
 */
 Mat thinImage(Mat srcImage, int maxIterations)
{
    Mat dstImage;
    IplImage* src = new IplImage(srcImage);//????
    IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

    CvSize size = cvGetSize(src);
    cvCopy(src, dst);//将src中的内容拷贝到dst中
    int count = 0;	//记录迭代次数
    while (true)
    {
        count++;
        if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达
            break;
        //std::cout << count << ' ';输出迭代次数
        vector<pair<int, int> > mFlag; //用于标记需要删除的点
        //对点标记
        for (int i = 0; i<size.height; ++i)
        {
            for (int j = 0; j<size.width; ++j)
            {
                //如果满足四个条件，进行标记
                //  p9 p2 p3
                //  p8 p1 p4
                //  p7 p6 p5
                int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
                int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
                int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
                int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
                int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
                int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
                int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
                int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
                int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
                {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;

                    if (ap == 1)
                    {
                        if (p2*p4*p6 == 0)
                        {
                            if (p4*p6*p8 == 0)
                            {
                                //标记
                                mFlag.push_back(make_pair(i, j));
                            }
                        }
                    }
                }
            }
        }

        //将标记的点删除
        for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
        {
            CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
        }

        //直到没有点满足，算法结束
        if (mFlag.size() == 0)
        {
            break;
        }
        else
        {
            mFlag.clear();//将mFlag清空
        }

        //对点标记
        for (int i = 0; i<size.height; ++i)
        {
            for (int j = 0; j<size.width; ++j)
            {
                //如果满足四个条件，进行标记
                //  p9 p2 p3
                //  p8 p1 p4
                //  p7 p6 p5
                int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
                if (p1 != 1) continue;
                int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
                int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
                int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
                int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
                int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
                int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
                int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
                int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
                {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;

                    if (ap == 1)
                    {
                        if (p2*p4*p8 == 0)
                        {
                            if (p2*p6*p8 == 0)
                            {
                                //标记
                                mFlag.push_back(make_pair(i, j));
                            }
                        }
                    }
                }
            }
        }
        //删除
        for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
        {
            CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
        }

        //直到没有点满足，算法结束
        if (mFlag.size() == 0)
        {
            break;
        }
        else
        {
            mFlag.clear();//将mFlag清空
        }
    }

    for (int i = 0; i<dst->height; ++i)
    {
        for (int j = 0; j<dst->width; ++j)
        {
            if (CV_IMAGE_ELEM(dst, uchar, i, j) == 1)
                CV_IMAGE_ELEM(dst, uchar, i, j) = 255;
        }
    }

    dstImage = Mat(dst);
    return dstImage;
}

/**
 * [Imag_Dect_Function 检测入口]
 * @param  arg [description]
 * @return     [description]
 */
static void * Imag_Dect(void * arg){
//    waitKey(0);
    Image.setupVideo(0,1);//建立(参数一摄像头0 录像1      ,参数二-1 默认 0-n序号）
//    Image.m_draw=false;//修改显示 false 不显示  true 显示
    Image.m_draw=true;
    Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
    int frame_t = 0;//get process time
    double last_t = tic();
     Image.set_CamSize(160,120);//修改   width,  height不能太小
//      Image.set_CamSize(150,100);//修改   width,  height
     statues=detect_line_ready;//修改初始状态
    statues=detect_9_square_ready;
    statues=detect_1_square_ready;
//    statues=detect_line_square_ready;
//      statues=detect_ball_square_ready;
    for (int i = 0;; )
    {
        if(Image.m_draw){
          frame_t++;//检测处理时间的

          if (frame_t % 10 == 0) {
            double t = tic();
            
            cout << "  " << 10./(t-last_t) << " fps" << endl;
            last_t = t;
          }
        }

        Image.capture>>src_Image2;//讀取是否需要放在另外一個綫程

        waitKey(10);
//        srcImage=  src_Image2.clone();
        switch(statues)
        {   case detect_line_ready://线检测初始设置
            {
                Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
                Image.set_CamSize(150,100);//修改   width,  height
                statues=detect_line;
            }
            break;
            case detect_line://执行线检测算法
            {
                 // Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
                // Image.set_CamSize(640,480);//修改   width,  height
                Imag_Dect_Function(src_Image2);
            }
            break;
            case detect_9_square_ready://九宫格 检测初始设置
            {
                // Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
                // Image.set_CamSize(640,480);//修改   width,  height
                statues=detect_9_square;
            }
            break;
            case detect_9_square://九宫格算法检测
            {
                // Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
                // Image.set_CamSize(640,480);//修改   width,  height
                Imag_Dect_Square_Function(1,src_Image2);
            }
            break;
            case detect_1_square_ready://起飞区 初始设置
            {
            statues=detect_1_square;
            }
            ;break;
            case detect_1_square://起飞区算法检测执行
            {
             Imag_Dect_Square_Function(0,src_Image2);
            }
            ;break;
            case detect_line_square_ready://起飞区 初始设置
            {
            statues=detect_line_square;
            }
            ;break;
            case detect_line_square://起飞区算法检测执行
            {
             Imag_Dect_Function(src_Image2);
             Imag_Dect_Square_Function(0,src_Image2);
            }
            ;break;
            case detect_ball_square_ready://起飞区 初始设置
            {
            statues=detect_ball_square;
            }
            ;break;
            case detect_ball_square://起飞区算法检测执行
            {
            Image_Dect_Ball(src_Image2);
            }
            ;break;

                default:
                 ;break;
            }
    }

}


static void * Imag_Dect_1(void * arg){
  usleep(1000000);
  int frame_t = 0;//get process time
  double last_t = tic();
  cout<<"Imag_Dect_2 open"<<endl;
    while(1)
  {
//        if(Image.m_draw){//输出当前帧率
          if(1){//输出当前帧率
          frame_t++;
          if (frame_t % 10 == 0) {
              double t = tic();
              cout << "Imag_Dect_1                                   =" << 10./(t-last_t) << " fps" << endl;
              last_t = t;
            }
        }
    pthread_mutex_lock(&mymutex);
    src_Image1=src_Image.clone();
//     usleep(10000);
//     cout<<"Imag_Dect_1"<<endl;
    pthread_mutex_unlock(&mymutex);
    Imag_Dect_Function(src_Image1);
     usleep(25000);
   
  }
}
int Start_Dect_1_Thread(void)
{
    int ret;
    pthread_t A_ARR;
    ret = pthread_create(&A_ARR, 0,Imag_Dect_1,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
static void * Imag_Dect_2(void * arg){
  usleep(1000000);
  int frame_t = 0;//get process time
  double last_t = tic();
  cout<<"Imag_Dect_2 open"<<endl;
  while(1)
  {
//      if(Image.m_draw){//输出当前帧率
      if(1){//输出当前帧率
        frame_t++;
        if (frame_t % 10 == 0) {
            double t = tic();
            cout << "Imag_Dect_2                                              " << 10./(t-last_t) << " fps" << endl;
            last_t = t;
          }
      }
    pthread_mutex_lock(&mymutex);
    src_Image2=src_Image.clone();
//    usleep(10000);
//    cout<<"Imag_Dect_2"<<endl;
    pthread_mutex_unlock(&mymutex);
    Imag_Dect_Square_Function(0,src_Image2);
    usleep(25000);
    
  }
}
int Start_Dect_2_Thread(void)
{
    int ret;
    pthread_t A_ARR;
    ret = pthread_create(&A_ARR, 0,Imag_Dect_2,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
  /**
   * 图像检测开始线程入口
   * @param  arg [description]
   * @return     [description]
   */
static void * Imag_Dect_Set(void * arg){
    Image.setupVideo(0,0);  //建立(参数一摄像头0 录像1      ,参数二-1 默认 0-n序号）
    Image.m_draw=true;     //修改imshow显示 false 不显示  true 显示
    Image.set_CamParameter(-1,-1,-1);//修改 exposure, gain, brightness
    Image.set_CamSize(320,240);//设置图像大小 最好为4:3的比例 ！!!!:设置150:100会失败
    int frame_t = 0;//get process time
    double last_t = tic();
    cout<<"Imag_Dect_Set open"<<endl;
    while (1)
    { 
      pthread_mutex_lock(&mymutex);
//      if(Image.m_draw){//输出当前帧率
       if(1){//输出当前帧率
        frame_t++;
        if (frame_t % 10 == 0) {
            double t = tic();
            cout << "捕获图像" << 10./(t-last_t) << " fps" << endl;
            last_t = t;
          }
      }
      Image.capture.read(src_Image);//读取图像 同时克隆到另一个图像中 提供给两个线程使用
      waitKey(2);
//     usleep(10000);
       pthread_mutex_lock(&myimshow);
      imshow("Imag_Dect_Set",src_Image);
       pthread_mutex_unlock(&myimshow);
      pthread_mutex_unlock(&mymutex);
      usleep(35000);//延时 修改图像更新帧率 与硬件相符合
 
      
    }
}
/**
 * 启动捕获图像 将图像捕获 设置帧率为25
 * @return  [description]
 */
int Start_ImagDect_Thread(void)
{
    int ret;
    pthread_t A_ARR;
    ret = pthread_create(&A_ARR, 0,Imag_Dect_Set,NULL);
    if(ret != 0)
    {
        return -1;
    }
    
    Start_Dect_1_Thread();//开启图像检测1
    Start_Dect_2_Thread();//开启图像检测2
    return 0;
}
