#include "chassis_ctrl.h"
#include "DR16.h"
#include "chassis.h"
#include "usart.h"
#include "gimbal.h"
#include "gimbal_ctrl.h"
#include "ramp.h"


Ramp chassis_x_ramp;			//底盘x斜坡
Ramp chassis_y_ramp;			//底盘y斜坡
Ramp chassis_yaw_ramp;			//底盘yaw斜坡

pid_t chassis_follow_pid;

void Chassis_Init(void)
{
	Chassis_Pid_Init();
	
	pid_init( &chassis_follow_pid, POSITION_PID, 1, 1, 0.042f, 0.01f, 0.0f);
}



void Chassis_Ctrl(void)
{
	u8 t;
	short motor[4];
//	int pos[4];
	float chassis_spd[4];
	static float vx,vy,vyaw;
//	static u8 chassis_swing_state = RESET;

//	
	vx 		= (float)(RC_CtrlData.rc.ch3 - 1024) * ( CHASSIS_VX_MAX / 660.0f);			//遥控器数据映射到速度
	vy 		= (float)(RC_CtrlData.rc.ch2 - 1024) * ( CHASSIS_VY_MAX / 660.0f);			//遥控器数据映射到速度
	if( RC_CtrlData.rc.s1 == 2)
		vyaw 	= (float)(RC_CtrlData.rc.ch0 - 1024) * ( CHASSIS_VYAW_MAX / 660.0f);		//遥控器数据映射到速度
	else vyaw = 0;
	
	
//////	//w按下动作
//////	if( RC_Key_Action_Read( RC_KEY_W, RC_ACTION_KEY_PRESS) == 1)
//////	{	
//////		Ramp_Set( &chassis_x_ramp);
//////		//printf("w down action.\r\n");
//////	}
//////	else if( RC_Key_Action_Read( RC_KEY_S, RC_ACTION_KEY_PRESS) == 1)//s按下动作
//////	{
//////		Ramp_Set( &chassis_x_ramp);
//////		//printf("s down action.\r\n");

//////	}
//////	//a按下动作
//////	if( RC_Key_Action_Read( RC_KEY_A, RC_ACTION_KEY_PRESS) == 1)
//////	{	
//////		Ramp_Set( &chassis_y_ramp);
//////		//printf("w down action.\r\n");
//////	}
//////	else if( RC_Key_Action_Read( RC_KEY_D, RC_ACTION_KEY_PRESS) == 1)//d按下动作
//////	{
//////		Ramp_Set( &chassis_y_ramp);
//////		//printf("s down action.\r\n");

//////	}
//////	//q按下动作
//////	if( RC_Key_Action_Read( RC_KEY_Q, RC_ACTION_KEY_PRESS) == 1)
//////	{	
//////		Ramp_Set( &chassis_yaw_ramp);
//////		//printf("w down action.\r\n");
//////	}
//////	else if( RC_Key_Action_Read( RC_KEY_E, RC_ACTION_KEY_PRESS) == 1)//e按下动作
//////	{
//////		Ramp_Set( &chassis_yaw_ramp);
//////		//printf("s down action.\r\n");

//////	}
//////	
//////	
//////	
//////	//w按下
//////	if( RC_Key_Read( RC_KEY_W) == 1 && vx >= 0.0f)
//////	{
//////		vx = 1.0f * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_UP);
////////		printf("w down\r\n");
//////	}
//////	else if( RC_Key_Read( RC_KEY_S) == 1 && vx <= 0.0f)//s按下
//////	{
//////		vx = -1.0f * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_UP);
////////		printf("s down\r\n");
//////	}
//////	
//////	//ws抬起
//////	if( RC_Key_Read( RC_KEY_W) == 0 || RC_Key_Read( RC_KEY_S) == 0)
//////	{
//////		if( vx > 0.0f)
//////			vx = 1.0f * Ramp_Calc( &chassis_x_ramp, 600, RAMP_DOWN);
//////		if( vx < 0.0f)
//////			vx = -1.0f * Ramp_Calc( &chassis_x_ramp, 600, RAMP_DOWN);
//////		//printf("ws key up\r\n");

//////	}
//////	
//////	
//////	

//////	
//////	//a按下
//////	if( RC_Key_Read( RC_KEY_A) == 1 && vy <= 0.0f)
//////	{
//////		vy = -1.0f * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_UP);
////////		printf("a down\r\n");
//////	}
//////	else if( RC_Key_Read( RC_KEY_D) == 1 && vy >= 0.0f)//d按下
//////	{
//////		vy = 1.0f * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_UP);
////////		printf("d down\r\n");
//////	}
//////	
//////	//ad抬起
//////	if( RC_Key_Read( RC_KEY_A) == 0 || RC_Key_Read( RC_KEY_D) == 0)
//////	{
//////		if( vy > 0.0f)
//////			vy = 1.0f * Ramp_Calc( &chassis_y_ramp, 600, RAMP_DOWN);
//////		if( vy < 0.0f)
//////			vy = -1.0f * Ramp_Calc( &chassis_y_ramp, 600, RAMP_DOWN);
//////		//printf("ad key up\r\n");

//////	}
	
	//printf("vy=%f, y_ramp=%f\r\n", vy, chassis_y_ramp.ramp_out);
	
	
	
	
	
//	//q按下
//	if( RC_Key_Read( RC_KEY_Q) == 1  /*RC_CtrlData.key.v == 64*/ && vyaw <= 0.0f)
//	{
//		vyaw = -0.4f * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_UP);
//		printf("q down\r\n");
//	}
//	else if( RC_Key_Read( RC_KEY_E) == 1 && vyaw >= 0.0f)	//e按下
//	{
//		vyaw = 0.4f * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_UP);
//		printf("e down,vyaw>=0 =%d\r\n",(vyaw>=0.0f));
//	}
	
//	//qe抬起
//	if( RC_Key_Read( RC_KEY_Q) == 0 || RC_Key_Read( RC_KEY_E) == 0)
//	//if( (RC_CtrlData.key.v & KEY_PRESSED_OFFSET_Q) ==0 && (RC_CtrlData.key.v & KEY_PRESSED_OFFSET_E) == 0)
//	{
//		if( vyaw > 0.0f)
//			vyaw = 0.4f * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_DOWN);
//		if( vyaw < 0.0f)
//			vyaw = -0.4f * Ramp_Calc( &chassis_yaw_ramp, 200, RAMP_DOWN);
//		//printf("qe key up,key=%x\r\n",RC_CtrlData.key.v);

//	}


//	//e按下
//	if( RC_Key_Action_Read( RC_KEY_E, RC_ACTION_KEY_PRESS) == 1)
//	{
//		if( chassis_swing_state == RESET)
//		{		
//			chassis_swing_state = SET;
//		}
//		else if( chassis_swing_state == SET)
//		{
////			vyaw = Chassis_Swing( chassis_swing_state, 0.4f);
//			chassis_swing_state = RESET;
//		}
//	}
//	vyaw = Chassis_Swing( chassis_swing_state, 0.4f);
//	
//	if( chassis_swing_state == RESET)

//		vyaw = Chassis_Follow();

//		printf("vyaw=%f\r\n",vyaw);




	//printf("vyaw=%f,vyaw<=0=%d,vyaw>=0 =%d\r\n",vyaw,(vyaw<=0.0f),(vyaw>=0.0f));
	
//	printf("vx=%f,vy=%f,vyaw=%f,x_ramp=%f,y=%f, yaw_ramp=%f,key=0x%x\r\n",vx,vy, vyaw, chassis_x_ramp.ramp_out, chassis_y_ramp.ramp_out, chassis_yaw_ramp.ramp_out, RC_CtrlData.key.v);
	

//	if( RC_CtrlData.rc.s1 == 1)		vyaw = -0.35f;
//	if( RC_CtrlData.rc.s1 == 3)		vyaw = 0;
//	if( RC_CtrlData.rc.s1 == 2)     vyaw = 0.35f;
//	
//	if( RC_CtrlData.rc.s1 == 1)		vyaw = Chassis_Swing( SET, 0.4f);
//	if( RC_CtrlData.rc.s1 == 3)		vyaw = Chassis_Swing( RESET, 0.0f);
//	if( RC_CtrlData.rc.s1 == 2)     vyaw = Chassis_Swing( SET, 0.2f);
//	printf("yaw_speed=%f,gimbal_motor_info[0].angle_set=%d\r\n",vyaw,gimbal_motor_info[0].angle_set);
	
//	chassis_spd[0] = (v + vyaw);
//	chassis_spd[1] = (v + vyaw);
//	chassis_spd[2] = (v + vyaw);
//	chassis_spd[3] = -(v - vyaw);
//	chassis_spd[4] = -(v - vyaw);
//	chassis_spd[5] = -(v - vyaw);
	
//	chassis_spd[0] = (vy - vx + vyaw * ( CHASSIS_A + CHASSIS_B));			//右前
//	chassis_spd[1] = vy + vx + vyaw * ( CHASSIS_A + CHASSIS_B);			//左前
//	chassis_spd[2] = -(vy - vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//左后
//	chassis_spd[3] = -(vy + vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//右后


	chassis_spd[3] = (vy - vx + vyaw * ( CHASSIS_A + CHASSIS_B));			//右后
	chassis_spd[2] = vy + vx + vyaw * ( CHASSIS_A + CHASSIS_B);				//左后
	chassis_spd[1] = -(vy - vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//左前
	chassis_spd[0] = -(vy + vx - vyaw * ( CHASSIS_A + CHASSIS_B));			//右前
	
	
	for( t = 0; t < 4; ++t)
	{
		motor[t] = chassis_spd[t] * 2 * 19 * 60;			//速度转化为转速
		/*pos[t]*/
//		chassis_motor_info[t].angle_set += chassis_spd[t] * 2000  ;		//速度转换为位置变化
//		pos[t] = chassis_motor_info[t].angle_set;
	}
	//printf("vx=%f,chassis_spd[0]=%f,motor[0]=%d\r\n", vyaw,chassis_spd[0],motor[0]);
	Chassis_Motor_Speed_Set( /*pos);*/motor);
	
}



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
				if( gimbal_motor_info[0].angle_set > YAW_LIMIT)
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
				if( gimbal_motor_info[0].angle_set < -YAW_LIMIT)
				{
					dir = CHASSIS_SWING_CLOCKWISE;
				}
					temp = chassis_yaw_speed;
//					return -chassis_yaw_speed;
				
			}
				break;
			default :
			{
				if( gimbal_motor_info[0].angle_set > YAW_LIMIT)
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




float Chassis_Follow( void)
{
	if( gimbal_motor_info[0].angle_set > 150 || gimbal_motor_info[0].angle_set < -150)
	{
		return pid_calc( &chassis_follow_pid, gimbal_motor_info[0].angle_set, 0.0f);
	}
	
	return 0.0f;
}



