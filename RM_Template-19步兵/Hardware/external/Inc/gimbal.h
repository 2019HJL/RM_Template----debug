#ifndef GIMBAL_H_INCLUDED
#define GIMBAL_H_INCLUDED

#include "sys.h"
#include "pid.h"

#define GIMBAL_YAW_OFFSET 	2250/*5035*/
#define GIMBAL_PITCH_OFFSET 0


typedef struct
{
	//ʵ�ʽǶ�
	u16 real_angle;
	u16 last_real_angle;
							
	signed short real_current;		//ʵ��ת�ص���
	signed short set_current;		//����ת�ص���
	
	int angle;						//�����Ƕȣ�һȦ8192
	int last_angle;	
	float raw_rate;					//ת�٣����ν��սǶȲ
		
	int angle_set;					//�趨�Ƕȣ�һȦ8192
	
} gimbal_motor_back;


void Gimbal_Pid_Init(void);
void Gimbal_Motor_Speed_Set( int yaw_motor_pos, int pitch_motor_pos);			//����������̨�����λ��
void Get_Gimbal_Motor_Info( u32 motor_id, u8 *canbuf_receive);


extern pid_t gimbal_pitch_spd_pid,gimbal_yaw_spd_pid;
extern pid_t gimbal_pitch_pos_pid,gimbal_yaw_pos_pid;
extern gimbal_motor_back gimbal_motor_info[2];



#endif // GIMBAL_H_INCLUDED
