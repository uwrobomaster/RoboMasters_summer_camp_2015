#ifndef LOGIC_CV_THREAD_H
#define LOGIC_CV_THREAD_H
void control(float x,float y ,float z, float w ) ;
float PID_Y(float set);
float PID_Yaw(float set) ;
float PID_Height(float set) ;
static void * CV_Thread_Function(void * arg) ;
int Start_CV_Thread(void) ;
#endif // LOGIC_CV_THREAD_H
