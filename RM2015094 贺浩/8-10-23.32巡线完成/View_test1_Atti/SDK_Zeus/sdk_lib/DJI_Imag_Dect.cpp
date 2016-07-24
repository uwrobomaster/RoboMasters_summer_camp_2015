
#include "DJI_Imag_Dect.h"


#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include "DJI_Pro_Test.h"

using namespace std;
using namespace cv;
 image_data   drone_image_data;
vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合

extern float mye[3];\
extern float Act_Hei;
extern float deltaCam;

Mat srcImage, midImage, dstImage;
int para = 0;
static int para_max = 10;
VideoCapture capture;
//variables for hists
Mat v_hist;
vector<Mat> hsv_planes;
int returntheta, returnrho;
float bestrho;
float besttheta;

float prerho;
float pretheta;

void initCam();
void initvideo();
bool gethist();
Mat thinImage(Mat srcImage, int maxIterations=-1);



static void * Imag_Dect_Function(void * arg){
    //initvideo();
    initCam();
prerho=0;
pretheta=0;
    double time = (double)getCPUTickCount();

    for (int i=0;;i++){
        //double time = (double)getCPUTickCount();
        capture >> srcImage;
        if (srcImage.empty()) continue;

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
        if(lines.size()==0)continue;
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


        drone_image_data.bestrho=returnrho+tan(mye[2]*CV_PI/180)*Act_Hei*deltaCam;
        drone_image_data.besttheta=returntheta;
         cout << "斜率： " << drone_image_data.besttheta << " 截距： " << drone_image_data.bestrho << endl;


//        time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
//       printf("time spent:  %f  ms\n",time);

 //     imshow("dst",dstImage);

      //  if (gethist()){
          //  cout << "already detected destination!" << endl;
        //}

        if (waitKey(10) == ' ') continue;
    }
    capture.release();
    return 0;
}


void initCam(){
    capture.open(0);
    if (!capture.isOpened())
        cout << "fail to open!" << endl;

    capture.set(CV_CAP_PROP_FPS, 25);
   // capture.set(CV_CAP_PROP_EXPOSURE, -6);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 150);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 100);
}


void initvideo(){
    capture.open("video.avi");
}

bool gethist(){
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
    cout << "hist count: " << count << endl;
    return true;
}
    return false ;
}

Mat thinImage(Mat srcImage, int maxIterations)
{
    Mat dstImage;
    IplImage* src = new IplImage(srcImage);
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
/*
 * @Function:   Creat a thread to get image data and Dect
 * @Input:
 * @Output:
*/
int Start_ImagDect_Thread(void)
{
    int ret;
    pthread_t A_ARR;
    ret = pthread_create(&A_ARR, 0,Imag_Dect_Function,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
