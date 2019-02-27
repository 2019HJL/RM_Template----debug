#ifndef RAMP_H_INCLUDED
#define RAMP_H_INCLUDED

#include "sys.h"

#define RAMP_UP		1			//��
#define RAMP_DOWN	2			//��

typedef struct
{
	u32 time;
	float ramp_out;
} Ramp;


void Ramp_Set( Ramp *ramp);								//б�¿�ʼ
float Ramp_Calc( Ramp *ramp, u32 time, u8 dir);			//б�¼��㣬time��λΪms			

extern volatile u32 ramp_cnt;




#endif

