#include "gimbal.h"
#include "can.h"

#include "DR16.h"

pid_t gimbal_pitch_spd_pid,gimbal_yaw_spd_pid;
pid_t gimbal_pitch_pos_pid,gimbal_yaw_pos_pid;

gimbal_motor_back gimbal_motor_info[2];


extern volatile int Yaw;
extern volatile float mygetqval[9];	//用于存放传感器转换结果的数组

void Gimbal_Pid_Init(void)
{	
	//pitch轴PID
	pid_init( &gimbal_pitch_pos_pid, POSITION_PID, 10000, 10000, 0.2f, 0.0f, -0.0001f);				//位置环初始化
	pid_init( &gimbal_pitch_spd_pid, POSITION_PID, 2000, 1000, -10.0f, -0.000001f, 0.0f);			//速度环初始化
	

//	//yaw with imu degree
//	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 5000, 5000, 0.03f, 0.0f, -0.0001f);				//位置环初始化
//	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 2000, 1000, -10.0f, -0.000001f, 0.0f);				//速度环初始化

	//yaw with imu 
	pid_init( &gimbal_yaw_pos_pid, POSITION_PID, 5000, 5000, 0.03f, 0.0f, -0.0001f);				//位置环初始化
	pid_init( &gimbal_yaw_spd_pid, POSITION_PID, 3000, 1000, -6.2f, -0.000004f, 0.0f);				//速度环初始化
	
	
	
//	gimbal_motor_info[0].angle 		= 0;
	gimbal_motor_info[0].angle_set  = 0;
//	gimbal_motor_info[1].angle 		= 0;
	gimbal_motor_info[1].angle_set  = 0;
}

extern u8 ammunition_canbuf[8];

//设置两个云台电机的位置
void Gimbal_Motor_Speed_Set( int yaw_motor_pos, int pitch_motor_pos)
{
	short pid_out[2];
	u8 canbuf[8];


	//yaw
//	pid_calc( &gimbal_yaw_pos_pid, gimbal_motor_info[0].angle, yaw_motor_pos);
//	pid_out[0] = pid_calc( &gimbal_yaw_spd_pid, gimbal_motor_info[0].raw_rate, /*set_gimbal_pos[t]);*/gimbal_yaw_pos_pid.pos_out);	
	//pitch
//	pid_calc( &gimbal_pitch_pos_pid, gimbal_motor_info[1].angle, /*( RC_CtrlData.rc.ch0 - 1024) /8 );*/pitch_motor_pos);
//	pid_out[1] = pid_calc( &gimbal_pitch_spd_pid, gimbal_motor_info[1].raw_rate, /*set_gimbal_pos[t]);*/gimbal_pitch_pos_pid.pos_out);

	//yaw
	pid_calc( &gimbal_yaw_pos_pid, Yaw, yaw_motor_pos);
	pid_out[0] = pid_calc( &gimbal_yaw_spd_pid, mygetqval[5]/*gimbal_motor_info[0].raw_rate*/, /*set_gimbal_pos[t]);*/gimbal_yaw_pos_pid.pos_out);	
	
	//pitch
	pid_calc( &gimbal_pitch_pos_pid, gimbal_motor_info[1].angle, /*( RC_CtrlData.rc.ch1 - 1024)*4  );*/pitch_motor_pos);
	pid_out[1] = pid_calc( &gimbal_pitch_spd_pid, gimbal_motor_info[1].raw_rate, /*(RC_CtrlData.rc.ch0 - 1024) );*/gimbal_pitch_pos_pid.pos_out);


//	printf("%d,%d\r\n",(int)mygetqval[5],(int)gimbal_motor_info[0].raw_rate);
	
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = pid_out[1] >> 8;
	canbuf[3] = pid_out[1];
//	canbuf[4] = 0;
//	canbuf[5] = 0;
	canbuf[6] = 0;
	canbuf[7] = 0;
	
	//拨弹电机数据，在此发送避免控制错误（分开发送会有0和数据间隔发送，影响控制）
	canbuf[4] = ammunition_canbuf[4];
	canbuf[5] = ammunition_canbuf[5];
	
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
			gimbal_motor_info[0].last_real_angle 	= 0;
			gimbal_motor_info[1].last_real_angle 	= 0;
			gimbal_motor_info[0].angle 				= -GIMBAL_YAW_OFFSET;			
			gimbal_motor_info[1].angle 				= -GIMBAL_PITCH_OFFSET;			
		}
				
		++cnt;	
		
		gimbal_motor_info[motor_id - 0x205].real_angle 			= canbuf_receive[0] << 8 | canbuf_receive[1];
		gimbal_motor_info[motor_id - 0x205].real_current 		= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		gimbal_motor_info[motor_id - 0x205].set_current 		= (signed short)( canbuf_receive[4] <<8 | canbuf_receive[5]);	
		
		
		//获取改变量
		diff = gimbal_motor_info[motor_id - 0x205].real_angle - gimbal_motor_info[motor_id - 0x205].last_real_angle;
		
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
		
		
		//记录角度
		gimbal_motor_info[motor_id - 0x205].last_angle 			= gimbal_motor_info[motor_id - 0x205].angle;
		gimbal_motor_info[motor_id - 0x205].last_real_angle 	= gimbal_motor_info[motor_id - 0x205].real_angle;		

		//计算使用值
		gimbal_motor_info[motor_id - 0x205].angle += diff;
		gimbal_motor_info[motor_id - 0x205].raw_rate = diff;
	}
	
}

