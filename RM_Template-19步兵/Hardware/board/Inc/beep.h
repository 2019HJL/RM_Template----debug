#ifndef BEEP_H_INCLUDED
#define BEEP_H_INCLUDED

#include "sys.h"


#define BEEP_SWITCH_ON 		1				//��������
#define BEEP_SWITCH_OFF		2				//��������



typedef struct 
{
	//�����趨
	uint32_t beep_ms;
	uint32_t beep_times;
	uint32_t beep_interval;
	//����ʵ��
	uint32_t ms;
	uint32_t times;
	uint32_t interval;
} Beep;


//����������
typedef struct
{
	char *music;						//����
	int progress;						//���Ž��ȣ������±꣩
	int current_tone_freq;				//��ǰ����
	float current_time;					//��ǰ����ʱ��
} Beep_Music;


void Beep_Init(void);					//Beep��ʼ��
void Beep_On(void);						//����Beep
void Beep_Off(void);					//�ر�Beep
void Beep_Ctrl(int beep_state);			//����Beep����
void Beep_Switch(void);					//��תBeep״̬

void Beep_Set( uint32_t ms, uint32_t times, uint32_t interval);		//����Beepʱ�䣬���������
void Beep_Server(void);												//Beep_Set������

void Beep_Tone(char *music);			//��������������
void Beep_Music_Server(void);			//���������ַ���


#endif // BEEP_H_INCLUDED
