#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 

#include "led.h"
#include "beep.h"
#include "DR16.h"

//-------------------------------------------------------
//该文件内放置常用常量，及总体配置
//-------------------------------------------------------



#define RM_INFANTRY
//#define RM_HERO
//#define RM_ENGINEER
//#define RM_AIR
//#define RM_SENTRY

//遥控摇杆映射
#define RC_GIMBAL_YAW				RC_CtrlData.rc.ch2
#define RC_GIMBAL_PITCH				RC_CtrlData.rc.ch3
#define RC_CHASSIS_X				RC_CtrlData.rc.ch1
#define RC_CHASSIS_Y				RC_CtrlData.rc.ch0
//#define RC_CHASSIS_YAW				RC_CtrlData.rc.ch



//遥控键鼠映射
//#define RC_KEY
//#define RC_KEY_SPEED_UP				RC_CtrlData.key.	
#define RC_KEY_SHOOT				RC_CtrlData.mouse.press_l
#define RC_KEY_GIMBAL_YAW			RC_CtrlData.mouse.y
#define RC_KEY_GIMBAL_PITCH			RC_CtrlData.mouse.x
#define RC_KEY_CHASSIS_X			//RC_CtrlData.
#define RC_KEY_CHASSIS_Y			//RC_CtrlData.key.v
//#define RC_KEY_CHASSIS_YAW			RC_CtrlData.



#define ABS(X)	( ( (X) > 0) ? ( X) : ( -(X)))			//绝对值计算
#define PI 3.141592654f									//圆周率
#define TRUE  1
#define FALSE 0

#endif











