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

volatile float mygetqval[9];			//用于存放传感器转换结果的数组
volatile uint32_t judge_data_cnt = 0;	//用于判断裁判系统是否连接

volatile uint32_t beep_cnt;
volatile uint32_t led_cnt;


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
}


void TIM7_Init(u32 time)
{
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic; 
	
	//时钟使能
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE);
	

	timer.TIM_Prescaler 		= (u16)(sqrt(90000.0f * time) - 1);			//时钟/1000，使time单位为ms
	timer.TIM_CounterMode 		= TIM_CounterMode_Up;
	timer.TIM_Period			= (u16)(sqrt(90000.0f * time) - 1);
	timer.TIM_ClockDivision 	= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM7, &timer);
	
	TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE); //允许定时器7更新中断 
	
	nvic.NVIC_IRQChannel 					= TIM7_IRQn;		
	nvic.NVIC_IRQChannelPreemptionPriority  = 0x01;
	nvic.NVIC_IRQChannelSubPriority 		= 0x02;
	nvic.NVIC_IRQChannelCmd					= ENABLE;	

	NVIC_Init(&nvic);								//初始化 NVIC 
	
	TIM_Cmd( TIM7, ENABLE);
}




//用于程序逻辑
void TIM6_DAC_IRQHandler(void) 		//TIM6的中断名称也不同
{  
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断  
	{  
		IWDG_Feed();			//独立看门狗喂狗
		
		++ramp_cnt;				//为斜坡函数提供支持	
		++tim6_cnt;				//为其它时间间隔提供支持
		++rc_cnt;				//为失控保护提供支持
		
		++judge_data_cnt;		//判断裁判系统是否连接(中断清零)
		
		//底盘电机掉线检测(接收清零)
		++chassis_offline_cnt[0];	
		++chassis_offline_cnt[1];
		++chassis_offline_cnt[2];
		++chassis_offline_cnt[3];
		
		
		if(judge_data_cnt<1000) LED_Ctrl(LED_COLOR_RED,LED_SWITCH_ON);
		else LED_Ctrl(LED_COLOR_RED,LED_SWITCH_OFF);
		
		
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
			//遥控数据设为默认值
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
			
			//程序重启
			System_Reset();
		}
		
		
		
		//5ms间隔
		if( tim6_cnt % 5 == 0)	
		{
			Ammunition_Ctrl();			//拨弹控制
			Chassis_Ctrl();				//底盘控制
			Gimbal_Ctrl();				//云台控制
			Friction_Ctrl();			//摩擦轮控制
			
			//获取imu数据
			IMU_getValues(mygetqval);
			IMU_Get((int*)&Yaw,(int*)&Pitch);
			
//			Report_Add(Yaw);
//			Report();
		}
		//do something.... 
	}  
	
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位 
}

//曲谱，学猫叫
char music_xuemaojiao[] = "1*2*3*51*3*3*2*1*2*5*5*5*5*-1*71*1*1*1*1*-71*71*765-5566666-535352*1*-53*3*3*4*5*1*2*3*2*----00555-556535-1113-333113---32111-1011115/5/3-1-5/4-444112----";//"1_1_5_5_6_6_54_4_3_3_2_2_15_5_4_4_3_3_25_5_4_4_3_3_21_1_5_5_6_6_54_4_3_3_2_2_1_";
//曲谱，兰花草
char music_lanhuacao[] = "6/3333-21-2_17/6/--66666-535543--36653-21217/6/-3/-3/117/6/-32-1_7/5/6/--";

char *music = music_xuemaojiao;

//用于系统监视
void TIM7_IRQHandler(void) 		
{  
	static uint32_t tim7_cnt = 0;
	static uint32_t reset_cnt = 0;
	
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断  
	{  
		++beep_cnt;
		++led_cnt;
		
		Beep_Music_Server();
		Beep_Server();

		
		//放音乐
		if(RC_CtrlData.rc.s2 == 1 && last_RC_CtrlData.rc.s2 == 3)
			Beep_Tone(music);
		else Beep_Off();
		
		if( Key_State() == KEY_PRESSED)
		{
			LED_Ctrl( LED_COLOR_RED, LED_SWITCH_ON);
			
			//按键长按重启
			++reset_cnt;
		}
		else
		{
			reset_cnt = 0;
			LED_Ctrl( LED_COLOR_RED, LED_SWITCH_OFF);
		}
		
		//系统重启
		if(reset_cnt > 3000)
			System_Reset();
		
		
		//5ms间隔
		if( tim7_cnt % 5 == 0)	
		{
			
		}
		
		//10ms间隔
		if( tim7_cnt % 10 == 0)	
		{
			Key_Scan();		//扫描按键
		}
		
	}  
	
	
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位 
}





