#include "beep.h"
#include "pwm.h"



void Beep_Init(void)
{
	PWM_Beep_Init();
}


void Beep_On(void)
{
	TIM_Cmd( TIM3, ENABLE);				//Ê¹ÄÜTIM3
	TIM_SetCompare1( TIM3, 64);			//50%PWM
}


void Beep_Off(void)
{	
	TIM_SetCompare1( TIM3, 0);			//0%PWM
	TIM_Cmd( TIM3, DISABLE);				//Ê§ÄÜTIM3

}
