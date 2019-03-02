#ifndef CHASSIS_POWER_H_INCLUDED
#define CHASSIS_POWER_H_INCLUDED


#include "sys.h"

#define CHASSIS_POWER_LIMITED		 		80.0f

#define ACCELERATION_MAX 					1.67f			//最大加速度
#define ACCELERATION_SUM_MAX 					6.2f			//最大加速度


void Chassis_Power_Limit( float *v);


#endif // CHASSIS_POWER_H_INCLUDED
