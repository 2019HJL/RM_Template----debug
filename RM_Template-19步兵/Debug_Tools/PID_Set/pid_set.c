#include "pid_set.h"
#include "stdlib.h"
#include "chassis.h"
#include "pid.h"
#include "gimbal.h"
#include "string.h"
#include "usart.h"


void PID_Set(void)// char *ch, u8 len)
{
	u8 t;
	u8 len;
	char ch[60];
	char pid_set[20];
	float kp,ki,kd;
	kp = ki = kd = 0;
	
	if( USART_RX_STA & 0x8000)
	{
		len = USART_RX_STA & 0x3fff;
		for( t = 0; t < len && t < 60; ++t)
			ch[t] = USART_RX_BUF[t];
		USART_RX_STA = 0;
	}
	//printf("%s\r\n",ch);
	
	for( t = 0; t < 20 && t < len; ++t)				//读要设置什么pid
	{
		if( ch[t] == ' ')
			break;
		pid_set[t] = ch[t];
	}
	pid_set[t] = '\0';							//加字符串结尾
	//printf("%s\r\n",pid_set);
	
	kp = atof( &ch[t]);					//读p
	for( ++t; t < len && ch[t] != ' '; ++t);			//寻找下一空格
	ki = atof( &ch[t]);					//读i
	for( ++t; t < len && ch[t] != ' '; ++t);			//寻找下一空格
	kd = atof( &ch[t]);					//读d
	//printf("%f,%f,%f\r\n",kp,ki,kd);
	
	//底盘速度环pid
	if( strcmp( pid_set, "PID_Chassis_Spd") == 0) 				
		for( t = 0; t < 4; ++t)
			pid_reset( &chassis_spd_pid[t], kp, ki, kd);
	
	//底盘位置环pid
	else if( strcmp( pid_set, "PID_Chassis_Pos") == 0) 				
		for( t = 0; t < 4; ++t)
			pid_reset( &chassis_pos_pid[t], kp, ki, kd);	
	
	//云台pitch轴速度环pid
	else if( strcmp( pid_set, "PID_Gimbal_Pitch_Spd") == 0) 
		pid_reset( &gimbal_pitch_spd_pid, kp, ki, kd);
	
	//云台pitch轴位置环pid
	else if( strcmp( pid_set, "PID_Gimbal_Pitch_Pos") == 0) 
		pid_reset( &gimbal_pitch_pos_pid, kp, ki, kd);		
	
	//云台yaw轴速度环pid
	else if( strcmp( pid_set, "PID_Gimbal_Yaw_Spd") == 0) 
		pid_reset( &gimbal_yaw_spd_pid, kp, ki, kd);	
	
	//云台yaw轴位置环pid
	else if( strcmp( pid_set, "PID_Gimbal_Yaw_Pos") == 0) 
		pid_reset( &gimbal_yaw_pos_pid, kp, ki, kd);			
}



