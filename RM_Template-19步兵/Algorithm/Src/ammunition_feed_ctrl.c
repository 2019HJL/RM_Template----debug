#include "ammunition_feed_ctrl.h"
#include "ammunition_feed.h"
#include "shooter_heat.h"
#include "DR16.h"



extern volatile int shoot_num;
extern uint32_t judge_data_cnt;


//������ʼ��
void Ammunition_Init(void)
{
	Ammunition_PID_Init();
}


//��������
void Ammunition_Ctrl(void)
{
	static int ammunition_spd=0;		//��������ٶ�
	static uint32_t jam_cnt = 0;		//������ʱ
	static uint32_t jam_t = 0;			//�����������ת��ʱ

	//�ж��Ƿ񲦵���ң�������ƣ��������ƣ�
	if( (RC_CtrlData.rc.s1 == 1 || RC_CtrlData.mouse.press_l == 1) && (shoot_num > 0 || judge_data_cnt > 1000) )
	{
		//������ת
		if(jam_t > 0)
		{
			ammunition_spd = 3200;
		}
		//��������
		else
		{	
			//ת�ٵ��ڶ�ֵ��˵������
			if( ABS(ammunition_motor_info.rpm) < 20)
				++jam_cnt;
			else jam_cnt = 0;
			//���÷�תʱ��
			if(jam_cnt > 2) jam_t = 50;
			
			ammunition_spd = -2500;	
		}
	}
	else ammunition_spd = 0;
	
	if(jam_t > 0) --jam_t;
	
	//���õ���ٶ�
	Ammunition_Motor_Speed_Set( ammunition_spd);
}

