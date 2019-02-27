#ifndef AMMUNITION_FEED_H_INCLUDED
#define AMMUNITION_FEED_H_INCLUDED

#include "sys.h"
#include "pid.h"

#define CLOCKWISE		1		//顺时针
#define ANTI_CLOCKWISE	2		//逆时针

#define AMMUNITION_JAM 1

#define AMMUNITION_ANGLE_END 1	//转完标志

typedef struct
{
	int32_t encoder;				//每帧原始数据
	float angle;					//角度（积分）
	int32_t rpm;					//转速
	
	float angle_set;
	
	u8 dir;							//方向
	u8 jam_flag;					//卡弹标志
	u8 end_flag;					//转到角度标志
} Encoder_data;



void Ammunition_Feed_Init(void);						//拨弹初始化
void Encoder_Ctrl( int32_t speed, float angle);
void Get_Encoder_Data(void);
void AmmunitionMotorSpeedSet( int16_t MotorSpeed);//拨弹电机转速/转向设置



extern Encoder_data encoder_data;
extern pid_t ammunition_spd_pid;

extern int16_t Encoder;

#endif // AMMUNITION_FEED_H
