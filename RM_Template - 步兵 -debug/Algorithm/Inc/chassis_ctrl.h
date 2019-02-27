#ifndef CHASSIS_CTRL_H_INCLUDED
#define CHASSIS_CTRL_H_INCLUDED


#include "sys.h"

#define CHASSIS_VX_MAX 		2.0f				//x���ٶ����ֵ
#define CHASSIS_VY_MAX 		2.0f				//y���ٶ����ֵ
#define CHASSIS_VYAW_MAX	1.0f				//��ת�ٶ����ֵ
#define CHASSIS_A			0.5f				//���̿��
#define CHASSIS_B			0.36f				//���̳���

#define CHASSIS_SWING_CLOCKWISE			1		//Ť����˳ʱ��
#define CHASSIS_SWING_ANTICLOCKWISE		2		//Ť������ʱ��


void Chassis_Init(void);		//���̳�ʼ��
void Chassis_Ctrl(void);		//���̿���
float Chassis_Swing( u8 Command, float chassis_yaw_speed);			//Ť��
float Chassis_Follow( void);										//���̸���





#endif // CHASSIS_H_INCLUDED
