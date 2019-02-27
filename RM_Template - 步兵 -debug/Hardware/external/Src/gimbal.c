#include "gimbal.h"
#include "can.h"

#include "DR16.h"

pid_t gimbal_pitch_spd_pid,gimbal_yaw_spd_pid;
pid_t gimbal_pitch_pos_pid,gimbal_yaw_pos_pid;

gimbal_motor_back gimbal_motor_info[2];


void Gimbal_Pid_Init(void)
{	
//	//yaw
//	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 5000, 5000, 5.0f, 0.0f, 0.0f);				//位置环初始化
//	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 1000, 1000, -0.899f, 0.0f, 0.0f);			//速度环初始化
	
	
//	//yaw
//	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 5000, 5000, 6.0f, 0.02f, 0.2f);				//位置环初始化
//	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 1000, 1000, -0.899f, 0.0f, 0.0f);			//速度环初始化
	
	//yaw
	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 5000, 5000, 5.0f, 0.0f, 0.0f);				//位置环初始化
	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 1000, 1000, -0.899f, 0.0f, 0.0f);			//速度环初始化
	
	
	//pitch
	pid_init( &gimbal_pitch_pos_pid, POSITION_PID, 10000, 10000, 5.0f, 0.0f, 0.0f);				//位置环初始化
	pid_init( &gimbal_pitch_spd_pid, POSITION_PID, 2000, 2000, -0.899f, 0.0f, 0.0f);			//速度环初始化
	
	
//		//yaw
//	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 2000, 2000, 0.8f, 0.0f, 0.0f);				//位置环初始化
//	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 2000, 2000, -3.3f, -0.02f, 0.8f);			//速度环初始化
//	
//	
//	//pitch
//	pid_init( &gimbal_pitch_pos_pid, POSITION_PID, 2000, 2000, 4.0f, 0.0f, 3.0f);				//位置环初始化
//	pid_init( &gimbal_pitch_spd_pid, POSITION_PID, 2000, 2000, -2.0f, -0.02f, 0.6f);			//速度环初始化

	
//	gimbal_motor_info[0].angle 		= 0;
	gimbal_motor_info[0].angle_set  = 0;
//	gimbal_motor_info[1].angle 		= 0;
	gimbal_motor_info[1].angle_set  = 0;
}

short temppp[2];

//设置两个云台电机的位置
void Gimbal_Motor_Speed_Set( int yaw_motor_pos, int pitch_motor_pos)
{
	//u8 t;
	short pid_out[2];
	u8 canbuf[8];


	//6623,pid

	//yaw
	pid_calc( &gimbal_yaw_pos_pid, gimbal_motor_info[0].angle, yaw_motor_pos);
	pid_out[0] = pid_calc( &gimbal_yaw_spd_pid, gimbal_motor_info[0].raw_rate, /*set_gimbal_pos[t]);*/gimbal_yaw_pos_pid.pos_out);	
	//pitch
	pid_calc( &gimbal_pitch_pos_pid, gimbal_motor_info[1].angle, /*( RC_CtrlData.rc.ch0 - 1024) /8 );*/pitch_motor_pos);
	pid_out[1] = pid_calc( &gimbal_pitch_spd_pid, gimbal_motor_info[1].raw_rate, /*set_gimbal_pos[t]);*/gimbal_pitch_pos_pid.pos_out);


	temppp[0]=pid_out[0];
	temppp[1]=gimbal_motor_info[0].angle;//pitch_motor_pos;
	
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = pid_out[1] >> 8;
	canbuf[3] = pid_out[1];
	canbuf[4] = 0;
	canbuf[5] = 0;
	canbuf[6] = 0;
	canbuf[7] = 0;
	
	
	CAN1_Send_Msg( 0x1ff, canbuf, 8);		//发送数据
}



void Get_Gimbal_Motor_Info( u32 motor_id, u8 *canbuf_receive)
{
	static u32 cnt = 0;
	short round_cnt;
	short diff;	
	
	if( motor_id - 0x205 >= 0 && motor_id - 0x205 < 2)
	{
		
		if( cnt == 0)
		{
			gimbal_motor_info[0].last_real_angle = 0;
			gimbal_motor_info[1].last_real_angle = 0;
			gimbal_motor_info[0].angle 		= -3935;			
			gimbal_motor_info[1].angle 		= 0;			
		}
				
		++cnt;	
//		gimbal_motor_info[motor_id - 0x205].last_real_angle = gimbal_motor_info[motor_id - 0x205].real_angle;
		
		gimbal_motor_info[motor_id - 0x205].real_angle = canbuf_receive[0] << 8 | canbuf_receive[1];
		gimbal_motor_info[motor_id - 0x205].real_current = (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		gimbal_motor_info[motor_id - 0x205].set_current = (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		
		gimbal_motor_info[motor_id - 0x205].last_angle = gimbal_motor_info[motor_id - 0x205].angle;
		gimbal_motor_info[motor_id - 0x205].last_angle_degree = gimbal_motor_info[motor_id - 0x205].angle_degree;
		
		
		diff = gimbal_motor_info[motor_id - 0x205].real_angle - gimbal_motor_info[motor_id - 0x205].last_real_angle;
		
		gimbal_motor_info[motor_id - 0x205].last_real_angle = gimbal_motor_info[motor_id - 0x205].real_angle;

		
		//过零点处理
		if( diff > 7500)
		{
			--round_cnt;	//逆时针
			diff = -(8192 - diff);
		}
		else if( diff < -7500)
		{
			++round_cnt;		//顺时针
			diff = (8192 + diff);
		}
		
//		//排除第一次的数据	
//		if( cnt == 0)
//			diff = 0;				
//		++cnt;		
//		//第一次的数据	
		

		//单位换算
		gimbal_motor_info[motor_id - 0x205].angle += diff;
		gimbal_motor_info[motor_id - 0x205].angle_degree += (float)(diff * 360.0f / 8192.0f);
		gimbal_motor_info[motor_id - 0x205].raw_rate = (float)(gimbal_motor_info[motor_id - 0x205].angle_degree - gimbal_motor_info[motor_id - 0x205].last_angle_degree) * 1000.0f;
	}
	
}

