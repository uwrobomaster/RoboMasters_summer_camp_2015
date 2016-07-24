#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat frame;
int main(){
	//VideoCapture capture("video.avi");
	VideoCapture capture(0);
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//capture.set(CV_CAP_PROP_POS_FRAMES, 0);   //设置开始帧数
	//double rate = capture.get(CV_CAP_PROP_FPS);  //获取帧率
	while (1){
		capture >> frame;
		imshow("video frames", frame);

		if (waitKey(10)==' ') break;
	}
	waitKey(0);
	return 0;
}