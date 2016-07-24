#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include<opencv2/legacy/legacy.hpp>
#include "opencv2/nonfree/features2d.hpp"
#define ph(i,j) ph[((i)*width)+(j)]
#define ps(i,j) ps[((i)*width)+(j)]
#define pv(i,j) pv[((i)*width)+(j)]
#define gray(i,j) gray.data[((i)*width)+(j)]
#define UNKNOWN_FLOW_THRESH 1e9
using namespace cv;
using namespace std;
int getrect(Mat src, CvPoint* result);
void salt(Mat im,Mat im2)
{
	Mat gray;
	//im = imread("C:\\Users\\dji\\Desktop\\1.jpg");
	//Mat im2 = imread("C:\\Users\\dji\\Desktop\\2.jpg");
	cvtColor(im, gray, CV_RGB2GRAY);
	Mat hsv1,hsv2;
	Mat corner;
	Mat gray2;
	Mat dst;
	cvtColor(im, hsv1, CV_RGB2HSV);
	cvtColor(im2, hsv2, CV_RGB2HSV);
	//imshow("sss", hsv);	

	vector<cv::KeyPoint>key1,key2;
	cv::SurfFeatureDetector surf(8000);
		surf.detect(im, key1);
		surf.detect(im2, key2);
	
	SurfDescriptorExtractor surfdesc;
	Mat desc1,desc2;
	surfdesc.compute(im, key1, desc1);
	surfdesc.compute(im2, key2, desc2);
	vector<DMatch>matches;
	BruteForceMatcher<cv::L2<float>>matcher;
	matcher.match(desc1, desc2, matches);
	std::nth_element(matches.begin(),    // initial position
		matches.begin() + 24, // position of the sorted element
		matches.end());     // end position
	// remove all elements after the 25th
	matches.erase(matches.begin() + 25, matches.end());
	for (int i = 0; i < 9; i++)
	{
		if (matches[i].distance < 0.1)
			cout << "wow" << endl;
		//	matches.erase(matches.begin() + i);
	}
	cv::Mat imageMatches;

	drawKeypoints(im, key1, im, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(im2, key2, im2, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::drawMatches(im, key1,  // 1st image and its keypoints
		im2, key2,  // 2nd image and its keypoints
		matches,			// the matches
		imageMatches,		// the image produced
		cv::Scalar(255, 255, 255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches", imageMatches);


	
	//imshow("sase", im);
	waitKey(100);
}

void makecolorwheel(vector<Scalar> &colorwheel)
{
	int RY = 15;
	int YG = 6;
	int GC = 4;
	int CB = 11;
	int BM = 13;
	int MR = 6;

	int i;

	for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255, 255 * i / RY, 0));
	for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255 - 255 * i / YG, 255, 0));
	for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0, 255, 255 * i / GC));
	for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0, 255 - 255 * i / CB, 255));
	for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255 * i / BM, 0, 255));
	for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255, 0, 255 - 255 * i / MR));
}

void motionToColor(Mat flow, Mat &color)
{
	if (color.empty())
		color.create(flow.rows, flow.cols, CV_8UC3);

	static vector<Scalar> colorwheel; //Scalar r,g,b
	if (colorwheel.empty())
		makecolorwheel(colorwheel);

	// determine motion range:
	float maxrad = -1;

	// Find max flow to normalize fx and fy
	for (int i = 0; i < flow.rows; ++i)
	{
		for (int j = 0; j < flow.cols; ++j)
		{
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);
			float fx = flow_at_point[0];
			float fy = flow_at_point[1];
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
				continue;
			float rad = sqrt(fx * fx + fy * fy);
			maxrad = maxrad > rad ? maxrad : rad;
		}
	}

	for (int i = 0; i < flow.rows; ++i)
	{
		for (int j = 0; j < flow.cols; ++j)
		{
			uchar *data = color.data + color.step[0] * i + color.step[1] * j;
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);

			float fx = flow_at_point[0] / maxrad;
			float fy = flow_at_point[1] / maxrad;
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
			{
				data[0] = data[1] = data[2] = 0;
				continue;
			}
			float rad = sqrt(fx * fx + fy * fy);

			float angle = atan2(-fy, -fx) / CV_PI;
			float fk = (angle + 1.0) / 2.0 * (colorwheel.size() - 1);
			int k0 = (int)fk;
			int k1 = (k0 + 1) % colorwheel.size();
			float f = fk - k0;
			//f = 0; // uncomment to see original color wheel

			for (int b = 0; b < 3; b++)
			{
				float col0 = colorwheel[k0][b] / 255.0;
				float col1 = colorwheel[k1][b] / 255.0;
				float col = (1 - f) * col0 + f * col1;
				if (rad <= 1)
					col = 1 - rad * (1 - col); // increase saturation with radius
				else
					col *= .75; // out of range
				data[2 - b] = (int)(255.0 * col);
			}
		}
	}
}

int main()
{
	Mat pre, next;

	Mat prevgray, gray, flow, cflow, frame;
	Mat motion2color;

	vector<Point2f> prepoint, nextpoint;
	vector<uchar> state;
	vector<float>err;
	VideoCapture cap("C:\\Users\\dji\\Desktop\\1.5.avi");
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "视频" << totalFrameNumber << "帧" << endl;
	for (;;)
	{
		double t = (double)cvGetTickCount();
		cap >> frame;
		resize(frame,frame,Size(frame.cols/ 2, frame.rows/ 2));
		if (frame.empty())
			break;
		cvtColor(frame, gray, CV_BGR2GRAY);
		//imshow("original", frame);

		if (prevgray.data)
		{
			calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			//motionToColor(flow, motion2color);
			float vx=0, vy=0;
			for (int i = -3; i < 3; i++)
				for (int j = -3; j < 3; j++)
				{
					vx += flow.at<Vec2f>(frame.cols / 2 + 3 * i, frame.rows / 2 + 3 * j)[0];
					vy += flow.at<Vec2f>(frame.cols / 2 + 3 * i, frame.rows / 2 + 3 * j)[1];
				}
			line(frame, Point((int)frame.cols / 2, (int)frame.rows / 2), 
				Point((int)frame.cols / 2 +vx/2, (int)frame.rows / 2 +vy/2), Scalar::all(255), 2);
			imshow("flow",frame);
		}
		if (waitKey(10) >= 0)
			break;
		std::swap(prevgray, gray);

		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
	}
		//	cvtColor(frame, next, CV_BGR2GRAY);
	//	if (!next.empty() && !pre.empty())
	//	{
	//		cv::SurfFeatureDetector surf(1000);
	//		vector<cv::KeyPoint>Keypoints;
	//		surf.detect(pre, Keypoints);
	//		for (size_t i = 0; i < Keypoints.size(); i++) {
	//			prepoint.push_back(Keypoints[i].pt);
	//		}
	//		//goodFeaturesToTrack(pre, prepoint, 500, 0.001, 10, Mat(), 3, false, 0.04);
	//		
	//		cornerSubPix(pre, prepoint, Size(10, 10), Size(-1, -1), TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
	//		calcOpticalFlowPyrLK(pre, next, prepoint, nextpoint, state, err, Size(35, 35), 1);
	//		for (int i = 0; i < state.size(); i++)
	//		{
	//			if (state[i] != 0)
	//			{
	//				line(frame, Point((int)prepoint[i].x, (int)prepoint[i].y), Point((int)nextpoint[i].x, (int)nextpoint[i].y), Scalar::all(255),2);
	//			}
	//		}
	//		drawKeypoints(frame, Keypoints, frame, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//		imshow("frame", frame);
	//		waitKey(1);
	//	}
	//	next.copyTo(pre);
	//}


	////VideoCapture capture(0);
	VideoCapture capture("C:\\Users\\dji\\Desktop\\1.5.avi");
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "视频" << totalFrameNumber << "帧" << endl;
	
	CvPoint re[255];
	Mat frame;
	//Mat frame = imread("C:\\Users\\dji\\Desktop\\tt.jpg");
	int newmark = 0;
	Point pt1, pt2;
	float linerem[2] = { 0, 0 };
	capture >> frame;
	while (1)
	{
		Mat rem;
		frame.copyTo(rem);
		capture >> frame;
		salt(frame,rem);
	//frame = imread("C:\\Users\\dji\\Desktop\\tt.jpg");
		//resize(frame, frame,Size(3*frame.cols,3*frame.rows));
		if (frame.empty())
		{
			cout << "img1 empty error";
			return -1;
		}
		//1 预处理
		Mat gray, sgray,vgray,vgray2;
		Mat binary;
		Mat canny;

		Mat  hsv;//临时变量和目标图的定义

		cvtColor(frame, hsv, CV_RGB2HSV);
		//imshow("sss", hsv);
		vector<Mat> hsv_sep;

		split(hsv, hsv_sep);

		uchar *ph = hsv_sep[0].data;
		uchar *ps = hsv_sep[1].data;
		uchar *pv = hsv_sep[2].data;
		int height = frame.rows;
		int width = frame.cols;
		//adaptiveThreshold(hsv_sep[1], gray, 1, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 99, 7);//二值化
		threshold(hsv_sep[1], sgray, 50, 255, CV_THRESH_BINARY_INV);
		threshold(hsv_sep[2], vgray,140, 255, CV_THRESH_BINARY);
		threshold(hsv_sep[2], vgray2, 80, 255, CV_THRESH_BINARY);

		//imshow("sgray", sgray);
		//imshow("vgray", vgray);
		
		gray = sgray&vgray;
		Mat element(3, 3, CV_8U, Scalar(1));
		//morphologyEx(gray, gray, MORPH_OPEN, element, Point(-1, -1), 3);//闭运算
		erode(gray, gray, element);
		//imshow("gray", gray);
		//hsv_sep[1].copyTo(gray);
		gray.copyTo(canny);
		////【2】进行边缘检测和转化为灰度图
		//uchar* pp = canny.data;
		////转换成灰度 （1变255）
		//for (int i = 0; i<canny.rows; ++i)
		//{
		//	for (int j = 0; j<canny.cols; ++j)
		//	{
		//		if (pp[((i)*canny.cols) + (j)] == 1)
		//			pp[((i)*canny.cols) + (j)] = 255;
		//		//else
		//		//	pp[((i)*canny.cols) + (j)] = 255;
		//	}
		//}

		Canny(canny, canny, 50, 200, 3);//进行一此canny边缘检测
		//【5】显示原始图  fdd
		//imshow("canny", canny);
		//【3】进行霍夫线变换
		
		
		vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
		HoughLines(canny, lines, 1, CV_PI / 180, canny.rows / 4, 0, 0);

		int marker1;
		marker1 = 0;
		//【4】依次在图中绘制出每条线段
		double a1 = cos(linerem[1]), b1 = sin(linerem[1]);
		double x01 = a1*linerem[0], y01 = b1*linerem[0];
		
		marker1++;

		pt1.x = cvRound(x01 + 1000 * (-b1));
		pt1.y = cvRound(y01 + 1000 * (a1));
		pt2.x = cvRound(x01- 1000 * (-b1));
		pt2.y = cvRound(y01 - 1000 * (a1));
		//line(frame, pt1, pt2, Scalar(0, 255, 255), 5, CV_AA);

		int mindetheta =100;
		int mint = 0;
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			//marker1++;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			if (theta>CV_PI * 7 / 8 || theta<CV_PI / 8)
			//line(frame, pt1, pt2, Scalar(255, 0, 255), 5, CV_AA);

			if (newmark == 0 && theta<CV_PI*1.4&&theta>CV_PI / 1.4)
			{
				cout << rho << "     " << theta << endl;
				linerem[0] = lines[i][0];
				linerem[1] = lines[i][1];
				//line(frame, pt1, pt2, Scalar(0, 0, 255), 5, CV_AA);
				newmark = 1;
				break;
			}
			if (abs(theta - linerem[1]) < mindetheta )
			{
				mint = i;
				mindetheta = abs(theta - linerem[1]);
			}
			else  if (abs(theta - linerem[1] + CV_PI) < mindetheta)
			{
				mint = i;
				mindetheta = abs(theta - linerem[1]-CV_PI);
			}
		}
		if (newmark == 1)
		{
			//if ((theta<(linerem[1] + CV_PI / 5) || (theta<(linerem[1] + CV_PI / 5 - CV_PI)))
			//&& ((theta>linerem[1] - CV_PI / 5) || (theta>(linerem[1] - CV_PI / 5 + CV_PI)))) // && rho<linerem[0] + frame.rows*0.5&&rho>linerem[0] - frame.rows*0.5)
			float rho = lines[mint][0], theta = lines[mint][1];
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			//marker1++;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			{
			//	line(frame, pt1, pt2, Scalar(0, 0, 255), 5, CV_AA);
				linerem[0] = lines[mint][0];
				linerem[1] = lines[mint][1];
			}
			//line(frame, pt1, pt2, Scalar(0, 0, 255), 5, CV_AA);
		}
		int i;
		//for (i = 0; i < getrect(frame,re); i++)
		//{
		//	circle(frame, result[i], 10, Scalar(255, 0, 0), 2);
		//	cout << i << ":   " << re[i].x << "     " << re[i].y << endl;

		//}
		int a;
		a = getrect(vgray2, re);
		for (i = 0; i < a; i++)
		{
			circle(frame, re[i], 10, Scalar(0, 255, 255), 10);
			//cout << i << ":   " << re[i].x << "     " << re[i].y << endl;

		}
		//imshow("【效果图】", frame);

		waitKey(20);

		//getrect(frame, re);
		//waitKey(0);
		//waitKey(10);
	}

	return 0;
}

int getrect(Mat src,CvPoint* result)
{
	//1 预处理
	Mat gray;
	Mat binary;
	Mat canny;
	//cvtColor(src, gray, CV_RGB2GRAY); //frame转灰度图gray
	//adaptiveThreshold(gray, binary, 1, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 55, 7);//二值化
	//threshold(gray, binary, 70, 255, CV_THRESH_BINARY);
	//medianBlur(binary, binary, 3); //中值滤波
	//imshow("src", src);
	//2 轮廓预处理
	src.copyTo(canny);
	//IplConvKernel* mykernel_1R = cvCreateStructuringElementEx(5,5,3,3,CV_SHAPE_RECT,NULL);
	//cvMorphologyEx(img_binary,img_binary,NULL,mykernel_1R,CV_MOP_CLOSE,3);
	//Mat element(3, 3, CV_8U, Scalar(1));
	//Mat element = getStructuringElement(CV_SHAPE_RECT, Size(3,3), Point(1,1));
	//morphologyEx(canny, canny, MORPH_CLOSE, element, Point(-1, -1), 3);//闭运算
	//Sobel(canny, canny, CV_16S, 1, 1);

	//uchar* pp = canny.data;
	////转换成灰度 （1变255）
	//for (int i = 0; i<canny.rows; ++i)
	//{
	//	for (int j = 0; j<canny.cols; ++j)
	//	{
	//		if (pp[((i)*canny.cols) + (j)] == 1)
	//			pp[((i)*canny.cols) + (j)] = 255;
	//		//else
	//		//	pp[((i)*canny.cols) + (j)] = 255;
	//	}
	//}
	//imshow("binary", canny);
	Canny(canny, canny, 50, 100, 3);
	//imshow("canny", canny);

	//3 提取轮廓/匹配轮廓
	//提取轮廓
	vector<vector<Point>> contours;//返回轮廓的容器
	vector<Vec4i> hierarchy;//用于存储轮廓

	//findContours(canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//检索所有的轮廓：重构嵌套轮廓的整个层，压缩水平的、垂直的和斜的部分，只保留他们的终点部分，偏移量为0
	findContours(canny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//检索所有的轮廓：重构嵌套轮廓的整个层，压缩水平的、垂直的和斜的部分，只保留他们的终点部分，偏移量为0

	//查找轮廓
	//除去太短的轮廓     
	int lmin = canny.rows / 12;//最小宽度
	int amin = (int)(canny.rows * canny.cols * 0.0003);//最小面积
	int amax = (int)(canny.rows * canny.cols * 0.01);//最大面积
	
	//vector<vector<Point>>::const_iterator itc = contours.begin();
	//while (itc != contours.end())
	//{
	//	if ((itc->size()) < lmin || (itc->size()) > lmax)
	//	{
	//		itc = contours.erase(itc);
	//	}
	//	else ++itc;
	//}

	vector<Rect>boundRect(contours.size());
	int num = 0;
	Mat drawing = Mat::zeros(canny.size(), CV_8UC3);

	//double min_match = 5;
	//int match_num = -1;

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area < amin)//去除面积小的轮廓
			continue;
		//if (area > amax)//去除面积大的轮廓
		//	continue;
		Moments moment;
		double hu[7];
		moment = moments(contours[i], false);
		HuMoments(moment, hu); //计算hu不变矩
		int j;
		vector<std::vector<cv::Point>>::iterator itc2;

		////查找正方形
		if (((hu[0] < 0.1666*1.7) && (hu[0]> 0.16666 / 1.7)))// &&
	//	(hu[1] < 5e-06 * 100000) && (hu[1] >5e-6 / 100000))

		//0.408137     0.0244482     0.0566028
		//0.313063     0.0197965     0.00708513
		//查找二维码
		//if (
		//	((hu[0] < 0.402137*1.1) && (hu[0]>0.402137 / 1.1)
		//	&& (hu[1] < 0.0244482 * 1000) && (hu[1] >0.0244482 / 1000)
		//	&& (hu[2] < 0.0566028 * 100000) && (hu[2] >0.0566028 / 100000))
		//	|| ((hu[0] < 0.288163*1.1) && (hu[0]>0.283063 / 1.1) &&
		//	(hu[1] < 0.0197965 * 1000) && (hu[1] > 0.0197965 / 1000)
		//	&& (hu[2] < 0.00708513 * 100000) && (hu[2] > 0.00708513 / 100000)))
		//if (1)
		{

			Scalar color = Scalar(255, 255, 0);
			//float sumx, sumy;
			//sumx = 0, sumy = 0;

			//for (j = 0; j<contours[i].size(); j++)
			//{
			//	sumx += contours[i][j].x;
			//	sumy += contours[i][j].y;
			//}
			//circle(drawing, cvPoint(sumx / contours[i].size(), sumy / contours[i].size()), 5, color, 2);
			boundRect[num] = boundingRect(Mat(contours[i]));
			if (num != 0 && (abs(result[num - 1].x - (boundRect[num].x + boundRect[num].width / 2)) < 2) &&
				(result[num - 1].y - (boundRect[num].y + boundRect[num].height / 2) < 2))
				continue;
			else
			{
				result[num].x = boundRect[num].x + boundRect[num].width / 2;
				result[num].y = boundRect[num].y + boundRect[num].height / 2;
				//cout << i << ":   " << hu[0] << "     " << hu[1] << "     " << hu[2] << endl;
				//cout << i << ":   " << result[num].x << "     " << result[num].y << endl;
			}
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, result[num], 5, Scalar(255, 0, 0), 2);
			rectangle(drawing, boundRect[num], Scalar(0, 255, 0));
			//imwrite("abc.bmp", drawing);
			//imshow("123", drawing);
			num++;
			//waitKey(0);
		}
		else
		{
			//drawContours( drawing, contours, i, Scalar( 0,255,0 ), 2, 8, hierarchy, 0, Point() );
			//imshow("123",drawing);
			//waitKey(2);
		}

		
	}
	//waitKey(0);
	//cout << "num" << num << endl;
	return num;
	//imshow("contours", frame);

}
//int getball()
//{
//	Mat midImage;
//	cvtColor(golf, midImage, CV_BGR2GRAY);
//	cvtColor(golf, golf2, CV_RGB2HSV);
//
//	GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);
//	//waitKey(0);
//	vector<Vec3f> circles;
//	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, 1, 5, 80, 30, 2, 40);
//
//
//	for (size_t i = 0; i < circles.size(); i++)
//	{
//		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//		int radius = cvRound(circles[i][2]);
//
//		circle(golf, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//
//		circle(golf, center, radius, Scalar(155, 50, 255), 3, 8, 0);
//	}
//	imshow("aaa", golf);
//
//
//
//	vector<Mat> mv;
//
//	split(golf2, mv);
//
//	uchar *ph = H.data;
//	uchar *ps = S.data;
//	uchar *pv = V.data;
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//			pv(i, j) = table[ph(i, j)][ps(i, j)][pv(i, j)];
//	}
//	merge(mv, golf2);
//	cvtColor(golf2, golf2, CV_HSV2RGB);
//	cvtColor(golf2, midImage, CV_RGB2GRAY);
//
//	int morph_elem = 2;
//	int operation = 0;
//	int morph_size = 4;
//	//Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat
//	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
//	//"Element:\n 0: Rect - 1: Cross - 2: Ellipse"
//
//	/// Apply the specified morphology operation
//	morphologyEx(midImage, midImage, operation, element);
//
//
//	//medianBlur(midImage, midImage, 7);
//	GaussianBlur(midImage, midImage, Size(21, 21), 2, 2);
//
//	//waitKey(0);
//	vector<Vec3f> circles2;
//	HoughCircles(midImage, circles2, CV_HOUGH_GRADIENT, 1.5, 100, 100, 50, 20, 200);
//	for (size_t i = 0; i < circles2.size(); i++)
//	{
//		Point center2(cvRound(circles2[i][0]), cvRound(circles2[i][1]));
//		int radius = cvRound(circles2[i][2]);
//		if (ps(cvRound(circles2[i][1]), cvRound(circles2[i][0])) < 40)
//		{
//			circle(golf2, center2, 3, Scalar(0, 255, 0), -1, 8, 0);
//			circle(golf2, center2, radius, Scalar(155, 50, 255), 3, 8, 0);
//		}
//
//	}
//
//
//	//imshow("cc", golf2);
//}