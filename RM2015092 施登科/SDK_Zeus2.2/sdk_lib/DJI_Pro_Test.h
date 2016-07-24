/*
 * DJI_Pro_Test.h
 *
 *  Created on: 7 Apr, 2015
 *      Author: wuyuwei
 */

#ifndef DJI_PRO_TEST_H_
#define DJI_PRO_TEST_H_

/* external functions */
void DJI_Onboard_API_Simple_Task(int data);
void DJI_Onboard_API_Control(unsigned char data);
void DJI_Onboard_API_UAV_Control(unsigned char data);
void DJI_Onboard_API_Activation(void);
void DJI_Onboard_API_Status_Query(void);



int DJI_Pro_Test_Setup(void);
/* internal functions */
unsigned int Get_Time(void);
void spin_callback(void);

typedef struct
{
//public:
        float vx;
        float vy;
        float vz;

        float x;
        float y;
        float z;
}VO_OUTPUT;

typedef struct
{
    float x;
    float y;
    float w;
    float h;
    int status;
//    float drone_height;
//    float dist_total;
//    float dist_x;
//    float dist_y;
//    float drone_vx;
//    float drone_vy;
//    float drone_vz;
//    float gimbal_pitch;
//    float gimbal_yaw;
//    float gimbal_roll;
}uav_data_to_ground;

//send data to ground
void send_data_air2ground(uav_data_to_ground data);

extern float uav_height;
extern float camera_state[3];//pitch, yaw, roll
extern VO_OUTPUT vo_data;
extern float drone_attitude[3];

//data buffer from ground
extern int pbuf_t[4];//point.x / point.y...
extern bool data_recv;

extern uav_data_to_ground drone_data;

#endif /* DJI_PRO_TEST_H_ */
