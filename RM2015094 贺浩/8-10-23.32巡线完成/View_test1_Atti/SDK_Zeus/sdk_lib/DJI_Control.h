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
void Hori_Vel_PIDinit();
float PID_Theta_Adj(float theta);
float PID_Rho_Adj(float rho);
float PID_Hori_Vel_Adj(float Hori_Vel_Set,float Hori_Vel_Actual);
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


struct Hori_Vel_PID
{
    float Set_Hori_Vel;
    float Actual_Hori_Vel;
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


extern api_ctrl_data drone_ctrl_data;

#endif
