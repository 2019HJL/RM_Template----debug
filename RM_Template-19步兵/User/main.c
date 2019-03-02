#include "main.h"


extern void Beep_Tone_Set(int tone);


int main(void)
{	float c_sum_abs=0,c_sum = 0;
	RM_Init();										//硬件初始化
	LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_ON);		//绿灯亮
	Friction_Range_Set_Fast();						//摩擦轮电机行程设定
	
	vision_get.pattern = 0;
	vision_get.Pitch = 0;
	vision_get.Yaw = 0;
	
	
	while(1)
	{
		//程序逻辑在tim.c中

//		printf("%.2f,%.2f,%.2f,%.2f\r\n",game_info.power_heat_data.chassisCurrent,game_info.power_heat_data.chassisPower,game_info.power_heat_data.chassisPowerBuffer,game_info.power_heat_data.chassisVolt);
//		for(i = 0; i<4; ++i)
//		{
//			c_sum_abs	+= ABS(chassis_motor_info[i].current);
//			c_sum		+= chassis_motor_info[i].current;
//		}
//		printf("%.2f,%.2f\r\n",c_sum,c_sum_abs);
//		c_sum=0;
//		c_sum_abs=0;
		
		if( Key_State() == KEY_PRESSED)
		{
			Laser_Switch();
		}

		
		delay_ms(5);

	}
	


}




void RM_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	
	delay_ms(100);
	
	RM_GPIO_Init();				//gpio初始化
	LED_Init();					//led初始化
	Beep_Init();				//蜂鸣器初始化
	Laser_Init();				//激光初始化
	
	//uart串口初始化
	USART2_Init(115200);		
	USART3_Init(115200);
	USART6_Init(115200);
	//can1初始化
	CAN1_Init( CAN_SJW_4tq, CAN_BS2_5tq, CAN_BS1_3tq, 5, CAN_Mode_Normal);
	
	RC_Init();					//遥控初始化
	Judge_Init();				//裁判系统初始化
	
	//PWM初始化（摩擦轮）
	PWM_Init( PWM_PA0, 50, 20000, TIM_OCPolarity_High);
	PWM_Init( PWM_PA1, 50, 20000, TIM_OCPolarity_High); 
	
	delay_ms(1000);
	
	DMA_USART2_Init();			//DMA初始化
	MPU6050_Initialize();		//MPU6050(惯导)初始化
	Chassis_Init();				//底盘初始化
	Gimbal_Init();				//云台初始化
	Ammunition_Init();			//摩擦轮初始化


	//看门狗初始化
//	IWDG_Init( 4, 500);
//	WWDG_Init( 0x7f, 0x5f, WWDG_Prescaler_8);			//暂时不能用

	//定时器初始化
	TIM6_Init(1);
	TIM7_Init(1);
}

