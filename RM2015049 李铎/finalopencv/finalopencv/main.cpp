//-----------------------------------【头文件包含部分】---------------------------------------
//		描述：包含程序所依赖的头文件
//---------------------------------------------------------------------------------------------- 
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h> 
//-----------------------------------【命名空间声明部分】---------------------------------------
//		描述：包含程序所使用的命名空间
//----------------------------------------------------------------------------------------------- 
using namespace cv;
using namespace std;
//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
Mat srcImage;
Mat midImage, dstImage;
VideoCapture capture;

void mouse_bgr(int event, int x, int y, int flags, void* ustc){
	if (event == CV_EVENT_LBUTTONDOWN){
		cout << "positive BGR:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y,x)[1] << " R:" << (int)srcImage.at<Vec3b>(y,x)[2] << endl;
	}
	if (event == CV_EVENT_RBUTTONDOWN){
		cout << "negtive BGR:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y, x)[1] << " R:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;
	}
}

void mouse_hsv(int event, int x, int y, int flags, void* ustc){
	if (event == CV_EVENT_LBUTTONDOWN){
		cout << "positive HSV:(" << x << ", " << y << "): H:" << (int)midImage.at<Vec3b>(y, x)[0] << " S:" << (int)midImage.at<Vec3b>(y, x)[1] << " V:" << (int)midImage.at<Vec3b>(y, x)[2] << endl;
	}
	if (event == CV_EVENT_RBUTTONDOWN){
		cout << "negtive HSV:(" << x << ", " << y << "): H:" << (int)midImage.at<Vec3b>(y, x)[0] << " S:" << (int)midImage.at<Vec3b>(y, x)[1] << " V:" << (int)midImage.at<Vec3b>(y, x)[2] << endl;
	}
}

int main( )
{
	//capture.open(0);

	double time = (double)getCPUTickCount();
	for (int i = 0;;i++){
		//【1】载入原始图和Mat变量定义   
		//srcImage = imread("saveImage/Image_ 0123.jpg");  //工程目录下应该有一张名为1.jpg的素材图
		//capture >> srcImage;
		char filename[20];
		sprintf(filename, "D:/gaoplane/videopics/cubeImages/img%d.jpg", i);
		srcImage = imread(filename);
		if (srcImage.empty()) continue;
		imshow("bgrimg", srcImage);
		setMouseCallback("bgrimg",mouse_bgr);

		cvtColor(srcImage, midImage, CV_BGR2HSV);
		imshow("hsvimg",midImage);
		setMouseCallback("hsvimg",mouse_hsv);

		waitKey(0);
	}
	//for (int i = 0; i < midImage.rows; i++){
	//	uchar* pt = midImage.ptr<uchar>(i);
	//	for (int j = 0; j < midImage.cols; j++){
	//		int imgH = pt[0];
	//		int imgS = pt[1];
	//		int imgV = pt[2];
	//		if (imgV<150 || imgS>50){
	//			pt[0] = 0;
	//			pt[1] = 0;
	//			pt[2] = 0;
	//		}
	//		pt += 3;
	//	}
	//}

	////Color filtered image
	////namedWindow("color filtered");
	////setMouseCallback("color filtered",on_mouse);
	//cvtColor(midImage, midImage, CV_HSV2BGR);
	//cvtColor(midImage, midImage, CV_BGR2GRAY);
	////imshow("color filtered", srcImage);

	////erosion for the image
	//int erosion_size = 1;
	//Mat element = getStructuringElement(MORPH_ELLIPSE,
	//	Size(2 * erosion_size + 1, 2 * erosion_size + 1),
	//	Point(erosion_size, erosion_size));
	///// Apply the erosion operation
	//erode(midImage, dstImage, element);
	//namedWindow("erosed image");
	//imshow("erosed image", dstImage);

	////【2】进行边缘检测和转化为灰度图
	////Canny(srcImage, midImage, 50, 200, 3);//进行一此canny边缘检测
	////cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
	//
	////【3】进行霍夫线变换
	//vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	//HoughLines(dstImage, lines, 1, CV_PI/180, 90, 0, 0 );
	//cvtColor(dstImage,dstImage,CV_GRAY2BGR);
	////【4】依次在图中绘制出每条线段
	//for( size_t i = 0; i < lines.size(); i++ )
	//{
	//	float rho = lines[i][0], theta = lines[i][1];
	//	//if (abs((int)rho%180)>15) continue;
	//	cout << "斜率： " << abs((int)rho % 180) << endl;
	//	Point pt1, pt2;
	//	double a = cos(theta), b = sin(theta);
	//	double x0 = a*rho, y0 = b*rho;
	//	pt1.x = cvRound(x0 + 1000*(-b));
	//	pt1.y = cvRound(y0 + 1000*(a));
	//	pt2.x = cvRound(x0 - 1000*(-b));
	//	pt2.y = cvRound(y0 - 1000*(a));
	//	line( dstImage, pt1, pt2, Scalar(0,0,255), 2, CV_AA);
	//}
	//imshow("dstImg",dstImage);
	//cout << "num of lines:" << lines.size() << endl;
	//time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
	//cout << "time2： " << time << endl;

	//waitKey(10);  

	return 0;  
}