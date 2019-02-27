#include "chassis_power.h"
#include "judge_data.h"
#include "chassis.h"
#include "beep.h"
#include "usart.h"


float Chassis_Power_Limit_Calc(void)
{
	//printf("%f,%f\r\n",(float)CHASSIS_POWER_LIMITED_INFANTRY / (float)game_info.power_heat_data.chassisPower, game_info.power_heat_data.chassisPower);
	

		
		//²½±ø
#if defined (RM_INFANTRY)
	if( game_info.power_heat_data.chassisPower > CHASSIS_POWER_LIMITED_INFANTRY)
	{
		Beep_On();	
		return (float)CHASSIS_POWER_LIMITED_INFANTRY / (float)game_info.power_heat_data.chassisPower;
	}		
#endif
		
		//Ó¢ÐÛ
#if defined (RM_HERO)
	if( game_info.power_heat_data.chassisPower > CHASSIS_POWER_LIMITED_HERO)
	{
		return (float)CHASSIS_POWER_LIMITED_HERO / (float)game_info.power_heat_data.chassisPower;
	}
#endif
		

	
	return 1.0f;
}

