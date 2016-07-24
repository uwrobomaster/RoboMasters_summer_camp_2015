#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

vector<Vec2f> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������

Mat srcImage, midImage, dstImage;
int para = 0;
static int para_max = 10;
VideoCapture capture;
//variables for hists
Mat v_hist;
vector<Mat> hsv_planes;
int returntheta, returnrho;
int besttheta, bestrho;

Mat thinImage(Mat srcImage, int maxIterations = -1);
void changeCamMode(int mode);
void on_mouse(int event, int x, int y, int flags, void* ustc);
void initCam();
static void on_trackbar(int, void*);
void initTrackBar();
void initvideo();
bool gethist();
int main(){
	initvideo();
	//initCam();

	//initTrackBar();

	double time = (double)getCPUTickCount();
	int curMode = 0;
	for (int i=0;;i++){
		//double time = (double)getCPUTickCount();
		capture >> srcImage;
		char filename[50];
		sprintf(filename,"D:/gaoplane/videopics/9cubes-6-3/img%4d.jpg",i);
		//srcImage=imread(filename);
		if (srcImage.empty()) continue;

		//��1������ԭʼͼ��Mat��������   
		resize(srcImage, srcImage, Size(150, 100));
		midImage = srcImage.clone();
		cvtColor(midImage, midImage, CV_BGR2HSV);

		imshow("original", srcImage);
		setMouseCallback("original", on_mouse);

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

		//setMouseCallback("color filtered",on_mouse);
		cv::cvtColor(midImage, midImage, CV_HSV2BGR);
		cv::cvtColor(midImage, midImage, CV_BGR2GRAY);
		imshow("mid",midImage);

		//erosion for the image
		int erosion_size = 5;
		Mat element = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));
		/// Apply the erosion operation
		erode(midImage, dstImage, element);
		//imshow("erosed image", dstImage);
		threshold(dstImage, dstImage, 100, 1, CV_THRESH_BINARY);
		//dstImage = thinImage(dstImage).clone();
		//imshow("thined image", dstImage);
		
		////��2�����б�Ե����ת��Ϊ�Ҷ�ͼ
		////Canny(srcImage, midImage, 50, 200, 3);//����һ��canny��Ե���
		////cvtColor(midImage,dstImage, CV_GRAY2BGR);//ת����Ե�����ͼΪ�Ҷ�ͼ
		//
		//��3�����л����߱任
		
		HoughLines(dstImage, lines, 1, CV_PI / 180, 40, 0, 0);
		cvtColor(dstImage, dstImage, CV_GRAY2BGR);
		//��4��������ͼ�л��Ƴ�ÿ���߶�


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
		cout << "lines.size: " << lines.size() << endl;
		//cout << "б�ʣ� " << returntheta << " �ؾࣺ " << returnrho << endl;

		/*float rho, theta;
		float distance = 60;
		float tmptheta = besttheta;
		float tmprho = bestrho;
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

			if (theta < CV_PI / 4){
				returnrho = rho / cos(theta) - 50 * tan(theta) - 75;
				returntheta = theta * 180 / CV_PI;
			}
			else if (theta>CV_PI * 3 / 4){
				returnrho = -rho / cos(CV_PI - theta) + 50 * tan(CV_PI - theta) - 75;
				returntheta = -(CV_PI - theta) * 180 / CV_PI;
			}
			else{
				continue;
			}
			if (distance>abs(returnrho - bestrho) + abs(returntheta - besttheta)){
				distance = abs(returnrho - bestrho) + abs(returntheta - besttheta);
				tmptheta = returntheta;
				tmprho = returnrho;
				line(dstImage, pt1, pt2, Scalar(0, 0, 255), 2, CV_AA);
			}
		}
		besttheta = tmptheta;
		bestrho = tmprho;
		cout << "б�ʣ� " << besttheta << " �ؾࣺ " << bestrho << endl;*/

		imshow("dstImg", dstImage);
		//cout << "num of lines:" << lines.size() << endl;
		//time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
		//cout << "time�� " << time << endl;

		if (gethist()){
			cout << "already detected destination!" << endl;
		}

		if (waitKey(10) == ' ') continue;
	}
	capture.release();
	waitKey(0);
	return 0;
}



void changeCamMode(int mode){
	if (mode == 0){
		capture.set(CV_CAP_PROP_EXPOSURE, -6);
	}
	else if (mode == 1){
		capture.set(CV_CAP_PROP_EXPOSURE, -7);
	}
	else{
		capture.set(CV_CAP_PROP_EXPOSURE, -11);
	}
}


void on_mouse(int event, int x, int y, int flags, void* ustc){
	if (event == CV_EVENT_LBUTTONDOWN){
		cout << "src point:(" << x << ", " << y << "): H:" << (int)srcImage.at<Vec3b>(y, x)[0] << " S:" << (int)srcImage.at<Vec3b>(y, x)[1] << " V:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;
		//cout << "src point:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y, x)[1] << " R:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;
	}
}


void initCam(){
	capture.open(0);
	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	capture.set(CV_CAP_PROP_FPS, 25);
	capture.set(CV_CAP_PROP_EXPOSURE, -6);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 150);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 100);
}


static void on_trackbar(int, void*)
{
	//parameter settings
	capture.release();
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 150);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 100);
	capture.set(CV_CAP_PROP_FPS, 25);

	capture.set(CV_CAP_PROP_EXPOSURE, para - 10);
}


void initTrackBar(){
	//set parameter adjustment interface
	namedWindow("parameter settings", CV_NORMAL);
	char TrackbarName[50];
	sprintf(TrackbarName, "parameter to set %d", para);
	createTrackbar(TrackbarName, "parameter settings", &para, para_max, on_trackbar);
}

void initvideo(){
	//capture.open("video.avi");
	capture.open("D:/gaoplane/videopics/9cubedetecting-6.avi");
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
	cout << "hist count: " << count << endl;
	imshow("part", partimg);

	return count>partimg.cols*partimg.rows*2/3;
}

Mat thinImage(Mat srcImage, int maxIterations)
{
	Mat dstImage;
	IplImage* src = &IplImage(srcImage);
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

	CvSize size = cvGetSize(src);
	cvCopy(src, dst);//��src�е����ݿ�����dst��
	int count = 0;	//��¼��������
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //���ƴ������ҵ�����������
			break;
		//std::cout << count << ' ';�����������
		vector<pair<int, int> > mFlag; //���ڱ����Ҫɾ���ĵ�
		//�Ե���
		for (int i = 0; i<size.height; ++i)
		{
			for (int j = 0; j<size.width; ++j)
			{
				//��������ĸ����������б��
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
								//���
								mFlag.push_back(make_pair(i, j));
							}
						}
					}
				}
			}
		}

		//����ǵĵ�ɾ��
		for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
		}

		//ֱ��û�е����㣬�㷨����
		if (mFlag.size() == 0)
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���
		}

		//�Ե���
		for (int i = 0; i<size.height; ++i)
		{
			for (int j = 0; j<size.width; ++j)
			{
				//��������ĸ����������б��
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
								//���
								mFlag.push_back(make_pair(i, j));
							}
						}
					}
				}
			}
		}
		//ɾ��
		for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
		}

		//ֱ��û�е����㣬�㷨����
		if (mFlag.size() == 0)
		{
			break;
		}
		else
		{
			mFlag.clear();//��mFlag���
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