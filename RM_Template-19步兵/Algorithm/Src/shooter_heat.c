#include "shooter_heat.h"
#include "judge_data.h"

#include "beep.h"
#include "usart.h"

uint8_t Shooter17_Exceed_Limit_Flag;		//17mm�����������ޱ�־
uint8_t Shooter42_Exceed_Limit_Flag;		//42mm�����������ޱ�־

uint16_t shoot_speed[6];

volatile int shoot_num = 10;


//�������Ƽ��㣬Ŀǰֻд��17mm��
void Shoot_Heat_Limit(void)
{
	if(shoot_speed[5] != 0)
	{
		//����������������
		shoot_num = (SHOOTER_HEAT_LIMIT_17_LEVEL1 - game_info.power_heat_data.shooterHeat0) / shoot_speed[5];
		if( shoot_num < 0)
			shoot_num = 0;
		if(shoot_num >= 0 && shoot_num < 10)
			Shooter17_Exceed_Limit_Flag = RESET;
		else Shooter17_Exceed_Limit_Flag = SET;
	}
	else 
	{	Shooter17_Exceed_Limit_Flag = RESET;
		shoot_num = 10;
	}
//	printf("%d,%d,%d\r\n",shoot_num,shoot_speed[5],Shooter17_Exceed_Limit_Flag);
}



//���浯���ٶȣ���ƽ��ֵ
void Shoot_Data_Save( uint16_t new_shoot_speed)
{
	uint8_t i;
	uint16_t sum = 0;
	for( i = 1; i < 5; ++i)
	{
		shoot_speed[i-1] = shoot_speed[i];
		sum += shoot_speed[i-1];
	}
	shoot_speed[i-1] = new_shoot_speed;
	sum += shoot_speed[i-1];
	
	shoot_speed[i] = sum / i;
}

