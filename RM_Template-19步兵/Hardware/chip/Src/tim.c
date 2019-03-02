#include "tim.h"
#include "usart.h"
#include "math.h"
#include "report.h"
#include "DR16.h"
#include "chassis.h"
#include "watch_dog.h"
#include "gimbal.h"
#include "imu.h"
#include "ammunition_feed_ctrl.h"
#include "ammunition_feed.h"
#include "chassis_ctrl.h"
#include "gimbal_ctrl.h"
#include "ramp.h"
#include "friction_ctrl.h"
#include "shooter_heat.h"
#include "system_reset.h"
#include "mpu6050_driver.h"
#include "key.h"


extern volatile u32 ramp_cnt;
extern volatile u32 rc_cnt;
extern volatile int shoot_num;
extern volatile uint32_t chassis_offline_cnt[4];



static u32 tim6_cnt = 0;

volatile float mygetqval[9];			//���ڴ�Ŵ�����ת�����������
volatile uint32_t judge_data_cnt = 0;	//�����жϲ���ϵͳ�Ƿ�����

volatile uint32_t beep_cnt;
volatile uint32_t led_cnt;


//TIM3 Beep
//TIM2,4,5,8 PWM

//TIM1~8�ɲ����ж�

//TIM6ʾ��������Ϊ�ж�ʱ������ms��
void TIM6_Init(u32 time)
{
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic; 
	
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE);
	

	timer.TIM_Prescaler 		= (u16)(sqrt(90000.0f * time) - 1);			//ʱ��/1000��ʹtime��λΪms
	timer.TIM_CounterMode 		= TIM_CounterMode_Up;
	timer.TIM_Period			= (u16)(sqrt(90000.0f * time) - 1);
	timer.TIM_ClockDivision 	= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM6, &timer);
	
	TIM_ITConfig( TIM6, TIM_IT_Update, ENABLE); //����ʱ��6�����ж� 
	
	nvic.NVIC_IRQChannel 					= TIM6_DAC_IRQn;		//TIM6������������ͬ���ж�����������仯
	nvic.NVIC_IRQChannelPreemptionPriority  = 0x01;
	nvic.NVIC_IRQChannelSubPriority 		= 0x03;
	nvic.NVIC_IRQChannelCmd					= ENABLE;	

	NVIC_Init(&nvic);								//��ʼ�� NVIC 
	
	TIM_Cmd( TIM6, ENABLE);
}


void TIM7_Init(u32 time)
{
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic; 
	
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE);
	

	timer.TIM_Prescaler 		= (u16)(sqrt(90000.0f * time) - 1);			//ʱ��/1000��ʹtime��λΪms
	timer.TIM_CounterMode 		= TIM_CounterMode_Up;
	timer.TIM_Period			= (u16)(sqrt(90000.0f * time) - 1);
	timer.TIM_ClockDivision 	= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM7, &timer);
	
	TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE); //����ʱ��7�����ж� 
	
	nvic.NVIC_IRQChannel 					= TIM7_IRQn;		
	nvic.NVIC_IRQChannelPreemptionPriority  = 0x01;
	nvic.NVIC_IRQChannelSubPriority 		= 0x02;
	nvic.NVIC_IRQChannelCmd					= ENABLE;	

	NVIC_Init(&nvic);								//��ʼ�� NVIC 
	
	TIM_Cmd( TIM7, ENABLE);
}




//���ڳ����߼�
void TIM6_DAC_IRQHandler(void) 		//TIM6���ж�����Ҳ��ͬ
{  
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //����ж�  
	{  
		IWDG_Feed();			//�������Ź�ι��
		
		++ramp_cnt;				//Ϊб�º����ṩ֧��	
		++tim6_cnt;				//Ϊ����ʱ�����ṩ֧��
		++rc_cnt;				//Ϊʧ�ر����ṩ֧��
		
		++judge_data_cnt;		//�жϲ���ϵͳ�Ƿ�����(�ж�����)
		
		//���̵�����߼��(��������)
		++chassis_offline_cnt[0];	
		++chassis_offline_cnt[1];
		++chassis_offline_cnt[2];
		++chassis_offline_cnt[3];
		
		
		if(judge_data_cnt<1000) LED_Ctrl(LED_COLOR_RED,LED_SWITCH_ON);
		else LED_Ctrl(LED_COLOR_RED,LED_SWITCH_OFF);
		
		
		//ʧ�ر���
		if( rc_cnt > 15)
		{
			RC_CtrlData.key.v = 0;
			RC_CtrlData.mouse.press_l = 0;
			RC_CtrlData.mouse.press_r = 0;
			RC_CtrlData.mouse.x = 0;
			RC_CtrlData.mouse.y = 0;
			RC_CtrlData.mouse.z = 0;
			RC_CtrlData.rc.ch0 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch1 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch2 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch3 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.s1 = 0;
			RC_CtrlData.rc.s2 = 0;
		}
		
		//�����쳣����
		if( RC_CtrlData.rc.ch0 < RC_CH_VALUE_MIN || RC_CtrlData.rc.ch0 > RC_CH_VALUE_MAX ||
			RC_CtrlData.rc.ch1 < RC_CH_VALUE_MIN || RC_CtrlData.rc.ch1 > RC_CH_VALUE_MAX ||
			RC_CtrlData.rc.ch2 < RC_CH_VALUE_MIN || RC_CtrlData.rc.ch2 > RC_CH_VALUE_MAX ||
			RC_CtrlData.rc.ch3 < RC_CH_VALUE_MIN || RC_CtrlData.rc.ch3 > RC_CH_VALUE_MAX )
		{
			//ң��������ΪĬ��ֵ
			RC_CtrlData.key.v = 0;
			RC_CtrlData.mouse.press_l = 0;
			RC_CtrlData.mouse.press_r = 0;
			RC_CtrlData.mouse.x = 0;
			RC_CtrlData.mouse.y = 0;
			RC_CtrlData.mouse.z = 0;
			RC_CtrlData.rc.ch0 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch1 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch2 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.ch3 = RC_CH_VALUE_OFFSET;
			RC_CtrlData.rc.s1 = 0;
			RC_CtrlData.rc.s2 = 0;
			
			//��������
			System_Reset();
		}
		
		
		
		//5ms���
		if( tim6_cnt % 5 == 0)	
		{
			Ammunition_Ctrl();			//��������
			Chassis_Ctrl();				//���̿���
			Gimbal_Ctrl();				//��̨����
			Friction_Ctrl();			//Ħ���ֿ���
			
			//��ȡimu����
			IMU_getValues(mygetqval);
			IMU_Get((int*)&Yaw,(int*)&Pitch);
			
//			Report_Add(Yaw);
//			Report();
		}
		//do something.... 
	}  
	
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //����жϱ�־λ 
}

//���ף�ѧè��
char music_xuemaojiao[] = "1*2*3*51*3*3*2*1*2*5*5*5*5*-1*71*1*1*1*1*-71*71*765-5566666-535352*1*-53*3*3*4*5*1*2*3*2*----00555-556535-1113-333113---32111-1011115/5/3-1-5/4-444112----";//"1_1_5_5_6_6_54_4_3_3_2_2_15_5_4_4_3_3_25_5_4_4_3_3_21_1_5_5_6_6_54_4_3_3_2_2_1_";
//���ף�������
char music_lanhuacao[] = "6/3333-21-2_17/6/--66666-535543--36653-21217/6/-3/-3/117/6/-32-1_7/5/6/--";

char *music = music_xuemaojiao;

//����ϵͳ����
void TIM7_IRQHandler(void) 		
{  
	static uint32_t tim7_cnt = 0;
	static uint32_t reset_cnt = 0;
	
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�  
	{  
		++beep_cnt;
		++led_cnt;
		
		Beep_Music_Server();
		Beep_Server();

		
		//������
		if(RC_CtrlData.rc.s2 == 1 && last_RC_CtrlData.rc.s2 == 3)
			Beep_Tone(music);
		else Beep_Off();
		
		if( Key_State() == KEY_PRESSED)
		{
			LED_Ctrl( LED_COLOR_RED, LED_SWITCH_ON);
			
			//������������
			++reset_cnt;
		}
		else
		{
			reset_cnt = 0;
			LED_Ctrl( LED_COLOR_RED, LED_SWITCH_OFF);
		}
		
		//ϵͳ����
		if(reset_cnt > 3000)
			System_Reset();
		
		
		//5ms���
		if( tim7_cnt % 5 == 0)	
		{
			
		}
		
		//10ms���
		if( tim7_cnt % 10 == 0)	
		{
			Key_Scan();		//ɨ�谴��
		}
		
	}  
	
	
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ 
}





