#include "tim.h"
#include "LED.h"
#include "usart.h"
#include "math.h"
#include "report.h"
#include "DR16.h"
#include "chassis.h"
#include "watch_dog.h"
#include "gimbal.h"
#include "mpu6500.h"
#include "ammunition_feed.h"
#include "chassis_ctrl.h"
#include "gimbal_ctrl.h"
#include "ramp.h"
#include "friction_ctrl.h"
#include "shooter_heat.h"
#include "system_reset.h"



extern volatile u32 ramp_cnt;
extern volatile u32 rc_cnt;

static u32 tim6_cnt = 0;

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
//	printf(" sqrt(90000000.0f / time) - 1=%f\r\n", sqrt(90000000.0f / time) - 1);
}


void TIM6_DAC_IRQHandler(void) 		//TIM6���ж�����Ҳ��ͬ
{  
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //����ж�  
	{  
		

		IWDG_Feed();			//�������Ź�ι��
		
		Shoot_Heat_Calc();
		
		++ramp_cnt;				//Ϊб�º����ṩ֧��	
		++tim6_cnt;				//Ϊ����ʱ�����ṩ֧��
		++rc_cnt;				//Ϊʧ�ر����ṩ֧��
		
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
			
			System_Reset();
		}
		
		
		Get_Encoder_Data();

		
		//5ms���
		if( tim6_cnt % 5 == 0)	
		{
			Chassis_Ctrl();				//���̿���
			Gimbal_Ctrl();				//��̨����
			Friction_Ctrl();			//Ħ���ֿ���
			
			
		}
		
		//do something.... 
	}  
	
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //����жϱ�־λ 
}

