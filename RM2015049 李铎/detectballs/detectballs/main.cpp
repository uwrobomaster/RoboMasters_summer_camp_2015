#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
vector<Mat> bgr_planes;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

Mat srcImage, midImage, dstImage,ballImage;
Mat img;
int threshval = 20;
vector<Vec3f> circles;

int arealength;
vector<Point> centers;

VideoCapture capture;

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

	capture.open("D:/gaoplane/videopics/balls1-6.avi");

	/*if (!capture.isOpened()){
	cout << "error when open camera" << endl;
	return -1;
	}*/

	for (int j = 0;; j++){
		capture >> srcImage;
		if (srcImage.empty()) continue;
		ballImage = srcImage.clone();
		resize(srcImage, srcImage, Size(300, 200));
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
		if (contours.empty() || hierarchy.empty()) continue;
		for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
		{
			Mat dst = Mat::zeros(img.size(), CV_8UC1);
			drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);
			Moments mt = moments(dst, true);
			int sizeOfArea = mt.m00 / cos(roll / 180 * CV_PI) / cos(pitch / 180 * CV_PI);
			arealength = sqrt(sizeOfArea)*height;
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
		if (contours.empty() || hierarchy.empty()) continue;
		for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
		{
			Mat dst = Mat::zeros(img.size(), CV_8UC1);
			drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);
			Moments mt = moments(dst, true);
			if (mt.m00 < 100) continue;
			Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
			centers.push_back(pt);
			/*cout << "width of area: " << arealength << endl;
			cout << "center: " << centers[centers.size() - 1].x << ", " << centers[centers.size() - 1].y << endl;
			cout << endl;*/
		}
		for (int i = 0; i < centers.size(); i++){
			circle(srcImage, centers[i], 3, Scalar(255, 0, 0), 3);
			circle(bw, centers[i], 3, 100, 3);
		}
		centers.clear();
		
		imshow("BGR", srcImage);
		char a = waitKey(10);
		if (a == ' '){
			continue;
		}
		else if (a == 'q'){
			capture.release();
			break;
		}

	}

	return 0;
}
