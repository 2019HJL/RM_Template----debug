#ifndef CHASSIS_H_INCLUDED
#define CHASSIS_H_INCLUDED

#include "sys.h"
#include "pid.h"

typedef struct
{
	u16 real_angle;						//ʵ�ʽǶȣ�һȦ8192
	signed short rpm;
	signed short real_current;
	u8 temperature;
	
	
	u16 last_real_angle;				//�ϴ�ʵ�ʽǶ�
	int angle;							//�����Ƕ�ֵ
	
	float current;						//������ʵ�ʵ���
	
	int angle_set;						//�趨�Ƕȣ�һȦ8192
	
} chassis_motor_back;




void Chassis_Pid_Init(void);
void Chassis_Motor_Speed_Set( short *motor_speed/*, int *motor_angle int *motor_pos*/);
void Get_Chassis_Motor_Info( uint32_t motor_id, u8 *canbuf_receive);




extern pid_t chassis_spd_pid[4];
extern pid_t chassis_pos_pid[4];
extern chassis_motor_back chassis_motor_info[4];



#endif // CHASSIS_H_INCLUDED
