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

//TIM1~8可产生中断

//TIM6示例，参数为中断时间间隔（ms）
void TIM6_Init(u32 time)
{
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic; 
	
	//时钟使能
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE);
	

	timer.TIM_Prescaler 		= (u16)(sqrt(90000.0f * time) - 1);			//时钟/1000，使time单位为ms
	timer.TIM_CounterMode 		= TIM_CounterMode_Up;
	timer.TIM_Period			= (u16)(sqrt(90000.0f * time) - 1);
	timer.TIM_ClockDivision 	= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM6, &timer);
	
	TIM_ITConfig( TIM6, TIM_IT_Update, ENABLE); //允许定时器6更新中断 
	
	nvic.NVIC_IRQChannel 					= TIM6_DAC_IRQn;		//TIM6这里与其他不同，中断名称随这个变化
	nvic.NVIC_IRQChannelPreemptionPriority  = 0x01;
	nvic.NVIC_IRQChannelSubPriority 		= 0x03;
	nvic.NVIC_IRQChannelCmd					= ENABLE;	

	NVIC_Init(&nvic);								//初始化 NVIC 
	
	TIM_Cmd( TIM6, ENABLE);
//	printf(" sqrt(90000000.0f / time) - 1=%f\r\n", sqrt(90000000.0f / time) - 1);
}


void TIM6_DAC_IRQHandler(void) 		//TIM6的中断名称也不同
{  
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断  
	{  
		

		IWDG_Feed();			//独立看门狗喂狗
		
		Shoot_Heat_Calc();
		
		++ramp_cnt;				//为斜坡函数提供支持	
		++tim6_cnt;				//为其它时间间隔提供支持
		++rc_cnt;				//为失控保护提供支持
		
		//失控保护
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
		
		//控制异常保护
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

		
		//5ms间隔
		if( tim6_cnt % 5 == 0)	
		{
			Chassis_Ctrl();				//底盘控制
			Gimbal_Ctrl();				//云台控制
			Friction_Ctrl();			//摩擦轮控制
			
			
		}
		
		//do something.... 
	}  
	
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位 
}

