#ifndef CHASSIS_CTRL_H_INCLUDED
#define CHASSIS_CTRL_H_INCLUDED


#include "sys.h"

#define CHASSIS_VX_MAX 		3.0f				//x轴速度最大值
#define CHASSIS_VY_MAX 		3.0f				//y轴速度最大值
#define CHASSIS_VYAW_MAX	2.0f				//旋转速度最大值
#define CHASSIS_A			0.3f				//底盘宽度
#define CHASSIS_B			0.3f				//底盘长度

#define CHASSIS_SWING_CLOCKWISE			1		//扭腰，顺时针
#define CHASSIS_SWING_ANTICLOCKWISE		2		//扭腰，逆时针
#define CHASSIS_SWING_ANGLE				1000	//扭腰角度（一圈8192）

#define CHASSIS_VX_KEY_MAX				1.0f	//键盘控制时x轴速度最大值
#define CHASSIS_VY_KEY_MAX				1.0f	//键盘控制时y轴速度最大值
#define CHASSIS_VYAW_KEY_MAX			0.6f	//键盘控制时旋转速度最大值
#define CHASSIS_FOLLOW_ANGLE_ALLOWANCE	80		//底盘跟随角度余量



void Chassis_Init(void);		//底盘初始化
void Chassis_Ctrl(void);		//底盘控制
float Chassis_Swing( u8 Command, float chassis_yaw_speed);			//扭腰
float Chassis_Follow( void);										//底盘跟随




#endif // CHASSIS_H_INCLUDED
