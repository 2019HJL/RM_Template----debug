#include "friction.h"
#include "pwm.h"
#include "delay.h"

//void Friction_Init()
//{
//	PWM_Init( PWM_PA0, 400, 20000, TIM_OCPolarity_High);
//}


//Ħ���ֵĵ����һ��ʹ��ʱ��Ҫ���������г�
void Friction_Range_Set( u32 pwm_gpio)
{
	PWM_Set( pwm_gpio, 2000 - 1);
	delay_ms(2000);
	PWM_Set( pwm_gpio, 1000);
	delay_ms(1000);
}


//999Ϊ�������
void Friction_Speed_Set( u32 pwm_gpio, u32 scale)
{
	if( scale >= 1000)
		scale = 999;
	PWM_Set( pwm_gpio, scale + 1000);
}


//PA0,PA1Ħ�����г��趨
void Friction_Range_Set_Fast(void)
{
	PWM_Set( PWM_PA0, 2000 - 1);
	PWM_Set( PWM_PA1, 2000 - 1);
	delay_ms(2000);
	PWM_Set( PWM_PA0, 1000);
	PWM_Set( PWM_PA1, 1000);
	delay_ms(1000);
}


