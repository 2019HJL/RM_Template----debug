#include "ramp.h"
#include "tim.h"

volatile u32 ramp_cnt = 0;		//�ڶ�ʱ����������Ϊб�º����ṩ֧��

//б�º���ʹ�÷��������chassis_ctrl.c�м��̿��Ʋ���


//б�¿�ʼ
void Ramp_Set( Ramp *ramp)
{
	ramp->time = ramp_cnt;
}



//б�º�������
float Ramp_Calc( Ramp *ramp, u32 time, u8 dir)
{
	u32 dif;
	
	dif = ramp_cnt - ramp->time;
	ramp->time = ramp_cnt;
	
	switch( dir)
	{
		case RAMP_UP:
			ramp->ramp_out += ( float)dif * 5.0f / ( float)time;
		break;
		case RAMP_DOWN:
			ramp->ramp_out -= ( float)dif * 5.0f / ( float)time;
		break;
	}
	
//	printf("dif/time=%f\r\n",( float)dif / ( float)time);
	
	if( ramp->ramp_out > 1.0f)
	{
		ramp->ramp_out = 1.0f;
		return 1.0f;	
	}
	if( ramp->ramp_out < 0.0f)
	{
		ramp->ramp_out = 0.0f;
		return 0.0f;
	}
		
	return ramp->ramp_out;
	
//	if( dif * 5 > time)
//		return 1.0f;
//	
//	return dif * 5.0f / time;
}





