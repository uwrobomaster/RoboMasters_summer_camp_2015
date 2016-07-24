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
vector<Point> centers;

VideoCapture capture;

void on_mouse(int event, int x, int y, int flags, void* ustc){
	if (event == CV_EVENT_LBUTTONDOWN){
		cout << "src point:(" << x << ", " << y << "): B:" << (int)srcImage.at<Vec3b>(y, x)[0] << " G:" << (int)srcImage.at<Vec3b>(y, x)[1] << " R:" << (int)srcImage.at<Vec3b>(y, x)[2] << endl;
		
	}
	else if (event == CV_EVENT_RBUTTONDOWN){
		cout << "src point:(" << x << ", " << y << "): H:" << (int)midImage.at<Vec3b>(y, x)[0] << " S:" << (int)midImage.at<Vec3b>(y, x)[1] << " V:" << (int)midImage.at<Vec3b>(y, x)[2] << endl;
	}
}

int main(){
	/*capture.open(0);
	capture.set(CV_CAP_PROP_EXPOSURE,-9);
	if (!capture.isOpened()){
		cout << "error when open camera" << endl;
		return -1;
	}*/
	for (int j = 0;; j++){
		char filename[50];
		sprintf(filename, "D:/gaoplane/videopics/redorange-9/img%4d.jpg", j);
		srcImage = imread(filename);
		//capture >> srcImage;
		if (srcImage.empty()) continue;

		resize(srcImage,srcImage,Size(150,100));
		imshow("src", srcImage);
		setMouseCallback("src", on_mouse);

		dstImage=srcImage.clone();
		cvtColor(dstImage, midImage, CV_BGR2HSV);
		for (int i = 0; i < srcImage.rows; i++){
			uchar * pt = srcImage.ptr<uchar>(i);
			uchar * dstpt = dstImage.ptr<uchar>(i);
			uchar * midpt = dstImage.ptr<uchar>(i);
			for (int j = 0; j < srcImage.cols; j++){
				//exposure -9
				if (pt[2]<90 || midpt[2]<90||pt[0]>120 || pt[0]<30 || pt[1]>160 || pt[1]<30 || pt[2]>160 || pt[2] < 100){
					dstpt[0] = 0;
					dstpt[1] = 0;
					dstpt[2] = 0;
				}
				else{
					if (pt[0] < 113 && pt[1] < 95){
						dstpt[0] = 0;
						dstpt[1] = 0;
						dstpt[2] = 255;
					}
					else{
						dstpt[0] = 0;
						dstpt[1] = 0;
						dstpt[2] = 0;
					}
				}
				//detect red rectangles
				//detect orenge rectangles

				/*if (pt[0]>120 || pt[0]<30 || pt[1]>160 || pt[1]<30 || pt[2]>160 || pt[2] < 100){
					dstpt[0] = 0;
					dstpt[1] = 0;
					dstpt[2] = 0;
				}
				else{
					if (pt[0] < 113 && pt[1] < 95){
						dstpt[0] = 0;
						dstpt[1] = 0;
						dstpt[2] = 255;
					}
					else{
						dstpt[0] = 80;
						dstpt[1] = 100;
						dstpt[2] = 100;
					}
				}*/
				/*if (pt[0]>100 || pt[0]<10 || pt[1]>110 || pt[2]>100 || pt[2] < 40){
					dstpt[0] = 0;
					dstpt[1] = 0;
					dstpt[2] = 0;
				}
				else{
					if (pt[0] < 60 && pt[1] < 50){
						dstpt[0] = 0;
						dstpt[1] = 0;
						dstpt[2] = 255;
					}
					else{
						dstpt[0] = 80;
						dstpt[1] = 100;
						dstpt[2] = 100;
					}
				}*/
				pt += 3; dstpt += 3; midpt += 3;
			}
		}
		imshow("dst",dstImage);
		cvtColor(dstImage, img, CV_BGR2GRAY);
		//imshow("Image", img);

		//erosion for the image
		int erosion_size = 2;
		Mat element = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));
		/// Apply the erosion operation
		erode(img, img, element);
		imshow("erosed image", img);

		Mat bw = (img> threshval);
		imshow("bn", bw);

		findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
		centers.clear();
		if (!contours.empty() && !hierarchy.empty())
		{
			for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
			{
				Mat dst = Mat::zeros(img.size(), CV_8UC1);
				drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);

				Moments mt = moments(dst, true);
				if (mt.m00 < 200) continue;

				Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
				centers.push_back(pt);
			}
		}
		Point bestcenter(img.cols, img.rows);
		for (int i = 0; i < centers.size(); i++){
			long distance1 = (centers[i].x - img.cols / 2)*(centers[i].x - img.cols / 2) + (centers[i].y - img.rows / 2)*(centers[i].y - img.rows / 2);
			long distance2 = (bestcenter.x - img.cols / 2)*(bestcenter.x - img.cols / 2) + (bestcenter.y - img.rows / 2)*(bestcenter.y - img.rows / 2);
			if (distance1<distance2){
				bestcenter.x = centers[i].x;
				bestcenter.y = centers[i].y;
			}
		}
		if (bestcenter.x != img.cols&&bestcenter.y != img.rows){
			circle(img, bestcenter, 3, 255, 3);
		}
		imshow("center", img);


		char a = waitKey(0);
		if (a == ' ')
			continue;
		else if (a == 's'){
			//imwrite("save.jpg",dstImage);
		}

	}

	return 0;
}