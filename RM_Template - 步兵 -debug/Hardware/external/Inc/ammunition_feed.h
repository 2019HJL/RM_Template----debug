#ifndef AMMUNITION_FEED_H_INCLUDED
#define AMMUNITION_FEED_H_INCLUDED

#include "sys.h"
#include "pid.h"

#define CLOCKWISE		1		//˳ʱ��
#define ANTI_CLOCKWISE	2		//��ʱ��

#define AMMUNITION_JAM 1

#define AMMUNITION_ANGLE_END 1	//ת���־

typedef struct
{
	int32_t encoder;				//ÿ֡ԭʼ����
	float angle;					//�Ƕȣ����֣�
	int32_t rpm;					//ת��
	
	float angle_set;
	
	u8 dir;							//����
	u8 jam_flag;					//������־
	u8 end_flag;					//ת���Ƕȱ�־
} Encoder_data;



void Ammunition_Feed_Init(void);						//������ʼ��
void Encoder_Ctrl( int32_t speed, float angle);
void Get_Encoder_Data(void);
void AmmunitionMotorSpeedSet( int16_t MotorSpeed);//�������ת��/ת������



extern Encoder_data encoder_data;
extern pid_t ammunition_spd_pid;

extern int16_t Encoder;

#endif // AMMUNITION_FEED_H
