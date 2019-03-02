#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "sys.h"

void IMU_Get(int *Yaw, int * Pitch);				//imu数据获取
float one_filter(float angle_m,float gyro_m);		//一阶互补滤波（不能同时用于多组数据）
float kalman_filter(float angle_m,float gyro_m);	//卡尔曼滤波（不能同时用于多组数据）


#endif
