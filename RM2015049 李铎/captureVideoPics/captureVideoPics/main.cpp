#include <opencv2/opencv.hpp>
#include <iostream>
#include <direct.h>
using namespace std;
using namespace cv;
VideoWriter wrvideo;
int main(){
	VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	wrvideo.open("balls3-7.avi",CV_FOURCC('M','J','P','G'),25,Size(640,480));
	capture.set(CV_CAP_PROP_EXPOSURE,-7);
	bool start = false;
	Mat frame;
	//_mkdir("cubeImages");
	for (int i=0;;i++){
		capture >> frame;
		if (frame.empty())continue;

		imshow("image", frame);
		if (start){
			char filename[20];
			//sprintf(filename,"D:/gaoplane/videopics/redorange-9/img%4d.jpg",i);
			//sprintf(filename, "cubeImages/img%d.jpg", i);
			//imwrite(filename,frame);
			wrvideo.write(frame);
			cout << "writing files: " << i << ".jpg" << endl;
		}
		char a = waitKey(10);
		if (a == 'q'){
			capture.release();
			wrvideo.release();
			break;
		}
		else if (a == ' '){
			start = true;
			i = 0;
			cout << "start capturing" << endl;
		}
		else if (a == 's'){
			capture.set(CV_CAP_PROP_EXPOSURE, -7);
			cout << "exposure -7 set" << endl;
		}
		else if (a == 'w'){
			capture.set(CV_CAP_PROP_EXPOSURE, -6);
			cout << "exposure -6 set" << endl;
		}
	}
	return 0;
}