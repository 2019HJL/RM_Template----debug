#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "sys.h"

void IMU_Get(int *Yaw, int * Pitch);				//imu���ݻ�ȡ
float one_filter(float angle_m,float gyro_m);		//һ�׻����˲�������ͬʱ���ڶ������ݣ�
float kalman_filter(float angle_m,float gyro_m);	//�������˲�������ͬʱ���ڶ������ݣ�


#endif
