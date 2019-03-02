#include "laser.h"


volatile int Laser_state;


//�����ʼ��
void Laser_Init(void)
{
	GPIO_ResetBits( GPIOG, GPIO_Pin_13);
}

//�������
void Laser_Ctrl( u8 laser_state)
{
	Laser_state = laser_state;
	
	if( laser_state == LASER_SWITCH_ON)
		GPIO_SetBits( GPIOG, GPIO_Pin_13);
	else GPIO_ResetBits( GPIOG, GPIO_Pin_13);
}

//����״̬��ת
void Laser_Switch(void)
{
	if( Laser_state == LASER_SWITCH_ON)
	{
		GPIO_ResetBits( GPIOG, GPIO_Pin_13);
		Laser_state = LASER_SWITCH_OFF;
	}
	else 
	{
		GPIO_SetBits( GPIOG, GPIO_Pin_13);
		Laser_state = LASER_SWITCH_ON;
	}	
}

