#ifndef BEEP_H_INCLUDED
#define BEEP_H_INCLUDED

#include "sys.h"


#define BEEP_SWITCH_ON 		1				//蜂鸣器开
#define BEEP_SWITCH_OFF		2				//蜂鸣器关



typedef struct 
{
	//用于设定
	uint32_t beep_ms;
	uint32_t beep_times;
	uint32_t beep_interval;
	//用于实现
	uint32_t ms;
	uint32_t times;
	uint32_t interval;
} Beep;


//蜂鸣器音乐
typedef struct
{
	char *music;						//谱子
	int progress;						//播放进度（数组下标）
	int current_tone_freq;				//当前音调
	float current_time;					//当前音调时长
} Beep_Music;


void Beep_Init(void);					//Beep初始化
void Beep_On(void);						//开启Beep
void Beep_Off(void);					//关闭Beep
void Beep_Ctrl(int beep_state);			//控制Beep开关
void Beep_Switch(void);					//反转Beep状态

void Beep_Set( uint32_t ms, uint32_t times, uint32_t interval);		//设置Beep时间，次数及间隔
void Beep_Server(void);												//Beep_Set服务函数

void Beep_Tone(char *music);			//蜂鸣器曲谱设置
void Beep_Music_Server(void);			//蜂鸣器音乐服务


#endif // BEEP_H_INCLUDED
