#include "main.h"







int main(void)
{
//	RM_GPIO_Init();
//	LED_Init();
//	delay_init(168);
	RM_Init();
	
	Friction_Range_Set_Fast();
	
	vision_get.pattern = 0;
	vision_get.Pitch = 0;
	vision_get.Yaw = 0;
	
//	PWM_Init( PWM_PD12, 2000, 200, TIM_OCPolarity_High);

	//Friction_Range_Set( PWM_PA0);
//	delay_ms(2000);
//	Friction_Speed_Set( PWM_PA0, 600);
	
//	Beep_On();
//	delay_ms(500);
//	Beep_Off();
//	delay_ms(500);
	
	while(1)
	{
//		LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_ON);
//		delay_ms(2000);
//		LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_OFF);
//		//Beep_On();
//		delay_ms(1000);
		//Beep_Off();
		//Friction_Speed_Set( PWM_PA0, (RC_CtrlData.rc.ch3 - 364)*(1000.0f/1320.0f));
//		printf("%f\r\n", (RC_CtrlData.rc.ch3 - 364)*(1000.0f/1320.0f));
		if( KEY_Scan() == KEY_PRESSED)
			LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_ON);
		else LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_OFF);
		
		
//		if( KEY_Scan() == KEY_PRESSED)
//			Ammunition_Ctrl();
//		else PWM_Set( PWM_PD12, 0);
		
//			PWM_Set( PWM_PD12, 100);

		Ammunition_Ctrl();

//		Chassis_Ctrl();
//		Gimbal_Ctrl();
		
//		if( KEY_Scan() == KEY_PRESSED)
//			Laser_Ctrl( LASER_SWITCH_ON);
//		else Laser_Ctrl( LASER_SWITCH_OFF);
		
		if( RC_CtrlData.rc.s2 == 1 || RC_CtrlData.rc.s2 == 3)
			Laser_Ctrl( LASER_SWITCH_ON);
		else Laser_Ctrl( LASER_SWITCH_OFF);
		

//		
//		if(judge_info_get.CmdID == 0x0001)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("maxHP=%d,reaminHP=%d\r\n", game_info.state.maxHP,game_info.state.remainHP);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}
//		
//		if(judge_info_get.CmdID == 0x0008)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("yaw = %f\r\n", game_info.pos.yaw);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}
		
//			if(judge_info_get.CmdID == 0x0003)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("speed=%f\r\n", game_info.shoot_data.bulletSpeed);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}
		
		
//		
//		if( RC_CtrlData.rc.s1 ==3)
//			PWM_Set( PWM_PA3, 0/*pid_out*/);
//		else if( RC_CtrlData.rc.s1 == 2)
		//	PWM_Set( PWM_PA3, 2000);

		//		Encoder_Ctrl(60,75);
//		delay_ms(1000);
		
		
//		PWM_Set(PWM_PA3, Apwm);	
//		if(Angle>12800)
//		{
//			PWM_Set(PWM_PA3, 0);
//			delay_ms(1000);	
//			Angle=0;
//			
//		}
		
		delay_ms(1);

	}
	


}




void RM_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	
	delay_ms(100);
	RM_GPIO_Init();
	LED_Init();
	Beep_Init();
	Laser_Init();
	USART2_Init(115200);
	USART3_Init(115200);
	USART6_Init(115200);
	CAN1_Init( CAN_SJW_4tq, CAN_BS2_5tq, CAN_BS1_3tq, 5, CAN_Mode_Normal);
	RC_Init();
	Judge_Init();
	PWM_Init( PWM_PA0, 50, 20000, TIM_OCPolarity_High);
	PWM_Init( PWM_PA1, 50, 20000, TIM_OCPolarity_High);
	delay_ms(1000);
	
	DMA_USART2_Init();
	
//	SPI5_Init();			//必须在MPU前初始化
//	MPU6500_Init();
	
//	Ammunition_Feed_Init();
	

	
	Chassis_Init();
	Gimbal_Init();
	Ammunition_Init();
	

//	IWDG_Init( 4, 500);
//	WWDG_Init( 0x7f, 0x5f, WWDG_Prescaler_8);			//暂时不能用

	TIM6_Init( 1);
	
}

