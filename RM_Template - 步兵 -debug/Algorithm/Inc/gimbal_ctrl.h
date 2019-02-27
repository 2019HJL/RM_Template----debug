#ifndef GIMBAL_CTRL_H_INCLUDED
#define GIMBAL_CTRL_H_INCLUDED

#include "sys.h"


//#define PITCH_CHANGE_MAX		4.1f//0.18f					//pitch轴角速度最大值（度/10ms）
//#define YAW_CHANGE_MAX			4.1f//0.18f					//yaw轴角速度最大值


//#define PITCH_CHANGE_MAX		20.5f//0.18f					//pitch轴角速度最大值（度/10ms）
//#define YAW_CHANGE_MAX			20.5f//0.18f					//yaw轴角速度最大值



#define PITCH_CHANGE_MAX		20.0f//0.18f					//pitch轴角速度最大值（度/10ms）
#define YAW_CHANGE_MAX			10.0f//0.18f					//yaw轴角速度最大值


#define PITCH_ZERO_OFFSET		0.0f						//pitch轴零点偏移
#define YAW_ZERO_OFFSET			0.0f						//yaw轴零点偏移

#define PITCH_LIMIT		300										//pitch轴限位
#define YAW_LIMIT		1000									//yaw轴限位



void Gimbal_Init(void);
void Gimbal_Ctrl(void);
float Gimbal_Follow( u8 Command, float RC_Data, float Vision_Data);



#endif // GIMBAL_CTRL_H_INCLUDED
