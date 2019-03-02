#include "beep.h"
#include "pwm.h"

volatile int Beep_State;
static Beep beep;
static int Server_State = RESET;

int freq_C[7] = {523,587,659,698,784,880,988};		//C������Ƶ��

Beep_Music beep_music;								//����

//��������ʼ��
void Beep_Init(void)
{
	PWM_Beep_Init();
	Beep_State = BEEP_SWITCH_OFF;
	
	Beep_Set(0,0,0);
}

//��������
void Beep_On(void)
{
	if( Beep_State == BEEP_SWITCH_ON)
		return ;
	TIM_Cmd( TIM3, ENABLE);				//ʹ��TIM3
	TIM_SetCompare1( TIM3, 64);			//50%PWM
	Beep_State = BEEP_SWITCH_ON;
}

//��������
void Beep_Off(void)
{	
	if( Beep_State == BEEP_SWITCH_OFF)
		return ;
	TIM_SetCompare1( TIM3, 0);			//0%PWM
	TIM_Cmd( TIM3, DISABLE);				//ʧ��TIM3
	Beep_State = BEEP_SWITCH_OFF;
}


//����������״̬����
void Beep_Ctrl(int beep_state)
{
	if( beep_state == BEEP_SWITCH_ON)
	{
		TIM_Cmd( TIM3, ENABLE);				//ʹ��TIM3
		TIM_SetCompare1( TIM3, 64);			//50%PWM
		Beep_State = BEEP_SWITCH_ON;	
	}
	else if(Beep_State == BEEP_SWITCH_OFF)
	{
		TIM_SetCompare1( TIM3, 0);			//0%PWM
		TIM_Cmd( TIM3, DISABLE);				//ʧ��TIM3
		Beep_State = BEEP_SWITCH_OFF;	
	}
}

//����������״̬��ת
void Beep_Switch(void)
{
	if( Beep_State == BEEP_SWITCH_ON)
	{
		TIM_SetCompare1( TIM3, 0);			//0%PWM
		TIM_Cmd( TIM3, DISABLE);				//ʧ��TIM3
		Beep_State = BEEP_SWITCH_OFF;
	}
	else if(Beep_State == BEEP_SWITCH_OFF)
	{
		TIM_Cmd( TIM3, ENABLE);				//ʹ��TIM3
		TIM_SetCompare1( TIM3, 64);			//50%PWM
		Beep_State = BEEP_SWITCH_ON;		
	}
}

//��������ϲ�������
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
	//����ʱ�䣨ms��
	if( beep.ms > 0)
	{
		--beep.ms;
		Beep_On();
		Beep_State = BEEP_SWITCH_ON;
	}	
	//���ż��
	else 
	{
		Beep_Off();
		Beep_State = BEEP_SWITCH_OFF;
		if( beep.interval > 0)
			--beep.interval;
		//���Ŵ���
		else if( beep.times > 1)
		{
			--beep.times;
			beep.ms = beep.beep_ms;
			beep.interval = beep.beep_interval;
		}
	}
	//������ϣ����ò���״̬
	if(beep.ms == 0 && beep.interval == 0 && beep.times <= 1)
		Server_State = RESET;
	else Server_State = SET;
}


//����Ҫ���ŵ�����
void Beep_Tone(char *music)
{
	beep_music.music 				= music;
	beep_music.progress 			= 0;
	beep_music.current_tone_freq	= 0;
	beep_music.current_time 		= 0;
}



//��������
void Music_Analysis(Beep_Music *music)
{
	music->current_time = 1;
	
	//�������
	if( music->music[music->progress] == '\0')
		return ;
	
	//��������0��Ƶ��
	if( music->music[music->progress] == '0') music->current_tone_freq = 0;
	
	//��ȡ����1~7��Ƶ��
	if( music->music[music->progress] >= '1' && music->music[music->progress] <= '7')
		music->current_tone_freq = freq_C[ music->music[ music->progress] - '1'];
	
	//��������
	for( music->progress+=1; music->music[music->progress] != '\0' && !(music->music[music->progress] >= '1' && music->music[music->progress] <= '7'); ++music->progress)
	{
		if( music->music[music->progress] == '*') music->current_tone_freq 	*=2;			//Ƶ��*2�����һ��
		if( music->music[music->progress] == '/') music->current_tone_freq 	/=2;			//Ƶ��/2�����һ��
		
		if( music->music[music->progress] == '-') music->current_time 		+=1;			//ʱ��+1s
		if( music->music[music->progress] == '_') music->current_time 		/=2.0f;		//ʱ��/2
	}
}



//���������ַ�����
void Beep_Music_Server(void)
{	
	if(Server_State == RESET && /*sizeof(beep_music.music) != beep_music.progress)*/ beep_music.music[beep_music.progress] != '\0')
	{
		Music_Analysis(&beep_music);						//��������
		Beep_Freq_Set(beep_music.current_tone_freq);		//����Ƶ��
		
		if(beep_music.current_time == 0)
			return ;
		Beep_Set(beep_music.current_time * 500, 1, 20);		//��������
	}
}

