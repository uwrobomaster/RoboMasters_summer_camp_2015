#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img;
int threshval = 20;
vector<Point> centers;
int main()
{
	img = imread("save.jpg",0);
	imshow("Image", img);

	//erosion for the image
	int erosion_size = 3;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	/// Apply the erosion operation
	erode(img, img, element);
	namedWindow("erosed image");
	imshow("erosed image", img);

	Mat bw = (img> threshval);
	imshow("bn", bw);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);

	if (!contours.empty() && !hierarchy.empty())
	{
		for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
		{
			Mat dst = Mat::zeros(img.size(), CV_8UC1);
			drawContours(dst, contours, idx, 255, CV_FILLED, 8, hierarchy);

			Moments mt=moments(dst,true);
			if (mt.m00 < 100) continue;

			Point pt((int)mt.m10 / mt.m00, (int)mt.m01 / mt.m00);
			centers.push_back(pt);
		}
	}
	Point bestcenter(img.rows,img.cols);
	for (int i = 0; i < centers.size(); i++){
		if (abs(centers[i].x - img.rows / 2) + abs(centers[i].y - img.cols / 2) <= abs(bestcenter.x - img.rows / 2) + abs(bestcenter.y - img.cols / 2)){
			bestcenter = centers[i];
		}
	}
	circle(img, bestcenter, 3, 255, 3);
	imshow("center",img);

	waitKey(0);
	return 0;
}
