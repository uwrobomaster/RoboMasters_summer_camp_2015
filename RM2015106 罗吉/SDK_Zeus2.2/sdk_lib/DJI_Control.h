#ifndef CONTROL_ZEUS
#define CONTROL_ZEUS
#define XY_SPEED_YAW_RATE 0
#define XY_SPEED_YAW_ANGLE 1
#define XY_POS_YAW_ANGLE 2
extern int Start_Control_Thread(void);
extern int q2e(const float *pq, float *pe);
extern int Set_Control_Param(void);
extern float PID(float target,float current);

typedef struct
{
	float	roll_degree;
	float	pitch_degree;
	float	yaw_degree;
}fmu_api_gimbal_data;

typedef struct
{
	float	x;
	float	y;
	float	z;
	float	w;
}api_ctrl_data;


typedef struct{
    float P;
    float I;
    float D;
}PID_Param;

extern api_ctrl_data drone_ctrl_data;

int Control_Cmd_Send(int flag) ;
#endif
