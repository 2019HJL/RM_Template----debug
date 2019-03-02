#include "gimbal_ctrl.h"
#include "gimbal.h"
#include "DR16.h"
#include "judge_data.h"
#include "chassis_ctrl.h"
#include "usart.h"
#include "report.h"

pid_t gimbal_follow_pid;

pid_t gimbal_yaw_vision_pid;
pid_t gimbal_pitch_vision_pid;

void Gimbal_Init(void)
{
	Gimbal_Pid_Init();
	
	pid_init( &gimbal_follow_pid, POSITION_PID, 20.0f, 20.0f, 0.04f, 0.000002f,0.000000002f);//0.00002f);
	
	pid_init( &gimbal_yaw_vision_pid, POSITION_PID, 10.0f, 10.0f, 0.03f, 0.0001f, 0.0f);
	pid_init( &gimbal_pitch_vision_pid, POSITION_PID, 10.0f, 10.0f, 0.03f, 0.0001f, 0.0f);
}


void Gimbal_Ctrl(void)
{
	float pitch_change, yaw_change;
//	int  pitch_motor_pos, yaw_motor_pos;
	
	u8 follow_state;		//视觉跟随用
	
	//遥控器映射
	pitch_change 	= ( (float)RC_CtrlData.rc.ch1 - 1024.0f) * ( 80.0f / 660.0f);		
	yaw_change 		= -( (float)RC_CtrlData.rc.ch0 - 1024.0f) * ( 220.0f / 660.0f);
	

	
	//视觉跟随（暂时无用）
	if( RC_CtrlData.mouse.press_r == 1)
		follow_state = SET;
	else follow_state = RESET;
	
	if(RC_CtrlData.rc.s2 == 1)
	{
		pitch_change = RC_CtrlData.mouse.y * 0.75;
		yaw_change   = -RC_CtrlData.mouse.x * 10;	
	}

	
	
//	yaw_change   = -Gimbal_Follow( follow_state, RC_CtrlData.mouse.x * 0.75, vision_get.Yaw * 8192.0f / 360.0f);	
//	pitch_change = -( RC_CtrlData.mouse.y * 0.5 + vision_get.Pitch * 8192.0f / 360.0f);//-Gimbal_Follow( /*follow_state*/RESET, RC_CtrlData.mouse.y * 0.5, 0);
	
	
	//限速（车确定后加）
//	if( yaw_change > YAW_CHANGE_MAX)
//		yaw_change  = YAW_CHANGE_MAX;
//	else if( yaw_change < -YAW_CHANGE_MAX)
//		yaw_change = -YAW_CHANGE_MAX;
//	
//	if( pitch_change > PITCH_CHANGE_MAX)
//		pitch_change  = PITCH_CHANGE_MAX;
//	else if( pitch_change < -PITCH_CHANGE_MAX)
//		pitch_change = -PITCH_CHANGE_MAX;

	

	gimbal_motor_info[0].angle_set += (int)yaw_change;
	gimbal_motor_info[1].angle_set += (int)pitch_change;
	
//	//限位（车确定后加）
//	if( gimbal_motor_info[0].angle_set > YAW_LIMIT)		//yaw
//		gimbal_motor_info[0].angle_set = YAW_LIMIT;
//	else if( gimbal_motor_info[0].angle_set < -YAW_LIMIT)
//		gimbal_motor_info[0].angle_set = -YAW_LIMIT;
//	
//	if( gimbal_motor_info[1].angle_set > PITCH_LIMIT)	//pitch
//		gimbal_motor_info[1].angle_set = PITCH_LIMIT;
//	else if( gimbal_motor_info[1].angle_set < -PITCH_LIMIT)
//		gimbal_motor_info[1].angle_set = -PITCH_LIMIT;
	

	
	Gimbal_Motor_Speed_Set( gimbal_motor_info[0].angle_set, gimbal_motor_info[1].angle_set);
}




//云台跟随
float Gimbal_Follow( u8 Command, float RC_Data, float Vision_Data)
{
	static int yaw_angle_set = 0, yaw_angle = 0;
	static float last_real_yaw;
//	static u8 last_command = RESET;
	static u32 cnt = 0;
	float diff;
	
	float temp;
	
//	Vision_Data = 0;
	
	//控制，修正数据融合
	yaw_angle_set += RC_Data + Vision_Data;
	


	diff = game_info.pos.yaw - last_real_yaw;
	last_real_yaw = game_info.pos.yaw;
	
	//排除第一次的数据	
	if( cnt == 0)
		diff = 0;				
	++cnt;		
	
	
	//过零点处理
	if( diff > 180.0f)
	{
		diff = ( 360.0f - diff);
	}
	else if( diff < -180.0f)
	{
		diff = -( 360.0f + diff);
	}
	
	
	
	diff *= 8192.0f / 360.0f;
	
	yaw_angle += diff;	
	
	temp = pid_calc( &gimbal_follow_pid, yaw_angle, yaw_angle_set);
	Report_Add( (short)(temp * 100));

	
	if( Command == SET)			
	{		
		return temp;
		//return (float)((float)(yaw_angle_set - yaw_angle)/10.0f);		
	}

	return RC_Data + Vision_Data;
 

//	else return RC_Data + Vision_Data;

}





