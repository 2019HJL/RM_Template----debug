#include "laser.h"


void Laser_Init(void)
{
	GPIO_ResetBits( GPIOG, GPIO_Pin_13);
}


void Laser_Ctrl( u8 laser_state)
{
	if( laser_state == LASER_SWITCH_ON)
		GPIO_SetBits( GPIOG, GPIO_Pin_13);
	else GPIO_ResetBits( GPIOG, GPIO_Pin_13);
}
