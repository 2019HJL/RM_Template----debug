#ifndef CHASSIS_POWER_H_INCLUDED
#define CHASSIS_POWER_H_INCLUDED


#include "sys.h"

#define CHASSIS_POWER_LIMITED		 		80.0f

#define ACCELERATION_MAX 					1.67f			//�����ٶ�
#define ACCELERATION_SUM_MAX 					6.2f			//�����ٶ�


void Chassis_Power_Limit( float *v);


#endif // CHASSIS_POWER_H_INCLUDED
