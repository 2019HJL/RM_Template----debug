#ifndef FRICTION_H_INCLUDED
#define FRICTION_H_INCLUDED

#include "sys.h"

//void Friction_Init();
void Friction_Range_Set( u32 pwm_gpio);					//�����г��趨��Ħ���ֵĵ����һ��ʹ��ʱ��Ҫ���������г�
void Friction_Speed_Set( u32 pwm_gpio, u32 scale);		//���ţ����ֵ999
void Friction_Range_Set_Fast(void);						////PA0,PA1Ħ�����г��趨������



#endif // FRICTION_H_INCLUDED
