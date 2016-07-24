#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
vector<Mat> bgr_planes;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

Mat srcImage, midImage, dstImage;
Mat img;
int threshval = 20;

int arealength;
vector<Point> centers;

VideoCapture capture;

void mouse_bgr(int event, int x, int y, int flags, void* ustc){
	if (event == CV_EVENT_LBUTTONDOWN){
		cout << "positive BGR:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y, x)[1] << " R:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;
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

int main(){
	cout << "roll" << endl;
	float roll = 0;
	cin >> roll;

	cout << "pitch" << endl;
	float pitch = 0;
	cin >> pitch;

	cout << "height" << endl;
	float height = 1;
	cin >> height;

	/*capture.open(0);
	capture.set(CV_CAP_PROP_EXPOSURE,-7);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,480);*/

	VideoWriter wrvideo;
	//wrvideo.open("cubesresults.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(640,480));

	//capture.open("D:/gaoplane/videopics/blackrectangle.avi");
	capture.open("D:/gaoplane/videopics/balls3-7.avi");
	//capture.open("D:/gaoplane/videopics/9cube01-7.avi");

	/*if (!capture.isOpened()){
	cout << "error when open camera" << endl;
	return -1;
	}*/

	for (int j = 0;; j++){

		/*char filename[50];
		sprintf(filename, "D:/gaoplane/videopics/redorange-9/img%4d.jpg", j);
		srcImage = imread(filename);*/
		capture >> srcImage;
		if (srcImage.empty()) continue;

		resize(srcImage, srcImage, Size(150, 100));
		cvtColor(srcImage, midImage, CV_BGR2HSV);
		imshow("HSV",midImage);
		setMouseCallback("HSV", mouse_hsv);
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
				//detecting 9cube01-7.avi
				////if (pt[0]>90&&pt[1]<200&&pt[2]>180&&pt[2]*1.9>pt[0]+pt[1]&&(float)pt[0]/(pt[0]+pt[1])>0.44&&dstpt[1]>80){
				//if (pt[2]>180&& (float)pt[0] / (pt[0] + pt[1])>=0.44&&pt[1]-pt[0]<30&&dstpt[1]>65){
				//	dstpt[0] = 255;
				//	dstpt[1] = 0;
				//	dstpt[2] = 0;
				//}
				//else{
				//	dstpt[0] = 0;
				//	dstpt[1] = 0;
				//	dstpt[2] = 0;
				//}
				pt += 3; dstpt += 3;
			}
		}
		cvtColor(dstImage, img, CV_BGR2GRAY);

		//int erosion_size = 3;
		//Mat element = getStructuringElement(MORPH_ELLIPSE,
		//	Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		//	Point(erosion_size, erosion_size));
		///// Apply the erosion operation
		//erode(img, img, element);

		Mat bw = (img> threshval);
		imshow("bw", bw);
		setMouseCallback("bw", mouse_bgr);

		findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
		if (!contours.empty() && !hierarchy.empty())
		{
			for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
			{
				Mat dst = Mat::zeros(img.size(), CV_8UC1);
				drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);

				Moments mt = moments(dst, true);
				int sizeOfArea = mt.m00 / cos(roll / 180 * CV_PI) / cos(pitch / 180 * CV_PI);
				arealength = sqrt(sizeOfArea)*height ;
				if (arealength < 30) continue;
				
				Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
				centers.push_back(pt);
				cout << "width of area: " << arealength<< endl;
				cout << "center: " << centers[centers.size() - 1].x << ", " << centers[centers.size() - 1].y << endl;
				cout << endl;
			}
		}
		for (int i = 0; i < centers.size(); i++){
			circle(srcImage, centers[i],3,Scalar(0,0,255),3);
			circle(bw, centers[i], 3, 50, 3);
		}
		centers.clear();
		imshow("BGR", srcImage); 
		setMouseCallback("BGR", mouse_bgr);
		//wrvideo.write(srcImage);

		char a = waitKey(0);
		if (a == ' '){
			continue;
		}else if(a=='q'){
			capture.release();
			wrvideo.release();
			break;
		}

	}

	return 0;
}