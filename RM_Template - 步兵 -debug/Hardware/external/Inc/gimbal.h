#ifndef GIMBAL_H_INCLUDED
#define GIMBAL_H_INCLUDED

#include "sys.h"
#include "pid.h"



typedef struct
{
	u16 real_angle;
	u16 last_real_angle;
							
	signed short real_current;		//实际电流
	signed short set_current;		//给定电流
	
	int angle;					//处理后角度，一圈8192
	float angle_degree;			//同上，单位（度）
	int last_angle;
	float last_angle_degree;
	short raw_rate;				//转速（暂通过角度计算，也可加速度计反馈）单位：度/秒
	
	
	int angle_set;				//设定角度，一圈8192
	
} gimbal_motor_back;


void Gimbal_Pid_Init(void);
void Gimbal_Motor_Speed_Set( int yaw_motor_pos, int pitch_motor_pos);			//设置两个云台电机的位置
void Get_Gimbal_Motor_Info( u32 motor_id, u8 *canbuf_receive);


extern pid_t gimbal_pitch_spd_pid,gimbal_yaw_spd_pid;
extern pid_t gimbal_pitch_pos_pid,gimbal_yaw_pos_pid;
extern gimbal_motor_back gimbal_motor_info[2];



#endif // GIMBAL_H_INCLUDED
