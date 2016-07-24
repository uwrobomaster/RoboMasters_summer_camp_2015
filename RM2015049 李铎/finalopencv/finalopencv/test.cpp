/**
* @file Pyramids.cpp
* @brief Sample code of image pyramids (pyrDown and pyrUp)
* @author OpenCV team
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables
Mat src, dst, tmp;

/**
* @function main
*/
int test(void)
{
	src = imread("1.jpg");
	pyrDown(src, src);
	pyrDown(src, src);
	//pyrDown(src, src);
	namedWindow("img", WINDOW_AUTOSIZE);
	imshow("img", src);

	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			int tmp = src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2];
			if (tmp<630){
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}

	namedWindow("newimg", WINDOW_AUTOSIZE);
	imshow("newimg", src);
	waitKey();

	return 0;
}
