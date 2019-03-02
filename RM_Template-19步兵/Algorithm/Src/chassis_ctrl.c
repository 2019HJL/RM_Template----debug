#include "chassis_ctrl.h"
#include "DR16.h"
#include "chassis.h"
#include "usart.h"
#include "gimbal.h"
#include "gimbal_ctrl.h"
#include "ramp.h"
#include "chassis_power.h"

#include "tim.h"

#include <math.h>

extern volatile uint32_t judge_data_cnt;

Ramp chassis_x_ramp;			//底盘x斜坡
Ramp chassis_y_ramp;			//底盘y斜坡
Ramp chassis_yaw_ramp;			//底盘yaw斜坡

pid_t chassis_follow_pid;

void Chassis_Ctrl_k();		//键盘控制底盘


void Chassis_Init(void)
{
	Chassis_Pid_Init();
	
	//底盘跟随pid
	pid_init( &chassis_follow_pid, POSITION_PID, 2, 1, /*0.0013f*/0.0026f, 0.0f, 0.00001f);
}


static float vx,vy,vyaw;

void Chassis_Ctrl(void)
{
	u8 t;
	short motor[4];
	float chassis_spd[4];

	float current[4],last_chassis_spd[4]={0},accel[4],accel_limit=1.0f,accel_sum=0;;

	if(RC_CtrlData.rc.s2 == RC_SW_DOWN)
	{
		vyaw = Chassis_Follow();
	}
	else if(RC_CtrlData.rc.s2 == RC_SW_MID)
	{
		//Chassis_Ctrl_k();		
		vyaw = -Chassis_Swing(SET,1.2f);
	}
	else
	{	
		vyaw = 0;
	}

	//遥控器数据映射到速度（底盘无头模式）
	//前进后退
	vx =  (float)(RC_CtrlData.rc.ch3 - 1024) * ( CHASSIS_VX_MAX / 660.0f) * cos( (float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f) 
		- (float)(RC_CtrlData.rc.ch2 - 1024) * ( CHASSIS_VX_MAX / 660.0f) * sin( -(float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f);
	//左右平移		
	vy = -(float)(RC_CtrlData.rc.ch3 - 1024) * ( CHASSIS_VX_MAX / 660.0f) * sin( (float)(gimbal_motor_info[0].angle /*-GIMBAL_OFFSET*/)/8192.0f * PI*2.0f) 
		+ (float)(RC_CtrlData.rc.ch2 - 1024) * ( CHASSIS_VX_MAX / 660.0f) * cos( -(float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f);
	
	//遥控器数据映射到速度（普通）
//		vx 		= (float)(RC_CtrlData.rc.ch3 - 1024) * ( CHASSIS_VX_MAX / 660.0f);			
//		vy 		= (float)(RC_CtrlData.rc.ch2 - 1024) * ( CHASSIS_VY_MAX / 660.0f);			

	if(RC_CtrlData.rc.s2 == RC_SW_UP)
	{
		Chassis_Ctrl_k();			//键盘控制
		vyaw = Chassis_Follow();	//底盘跟随（云台）
	}
	

	//六轮
//	chassis_spd[0] = (v + vyaw);
//	chassis_spd[1] = (v + vyaw);
//	chassis_spd[2] = (v + vyaw);
//	chassis_spd[3] = -(v - vyaw);
//	chassis_spd[4] = -(v - vyaw);
//	chassis_spd[5] = -(v - vyaw);
	
	//保存上次速度以计算加速度
	for( t = 0; t < 4; ++t)
	{
		last_chassis_spd[t] = chassis_spd[t];
	}
	
	//四麦克纳姆轮
	chassis_spd[3] = (vy - vx + vyaw * ( CHASSIS_A + CHASSIS_B));			//右前
	chassis_spd[2] = vy + vx + vyaw * ( CHASSIS_A + CHASSIS_B);				//左前
	chassis_spd[1] = -(vy - vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//左后
	chassis_spd[0] = -(vy + vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//右后

	//裁判系统连接时限制功率（速度，加速度）
	if(judge_data_cnt < 1000)
	{
		//计算加速度
		for( t = 0; t < 4; ++t)
		{
			accel[t] = (chassis_spd[t] - last_chassis_spd[t]);
		}
		//限制加速度
		for( t = 0; t < 4; ++t)
		{	                       
			accel_sum += ABS(accel[t]);
		}	
		if(accel_sum > ACCELERATION_SUM_MAX)
			accel_limit =  ACCELERATION_SUM_MAX / ( accel_sum + 0.5f);
			
		for( t = 0; t < 4; ++t)
		{
			accel[t] *= accel_limit;
		}
		for( t = 0; t < 4; ++t)
		{
			chassis_spd[t] = last_chassis_spd[t] + accel[t];
		}
		//功率限制（超功率降速）
		Chassis_Power_Limit( chassis_spd);
	}
	
	//发送控制信息
	for( t = 0; t < 4; ++t)
	{
		motor[t] = chassis_spd[t] * 2 * 19 * 60;			//速度转化为转速
	}
	Chassis_Motor_Speed_Set( motor);
}





//键盘控制
void Chassis_Ctrl_k()
{
	//w按下动作
	if( RC_Key_Action_Read( RC_KEY_W, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_x_ramp);
		//printf("w down action.\r\n");
	}//s按下动作
	else if( RC_Key_Action_Read( RC_KEY_S, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_x_ramp);
		//printf("s down action.\r\n");

	}
	//a按下动作
	if( RC_Key_Action_Read( RC_KEY_A, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_y_ramp);
		//printf("w down action.\r\n");
	}//d按下动作
	else if( RC_Key_Action_Read( RC_KEY_D, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_y_ramp);
		//printf("s down action.\r\n");

	}
	//q按下动作
	if( RC_Key_Action_Read( RC_KEY_Q, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_yaw_ramp);
		//printf("w down action.\r\n");
	}//e按下动作
	else if( RC_Key_Action_Read( RC_KEY_E, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_yaw_ramp);
		//printf("s down action.\r\n");

	}
	
	
	
	//w按下
	if( RC_Key_Read( RC_KEY_W) == 1 && vx >= 0.0f)
	{
		vx = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_UP);
//		printf("w down\r\n");
	}//s按下
	else if( RC_Key_Read( RC_KEY_S) == 1 && vx <= 0.0f)
	{
		vx = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_UP);
//		printf("s down\r\n");
	}
	
	//ws抬起
	if( RC_Key_Read( RC_KEY_W) == 0 || RC_Key_Read( RC_KEY_S) == 0)
	{
		if( vx > 0.0f)
			vx = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 600, RAMP_DOWN);
		if( vx < 0.0f)
			vx = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 600, RAMP_DOWN);
		//printf("ws key up\r\n");

	}
	
	
	

	
	//a按下
	if( RC_Key_Read( RC_KEY_A) == 1 && vy <= 0.0f)
	{
		vy = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_UP);
//		printf("a down\r\n");
	}//d按下
	else if( RC_Key_Read( RC_KEY_D) == 1 && vy >= 0.0f)
	{
		vy = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_UP);
//		printf("d down\r\n");
	}
	
	//ad抬起
	if( RC_Key_Read( RC_KEY_A) == 0 || RC_Key_Read( RC_KEY_D) == 0)
	{
		if( vy > 0.0f)
			vy = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 600, RAMP_DOWN);
		if( vy < 0.0f)
			vy = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 600, RAMP_DOWN);
		//printf("ad key up\r\n");

	}
	
	//printf("vy=%f, y_ramp=%f\r\n", vy, chassis_y_ramp.ramp_out);
	
	
	
	
	
	//q按下
	if( RC_Key_Read( RC_KEY_Q) == 1  /*RC_CtrlData.key.v == 64*/ && vyaw <= 0.0f)
	{
		vyaw = -CHASSIS_VYAW_KEY_MAX * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_UP);
		printf("q down\r\n");
	}
	else if( RC_Key_Read( RC_KEY_E) == 1 && vyaw >= 0.0f)	//e按下
	{
		vyaw = CHASSIS_VYAW_KEY_MAX * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_UP);
		printf("e down,vyaw>=0 =%d\r\n",(vyaw>=0.0f));
	}
	
	//qe抬起
	if( RC_Key_Read( RC_KEY_Q) == 0 || RC_Key_Read( RC_KEY_E) == 0)
	//if( (RC_CtrlData.key.v & KEY_PRESSED_OFFSET_Q) ==0 && (RC_CtrlData.key.v & KEY_PRESSED_OFFSET_E) == 0)
	{
		if( vyaw > 0.0f)
			vyaw = CHASSIS_VYAW_KEY_MAX * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_DOWN);
		if( vyaw < 0.0f)
			vyaw = -CHASSIS_VYAW_KEY_MAX * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_DOWN);
		//printf("qe key up,key=%x\r\n",RC_CtrlData.key.v);

	}
	
//	vyaw = 0.04 * RC_CtrlData.mouse.x;


}




//扭腰（云台为轴，底盘左右等幅度转动）
float Chassis_Swing( u8 Command, float chassis_yaw_speed)
{
	static u8 dir ;//= CHASSIS_SWING_CLOCKWISE;
	float temp ;
//printf("dir = %d\r\n", dir);
	if( Command == SET)
	{
		//不细分方向，达到扭腰目的即可
		switch( dir)
		{
			case CHASSIS_SWING_CLOCKWISE:
			{
				//底盘与云台达一定角度，底盘转动方向换向
				if( gimbal_motor_info[0].angle > CHASSIS_SWING_ANGLE)
				{
					dir = CHASSIS_SWING_ANTICLOCKWISE;
				}
					temp = - chassis_yaw_speed;
//					printf("llll\r\n");
//					return -chassis_yaw_speed;
				
			}
				break;
			case CHASSIS_SWING_ANTICLOCKWISE:
			{
				if( gimbal_motor_info[0].angle < -CHASSIS_SWING_ANGLE)
				{
					dir = CHASSIS_SWING_CLOCKWISE;
				}
					temp = chassis_yaw_speed;
//					return -chassis_yaw_speed;
				
			}
				break;
			default :
			{
				if( gimbal_motor_info[0].angle > CHASSIS_SWING_ANGLE)
				{
					dir = CHASSIS_SWING_ANTICLOCKWISE;
				}
					temp = - chassis_yaw_speed;
//					return -chassis_yaw_speed;
				
			}
				break;
				
		}
	}
	else 
	{
		dir = CHASSIS_SWING_CLOCKWISE;
		temp = - chassis_yaw_speed;

//		return -chassis_yaw_speed;
	}
	return temp;
}



//底盘跟随（云台）
float Chassis_Follow( void)
{
	//printf("%d\r\n",gimbal_motor_info[0].angle);
	if( gimbal_motor_info[0].angle > CHASSIS_FOLLOW_ANGLE_ALLOWANCE || gimbal_motor_info[0].angle < -CHASSIS_FOLLOW_ANGLE_ALLOWANCE)		//跟随条件（暂时留出了少量余量）
	{
		return pid_calc( &chassis_follow_pid, gimbal_motor_info[0].angle, 0);
	}
	
	return 0.0f;
}



