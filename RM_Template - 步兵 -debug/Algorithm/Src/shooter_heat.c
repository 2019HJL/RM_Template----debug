#include "shooter_heat.h"
#include "judge_data.h"


uint8_t Shooter17_Exceed_Limit_Flag = HEAT_UNEXCEED_LIMIT;
uint8_t Shooter42_Exceed_Limit_Flag = HEAT_UNEXCEED_LIMIT;

void Shoot_Heat_Calc(void)
{
	static uint16_t last_shooterHeat0 = 0,last_shooterHeat1 = 0;
	static uint16_t heat0[10] = {0}, heat0_index = 0, heat1[10] = {0}, heat1_index = 0;
	int16_t temp, heat0_avg, heat1_avg;
	u8 a;
	
	
	Shooter17_Exceed_Limit_Flag = HEAT_UNEXCEED_LIMIT;
	Shooter42_Exceed_Limit_Flag = HEAT_UNEXCEED_LIMIT;
	
	
	//计算17平均热量	
	temp = game_info.power_heat_data.shooterHeat0 - last_shooterHeat0;
	if( temp < 0)
		temp = -temp;
	
	if( heat0_index >=10)
		heat0_index = 0;
	
	if( temp != 0)
		heat0[ heat0_index] = temp;
	
	for( a = 0; a < 10 && heat0[a] != 0; ++a)
	{
		heat0_avg += temp;
	}
	heat0_avg /= a;
	
	last_shooterHeat0 = game_info.power_heat_data.shooterHeat0;
	
	
	//计算42平均热量	
	temp = game_info.power_heat_data.shooterHeat1 - last_shooterHeat1;
	if( temp < 0)
		temp = -temp;
	
	if( heat1_index >=10)
		heat1_index = 0;
	
	if( temp != 0)
		heat1[ heat1_index] = temp;
	
	for( a = 0; a < 10 && heat1[a] != 0; ++a)
	{
		heat1_avg += temp;
	}
	heat1_avg /= a;
	
	last_shooterHeat1 = game_info.power_heat_data.shooterHeat1;
	
	
	//热量超限计算，等级
	
	//步兵，英雄
#if defined (RM_INFANTRY) || defined (RM_HERO)
	switch( game_info.state.robotLevel)
	{
		case 1:
			if( game_info.power_heat_data.shooterHeat0 + heat0_avg >= SHOOTER_HEAT_LIMIT_17_LEVEL1)
				Shooter17_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			if( game_info.power_heat_data.shooterHeat1 + heat1_avg >= SHOOTER_HEAT_LIMIT_42_LEVEL1)
				Shooter42_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			break;
		case 2:
			if( game_info.power_heat_data.shooterHeat0 + heat0_avg >= SHOOTER_HEAT_LIMIT_17_LEVEL2)
				Shooter17_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			if( game_info.power_heat_data.shooterHeat1 + heat1_avg >= SHOOTER_HEAT_LIMIT_42_LEVEL2)
				Shooter42_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			break;
		case 3:
			if( game_info.power_heat_data.shooterHeat0 + heat0_avg >= SHOOTER_HEAT_LIMIT_17_LEVEL3)
				Shooter17_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			if( game_info.power_heat_data.shooterHeat1 + heat1_avg >= SHOOTER_HEAT_LIMIT_42_LEVEL3)
				Shooter42_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
			break;
	}

#endif
	
	//哨兵
#if defined (RM_SENTRY)
	if( game_info.power_heat_data.shooterHeat0 + heat0_avg >= SHOOTER_HEAT_LIMIT_SENTRY)
		Shooter17_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
#endif
	
	//空中
#if defined (RM_AIR)
	if( game_info.power_heat_data.shooterHeat0 + heat0_avg >= SHOOTER_HEAT_LIMIT_AIR)
		Shooter17_Exceed_Limit_Flag = HEAT_EXCEED_LIMIT;
#endif
	
//	game_info.power_heat_data.shooterHeat0;
}

