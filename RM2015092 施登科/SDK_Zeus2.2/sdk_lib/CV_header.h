#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"


#ifndef CV_HEADER_H
#define CV_HEADER_H
void CV_Init(void)  ;
void Grab_Image(void) ;
int LineDetect(int flag);
void CV_End(void) ;
void CV_Show(int state) ;
int Start_CV_Thread(void) ;
void ExposureChange(int value) ;
void Camera_Serial_Close(void);
void Stop_the_CVThread(void);
int BlackBoxCheck(void) ;
int  SodukuCheck(void) ;
int Start_Height_Control_Thread(void) ;
#endif // CV_HEADER_H

