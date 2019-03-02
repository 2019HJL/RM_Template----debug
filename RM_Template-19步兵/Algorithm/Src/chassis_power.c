#include "chassis_power.h"
#include "judge_data.h"
#include "chassis.h"
#include "usart.h"

volatile int pid_zero_flag = RESET;

//------------------------------------------------------
//功率控制效果一般，对遥控手感影响较为明显，应当重写
//------------------------------------------------------





//void Chassis_Power_Limit(float *acceleration, float *speed, float *current)
//{
//	float current_sum;
//	int i;
//	float accel_limit = 0.0f;
//	static uint32_t exceed_cnt;		//超限时间
//	
//	for( i=0; i < 4; ++i)
//	{
//		current_sum += ABS(current[i]);
//	}
//	
//	//总电流超限检测
//	if( current_sum * 24.0f < CHASSIS_POWER_LIMITED)
//	{Beep_Off();
//		exceed_cnt = 0;
//		pid_zero_flag = RESET;
//		return ;
//	}
//	else
//	{Beep_On();
//		++exceed_cnt;
//		
//		//超限时间过长，断开输出
//		if( exceed_cnt > 1000)
//			for( i = 0; i < 4; ++i)
//				pid_zero_flag = SET;
//		
//		for( i = 0; i < 4; ++i)
//		{
//			//限制速度
//			speed[i] *= CHASSIS_POWER_LIMITED / current_sum / 24.0f;
//			//限制加速度                       
//			if( acceleration[i] * speed[i] > 0 && acceleration[i] > ACCELERATION_MAX && accel_limit < ACCELERATION_MAX / acceleration[i])		//速度加速度同向时限制最大值
//				accel_limit =  ACCELERATION_MAX / acceleration[i];
//		}
//		
//		for( i = 0; i < 4; ++i)
//		{
//			acceleration[i] *= accel_limit;
//		}
//	}
//}
void Chassis_Power_Limit( float *speed)
{
	float current_sum;
	int i;
	float accel_limit = 1.0f;
	static uint32_t exceed_cnt;		//超限时间
	
//	for( i=0; i < 4; ++i)
//	{
//		current_sum += ABS(current[i]);
//	}

	//功率超限检测
	if( game_info.power_heat_data.chassisPower < CHASSIS_POWER_LIMITED)
	{
//		Beep_Off();
		exceed_cnt = 0;
		pid_zero_flag = RESET;
		return ;
	}
	else
	{
//		Beep_On();
		++exceed_cnt;
		
		//超限时间过长（1s），断开底盘输出
		if( exceed_cnt > 200)
			for( i = 0; i < 4; ++i)
				pid_zero_flag = SET;
		//限制速度
		for( i = 0; i < 4; ++i)
		{
			if(CHASSIS_POWER_LIMITED / ((float)game_info.power_heat_data.chassisPower + 10.0f) <= 1.0f)
			{
				if(game_info.power_heat_data.chassisPower - CHASSIS_POWER_LIMITED <10.0f)
					speed[i] *= (float)CHASSIS_POWER_LIMITED / ((float)game_info.power_heat_data.chassisPower + 35.0f);		
				else if(game_info.power_heat_data.chassisPower - CHASSIS_POWER_LIMITED <20.0f)
					speed[i] *= (float)CHASSIS_POWER_LIMITED / ((float)game_info.power_heat_data.chassisPower + 60.0f);				
				else if(game_info.power_heat_data.chassisPower - CHASSIS_POWER_LIMITED <30.0f)
					speed[i] *= (float)CHASSIS_POWER_LIMITED / ((float)game_info.power_heat_data.chassisPower + 80.0f);
			}

		}


	}	

}
