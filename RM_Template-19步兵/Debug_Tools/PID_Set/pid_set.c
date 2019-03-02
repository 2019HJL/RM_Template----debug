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
	
	for( t = 0; t < 20 && t < len; ++t)				//��Ҫ����ʲôpid
	{
		if( ch[t] == ' ')
			break;
		pid_set[t] = ch[t];
	}
	pid_set[t] = '\0';							//���ַ�����β
	//printf("%s\r\n",pid_set);
	
	kp = atof( &ch[t]);					//��p
	for( ++t; t < len && ch[t] != ' '; ++t);			//Ѱ����һ�ո�
	ki = atof( &ch[t]);					//��i
	for( ++t; t < len && ch[t] != ' '; ++t);			//Ѱ����һ�ո�
	kd = atof( &ch[t]);					//��d
	//printf("%f,%f,%f\r\n",kp,ki,kd);
	
	//�����ٶȻ�pid
	if( strcmp( pid_set, "PID_Chassis_Spd") == 0) 				
		for( t = 0; t < 4; ++t)
			pid_reset( &chassis_spd_pid[t], kp, ki, kd);
	
	//����λ�û�pid
	else if( strcmp( pid_set, "PID_Chassis_Pos") == 0) 				
		for( t = 0; t < 4; ++t)
			pid_reset( &chassis_pos_pid[t], kp, ki, kd);	
	
	//��̨pitch���ٶȻ�pid
	else if( strcmp( pid_set, "PID_Gimbal_Pitch_Spd") == 0) 
		pid_reset( &gimbal_pitch_spd_pid, kp, ki, kd);
	
	//��̨pitch��λ�û�pid
	else if( strcmp( pid_set, "PID_Gimbal_Pitch_Pos") == 0) 
		pid_reset( &gimbal_pitch_pos_pid, kp, ki, kd);		
	
	//��̨yaw���ٶȻ�pid
	else if( strcmp( pid_set, "PID_Gimbal_Yaw_Spd") == 0) 
		pid_reset( &gimbal_yaw_spd_pid, kp, ki, kd);	
	
	//��̨yaw��λ�û�pid
	else if( strcmp( pid_set, "PID_Gimbal_Yaw_Pos") == 0) 
		pid_reset( &gimbal_yaw_pos_pid, kp, ki, kd);			
}



