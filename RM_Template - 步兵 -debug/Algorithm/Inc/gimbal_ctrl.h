#ifndef GIMBAL_CTRL_H_INCLUDED
#define GIMBAL_CTRL_H_INCLUDED

#include "sys.h"


//#define PITCH_CHANGE_MAX		4.1f//0.18f					//pitch����ٶ����ֵ����/10ms��
//#define YAW_CHANGE_MAX			4.1f//0.18f					//yaw����ٶ����ֵ


//#define PITCH_CHANGE_MAX		20.5f//0.18f					//pitch����ٶ����ֵ����/10ms��
//#define YAW_CHANGE_MAX			20.5f//0.18f					//yaw����ٶ����ֵ



#define PITCH_CHANGE_MAX		20.0f//0.18f					//pitch����ٶ����ֵ����/10ms��
#define YAW_CHANGE_MAX			10.0f//0.18f					//yaw����ٶ����ֵ


#define PITCH_ZERO_OFFSET		0.0f						//pitch�����ƫ��
#define YAW_ZERO_OFFSET			0.0f						//yaw�����ƫ��

#define PITCH_LIMIT		300										//pitch����λ
#define YAW_LIMIT		1000									//yaw����λ



void Gimbal_Init(void);
void Gimbal_Ctrl(void);
float Gimbal_Follow( u8 Command, float RC_Data, float Vision_Data);



#endif // GIMBAL_CTRL_H_INCLUDED
