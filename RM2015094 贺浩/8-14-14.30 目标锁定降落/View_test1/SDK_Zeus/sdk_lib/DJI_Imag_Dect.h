#ifndef DJI_IMAG_DECT_H
#define DJI_IMAG_DECT_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>

#ifndef __APPLE__
#define EXPOSURE_CONTROL // only works in Linux
#endif

#ifdef EXPOSURE_CONTROL
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <errno.h>
#endif
using namespace std;
using namespace cv;

extern int Start_ImagDect_Thread(void);//启动检测直线
extern int Start_Dect_Square_Thread(void);//启动检测方块的线程
void initCam();
void initvideo();
bool gethist(Mat dstImage);
void on_mouse(int event, int x, int y, int flags, void* ustc);
void get_image();

void Imag_Dect_Function(Mat srcimage);
void Imag_Dect_Square_Function(int color,Mat srcimage);



// vector<Point> centers;
//extern void * Imag_Dect_Function(void * arg);
typedef struct
{
		int besttheta;
		int bestrho;
        bool blackDetected;
        bool squareDetected;
		vector<Point> centers;
}image_data;

typedef struct
{
float roll;
float pitch;
float  height;

}gimbal_data;


enum Statues{detect_line_ready,detect_line,
             detect_9_square_ready,detect_9_square,
             detect_1_square_ready,detect_1_square,
             detect_line_square_ready,detect_line_square,
             detect_ball_square_ready,detect_ball_square
            };
const string usage = "\n"
  "Usage:\n"
  "  apriltags_demo [OPTION...] [IMG1 [IMG2...]]\n"
  "\n"
  "Options:\n"
  "  -h  -?          Show help options\n"
  "  -a              Arduino (send tag ids over serial port)\n"
  "  -d              Disable graphics\n"
  "  -t              Timing of tag extraction\n"
  "  -C <bbxhh>      Tag family (default 36h11)\n"
  "  -D <id>         Video device ID (if multiple cameras present)\n"
  "  -F <fx>         Focal length in pixels\n"
  "  -W <width>      Image width (default 640, availability depends on camera)\n"
  "  -H <height>     Image height (default 480, availability depends on camera)\n"
  "  -S <size>       Tag size (square black frame) in meters\n"
  "  -E <exposure>   Manually set camera exposure (default auto; range 0-10000)\n"
  "  -G <gain>       Manually set camera gain (default auto; range 0-255)\n"
  "  -B <brightness> Manually set the camera brightness (default 128; range 0-255)\n"
  "\n";


extern image_data   drone_image_data;
extern gimbal_data  drone_gimbal_data;//定义角度信息 need


extern Statues   		statues;

extern float mye[3];\
extern float Act_Hei;
extern float deltaCam;
extern char filename[20];
extern char videoname[20];
extern VideoWriter wrVideo;
#endif // DJI_IMAG_DECT_H
