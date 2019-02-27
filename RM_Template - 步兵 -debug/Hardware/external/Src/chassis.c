#include "chassis.h"
#include "can.h"
#include "usart.h"
#include "chassis_power.h"
#include "beep.h"

const u8 TRUE = 0;
const u8 FALSE = 1;

pid_t chassis_spd_pid[4];
pid_t chassis_pos_pid[4];

chassis_motor_back chassis_motor_info[4];

void Chassis_Pid_Init(void)
{
	u8 t;
	for( t = 0; t < 4; ++t)
	{
		pid_init( &chassis_pos_pid[t], POSITION_PID, 3000, 3000, 0.04f, 0.00003f, 0.0f);							//位置环初始化
		pid_init( &chassis_spd_pid[t], POSITION_PID, 16000, 16000, 1.5f, 0.1f, 0.0f);			//速度环初始化

		
		chassis_motor_info[t].angle 	= 0;
		chassis_motor_info[t].angle_set = 0;
	}	
}

short pid_out[4];
//设置四个底盘电机的速度
void Chassis_Motor_Speed_Set( short *motor_speed/*, int *motor_angle int *motor_pos*/)
{
	u8 t;
	u8 double_cir = FALSE;
	u8 canbuf[8];
//	static int last_speed[4];
//	static int angle[4];

//	//运动时保存角度
//	if( !(last_speed[0] == 0 && motor1 == 0))	angle[0] = chassis_motor_info[0].angle;
//	if( !(last_speed[1] == 0 && motor2 == 0))	angle[1] = chassis_motor_info[1].angle;
//	if( !(last_speed[2] == 0 && motor3 == 0))	angle[2] = chassis_motor_info[2].angle;
//	if( !(last_speed[3] == 0 && motor4 == 0))	angle[3] = chassis_motor_info[3].angle;
//	
//	last_speed[0] = motor1;
//	last_speed[1] = motor2;
//	last_speed[2] = motor3;
//	last_speed[3] = motor4;
	//过零点处理
//	
	
	for( t = 0; t < 4; ++t)
	{
		//short a,pid_pos_out[4];
		if( motor_speed[t] == 0  && double_cir == TRUE)
		{
			pid_calc( &chassis_pos_pid[t], chassis_motor_info[t].angle, /*motor_pos[t]*/chassis_motor_info[t].angle_set);							//外环，位置环，实际值（机械角度），目标值（保存角度）
			pid_out[t] = pid_calc( &chassis_spd_pid[t], chassis_motor_info[t].rpm, chassis_pos_pid[t].pos_out);				//内环，速度环，实际值（目标速度），目标值（外环输出）
		}
		else
		{
			pid_out[t] = pid_calc( &chassis_spd_pid[t], chassis_motor_info[t].rpm, motor_speed[t]);			//单环
			chassis_motor_info[t].angle_set = chassis_motor_info[t].angle;
		}

		pid_out[t] *= Chassis_Power_Limit_Calc();			//功率限制
		//Beep_Off();
		
	}

//	for( t = 0; t < 4; ++t)
//	{
//		pid_out[t] = pid_calc( &chassis_spd_pid[t], chassis_motor_info[t].rpm, motor[t]);			//单环
//	}
	//pid_out[0] = 400;
//	printf("pid_out=%f\r\n",pid_out[0]);
	canbuf[0] = pid_out[0] >> 8;
	canbuf[1] = pid_out[0];
	canbuf[2] = pid_out[1] >> 8;
	canbuf[3] = pid_out[1];
	canbuf[4] = pid_out[2] >> 8;
	canbuf[5] = pid_out[2];
	canbuf[6] = pid_out[3] >> 8;
	canbuf[7] = pid_out[3];

	
	
	CAN1_Send_Msg( 0x200, canbuf, 8);		//发送数据
	
	
//	canbuf[0] = pid_out[4] >> 8;
//	canbuf[1] = pid_out[4];
//	canbuf[2] = pid_out[5] >> 8;
//	canbuf[3] = pid_out[5];
//	canbuf[4] = 0;
//	canbuf[5] = 0;
//	canbuf[6] = 0;
//	canbuf[7] = 0;

//	
//	
//	CAN1_Send_Msg( 0x1ff, canbuf, 8);		//发送数据
}





void Get_Chassis_Motor_Info( uint32_t motor_id, u8 *canbuf_receive)
{
	static u32 cnt = 0;
	short diff;
	
	if( (motor_id - 0x201 >= 0) && (motor_id - 0x201 < 4))
	{
		chassis_motor_info[motor_id - 0x201].last_real_angle = chassis_motor_info[motor_id - 0x201].real_angle;			//保存上次角度
		
		//读取当前信息
		chassis_motor_info[motor_id - 0x201].real_angle 	= canbuf_receive[0] << 8 | canbuf_receive[1];
		chassis_motor_info[motor_id - 0x201].rpm 			= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		chassis_motor_info[motor_id - 0x201].real_current 	= (signed short)(canbuf_receive[4] << 8 | canbuf_receive[5]);
		chassis_motor_info[motor_id - 0x201].temperature 	= canbuf_receive[6];//printf("\r\nid=%x,rmp=%d\r\n",motor_id,chassis_motor_info[motor_id - 0x201].rpm);
		
		
		//过零点处理
		diff = chassis_motor_info[motor_id - 0x201].real_angle - chassis_motor_info[motor_id - 0x201].last_real_angle;
		if( diff > 7500)
		{
			//--round_cnt;	//逆时针
			diff = -(8192 - diff);
		}
		else if( diff < -7500)
		{
			//++round_cnt;		//顺时针
			diff = (8192 + diff);
		}
		
		
		//排除第一次的数据	
		if( cnt == 0)
			diff = 0;				
		++cnt;	
		
		
		chassis_motor_info[motor_id - 0x201].angle += diff;
		
	}
	
}

