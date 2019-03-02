#include "beep.h"
#include "pwm.h"

volatile int Beep_State;
static Beep beep;
static int Server_State = RESET;

int freq_C[7] = {523,587,659,698,784,880,988};		//C调中音频率

Beep_Music beep_music;								//曲谱

//蜂鸣器初始化
void Beep_Init(void)
{
	PWM_Beep_Init();
	Beep_State = BEEP_SWITCH_OFF;
	
	Beep_Set(0,0,0);
}

//蜂鸣器开
void Beep_On(void)
{
	if( Beep_State == BEEP_SWITCH_ON)
		return ;
	TIM_Cmd( TIM3, ENABLE);				//使能TIM3
	TIM_SetCompare1( TIM3, 64);			//50%PWM
	Beep_State = BEEP_SWITCH_ON;
}

//蜂鸣器关
void Beep_Off(void)
{	
	if( Beep_State == BEEP_SWITCH_OFF)
		return ;
	TIM_SetCompare1( TIM3, 0);			//0%PWM
	TIM_Cmd( TIM3, DISABLE);				//失能TIM3
	Beep_State = BEEP_SWITCH_OFF;
}


//蜂鸣器播放状态控制
void Beep_Ctrl(int beep_state)
{
	if( beep_state == BEEP_SWITCH_ON)
	{
		TIM_Cmd( TIM3, ENABLE);				//使能TIM3
		TIM_SetCompare1( TIM3, 64);			//50%PWM
		Beep_State = BEEP_SWITCH_ON;	
	}
	else if(Beep_State == BEEP_SWITCH_OFF)
	{
		TIM_SetCompare1( TIM3, 0);			//0%PWM
		TIM_Cmd( TIM3, DISABLE);				//失能TIM3
		Beep_State = BEEP_SWITCH_OFF;	
	}
}

//蜂鸣器播放状态反转
void Beep_Switch(void)
{
	if( Beep_State == BEEP_SWITCH_ON)
	{
		TIM_SetCompare1( TIM3, 0);			//0%PWM
		TIM_Cmd( TIM3, DISABLE);				//失能TIM3
		Beep_State = BEEP_SWITCH_OFF;
	}
	else if(Beep_State == BEEP_SWITCH_OFF)
	{
		TIM_Cmd( TIM3, ENABLE);				//使能TIM3
		TIM_SetCompare1( TIM3, 64);			//50%PWM
		Beep_State = BEEP_SWITCH_ON;		
	}
}

//蜂鸣器间断播放设置
void Beep_Set( uint32_t ms, uint32_t times, uint32_t interval)
{
	beep.ms = ms;
	beep.times = times;
	beep.interval = interval;
	
	beep.beep_ms = ms;
	beep.beep_times = times;
	beep.beep_interval = interval;
	
}

void Beep_Server()
{
	//播放时间（ms）
	if( beep.ms > 0)
	{
		--beep.ms;
		Beep_On();
		Beep_State = BEEP_SWITCH_ON;
	}	
	//播放间隔
	else 
	{
		Beep_Off();
		Beep_State = BEEP_SWITCH_OFF;
		if( beep.interval > 0)
			--beep.interval;
		//播放次数
		else if( beep.times > 1)
		{
			--beep.times;
			beep.ms = beep.beep_ms;
			beep.interval = beep.beep_interval;
		}
	}
	//播放完毕，设置播放状态
	if(beep.ms == 0 && beep.interval == 0 && beep.times <= 1)
		Server_State = RESET;
	else Server_State = SET;
}


//设置要播放的曲谱
void Beep_Tone(char *music)
{
	beep_music.music 				= music;
	beep_music.progress 			= 0;
	beep_music.current_tone_freq	= 0;
	beep_music.current_time 		= 0;
}



//解析曲谱
void Music_Analysis(Beep_Music *music)
{
	music->current_time = 1;
	
	//播放完毕
	if( music->music[music->progress] == '\0')
		return ;
	
	//设置音调0的频率
	if( music->music[music->progress] == '0') music->current_tone_freq = 0;
	
	//获取音调1~7的频率
	if( music->music[music->progress] >= '1' && music->music[music->progress] <= '7')
		music->current_tone_freq = freq_C[ music->music[ music->progress] - '1'];
	
	//解析曲谱
	for( music->progress+=1; music->music[music->progress] != '\0' && !(music->music[music->progress] >= '1' && music->music[music->progress] <= '7'); ++music->progress)
	{
		if( music->music[music->progress] == '*') music->current_tone_freq 	*=2;			//频率*2，变高一阶
		if( music->music[music->progress] == '/') music->current_tone_freq 	/=2;			//频率/2，变低一阶
		
		if( music->music[music->progress] == '-') music->current_time 		+=1;			//时间+1s
		if( music->music[music->progress] == '_') music->current_time 		/=2.0f;		//时间/2
	}
}



//蜂鸣器音乐服务函数
void Beep_Music_Server(void)
{	
	if(Server_State == RESET && /*sizeof(beep_music.music) != beep_music.progress)*/ beep_music.music[beep_music.progress] != '\0')
	{
		Music_Analysis(&beep_music);						//解析曲谱
		Beep_Freq_Set(beep_music.current_tone_freq);		//设置频率
		
		if(beep_music.current_time == 0)
			return ;
		Beep_Set(beep_music.current_time * 500, 1, 20);		//播放音调
	}
}

