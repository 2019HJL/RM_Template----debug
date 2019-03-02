#include "ammunition_feed_ctrl.h"
#include "ammunition_feed.h"
#include "shooter_heat.h"
#include "DR16.h"



extern volatile int shoot_num;
extern uint32_t judge_data_cnt;


//拨弹初始化
void Ammunition_Init(void)
{
	Ammunition_PID_Init();
}


//拨弹控制
void Ammunition_Ctrl(void)
{
	static int ammunition_spd=0;		//拨弹电机速度
	static uint32_t jam_cnt = 0;		//卡弹计时
	static uint32_t jam_t = 0;			//卡弹，电机反转计时

	//判断是否拨弹（遥控器控制，热量限制）
	if( (RC_CtrlData.rc.s1 == 1 || RC_CtrlData.mouse.press_l == 1) && (shoot_num > 0 || judge_data_cnt > 1000) )
	{
		//卡弹反转
		if(jam_t > 0)
		{
			ammunition_spd = 3200;
		}
		//正常拨弹
		else
		{	
			//转速低于定值，说明卡弹
			if( ABS(ammunition_motor_info.rpm) < 20)
				++jam_cnt;
			else jam_cnt = 0;
			//设置反转时间
			if(jam_cnt > 2) jam_t = 50;
			
			ammunition_spd = -2500;	
		}
	}
	else ammunition_spd = 0;
	
	if(jam_t > 0) --jam_t;
	
	//设置电机速度
	Ammunition_Motor_Speed_Set( ammunition_spd);
}

