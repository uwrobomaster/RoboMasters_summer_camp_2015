#ifndef CONTROL_ZEUS
#define CONTROL_ZEUS
extern int Start_Control_Thread(void);
//extern int Start_ImagDect_Thread(void);
extern int q2e(const float *pq, float *pe);
extern int Test_Control(void);

/************PID_void************/
float PID_Height_Adj(float Height_Set,float Height_Actual);
void  Height_PIDinit();
void Theta_PIDinit();
void Rho_PIDinit();
void X_Offset_PIDinit();
float PID_Theta_Adj(float theta);
float PID_Rho_Adj(float rho);
float PID_X_Adj(float x_offset);
/*******************************/

typedef struct
{
    int	roll_degree;
    int	pitch_degree;
	float	yaw_degree;
}fmu_api_gimbal_data;

typedef struct
{
	float	x;
	float	y;
	float	z;
	float	w;
}api_ctrl_data;


/*PID*/
struct Height_PID
{
//  float SetHeight;
//  float ActualHeight;
    float err;
    float err_next;
    float err_last;
    float Kp,Ki,Kd;
};


struct Speed_PID
{
    float SetSpeed;
    float ActualSpeed;
    float err;
    float err_next;
    float err_last;
    float Kp,Ki,Kd;
};

struct Theta_PID
{
    float err;
    float err_next;
    float err_last;
    float Kp,Ki,Kd;
};

struct Rho_PID
{
    float err;
//  float err_next;
    float err_last;
    float Kp,Ki,Kd;
};

struct X_Offset_PID
{
    float err;
//  float err_next;
    float err_last;
    float Kp,Ki,Kd;
};


extern api_ctrl_data drone_ctrl_data;

#endif
