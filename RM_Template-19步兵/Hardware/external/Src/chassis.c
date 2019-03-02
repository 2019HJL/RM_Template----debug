#include "chassis.h"
#include "can.h"
#include "usart.h"
#include "chassis_power.h"
#include "beep.h"

#include "DR16.h"

extern volatile int pid_zero_flag;

volatile uint32_t chassis_offline_cnt[4] = {0};			//���̵��߼���������ʱ���ж������ӣ������ж����㣩



pid_t chassis_spd_pid[4];
pid_t chassis_pos_pid[4];

chassis_motor_back chassis_motor_info[4];

void Chassis_Pid_Init(void)
{
	u8 t;
	for( t = 0; t < 4; ++t)
	{
//		pid_init( &chassis_pos_pid[t], POSITION_PID, 3000, 3000, 0.04f, 0.00003f, 0.0f);		//λ�û���ʼ��
//		pid_init( &chassis_spd_pid[t], POSITION_PID, 16000, 16000, 1.5f, 0.1f, 0.0f);			//�ٶȻ���ʼ��

		pid_init( &chassis_pos_pid[t], POSITION_PID, 1000, 500, 0.02f, 0.000000f, 0.000012f);	//λ�û���ʼ��
		pid_init( &chassis_spd_pid[t], POSITION_PID, 10000, 4000, 1.5f, 0.1f, 0.000f);			//�ٶȻ���ʼ��

		
		chassis_motor_info[t].angle 	= 0;
		chassis_motor_info[t].angle_set = 0;
	}	
}


//�����ĸ����̵�����ٶ�
void Chassis_Motor_Speed_Set(short *motor_speed)
{
	short pid_out[4];
	u8 t;
	u8 canbuf[8];

	for( t = 0; t < 4; ++t)
	{
//		if( motor_speed[t] == 0)
//		{
//			pid_calc( &chassis_pos_pid[t], chassis_motor_info[t].angle, /*motor_pos[t]*/chassis_motor_info[t].angle_set);	//�⻷��λ�û���ʵ��ֵ����е�Ƕȣ���Ŀ��ֵ������Ƕȣ�
//			pid_out[t] = pid_calc( &chassis_spd_pid[t], chassis_motor_info[t].rpm, chassis_pos_pid[t].pos_out);				//�ڻ����ٶȻ���ʵ��ֵ��Ŀ���ٶȣ���Ŀ��ֵ���⻷�����
//		}
//		else
		{
			if(chassis_offline_cnt[t] < 100)		//����ʱ��С��100ms�������㣬���򲻿��Ƹõ��
			{
				pid_out[t] = pid_calc( &chassis_spd_pid[t], chassis_motor_info[t].rpm, motor_speed[t]);			//����
				chassis_motor_info[t].angle_set = chassis_motor_info[t].angle;
			}
		}		
	}
	
	if( pid_zero_flag == SET )//|| RC_CtrlData.rc.s2 == 3)
		for( t = 0; t < 4; ++t)
			pid_out[t] = 0;

	
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = pid_out[1] >> 8;
	canbuf[3] = pid_out[1];
	canbuf[4] = pid_out[2] >> 8;
	canbuf[5] = pid_out[2];
	canbuf[6] = pid_out[3] >> 8;
	canbuf[7] = pid_out[3];

	CAN1_Send_Msg( 0x200, canbuf, 8);		//��������
	
}





void Get_Chassis_Motor_Info( uint32_t motor_id, u8 *canbuf_receive)
{
	static u32 cnt = 0;
	short diff;
	
	if( (motor_id - 0x201 >= 0) && (motor_id - 0x201 < 4))
	{
		chassis_offline_cnt[motor_id - 0x201] = 0;		//������߼���������
		
		chassis_motor_info[motor_id - 0x201].last_real_angle = chassis_motor_info[motor_id - 0x201].real_angle;			//�����ϴνǶ�
		
		//��ȡ��ǰ��Ϣ
		chassis_motor_info[motor_id - 0x201].real_angle 	= canbuf_receive[0] << 8 | canbuf_receive[1];
		chassis_motor_info[motor_id - 0x201].rpm 			= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		chassis_motor_info[motor_id - 0x201].real_current 	= (signed short)(canbuf_receive[4] << 8 | canbuf_receive[5]);
		chassis_motor_info[motor_id - 0x201].temperature 	= canbuf_receive[6];
		
		chassis_motor_info[motor_id - 0x201].current 		= (float)chassis_motor_info[motor_id - 0x201].real_current / 819.2f;
		
		
		//����㴦��
		diff = chassis_motor_info[motor_id - 0x201].real_angle - chassis_motor_info[motor_id - 0x201].last_real_angle;
		if( diff > 7500)
		{
			diff = -(8192 - diff);
		}
		else if( diff < -7500)
		{
			diff = (8192 + diff);
		}
		
		
		//�ų���һ�ε�����	
		if( cnt == 0)
			diff = 0;				
		++cnt;	
		
		//�ǶȻ���
		chassis_motor_info[motor_id - 0x201].angle += diff;
		
	}
	
}

