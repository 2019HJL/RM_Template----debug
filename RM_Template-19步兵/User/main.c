#include "main.h"


extern void Beep_Tone_Set(int tone);


int main(void)
{	float c_sum_abs=0,c_sum = 0;
	RM_Init();										//Ӳ����ʼ��
	LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_ON);		//�̵���
	Friction_Range_Set_Fast();						//Ħ���ֵ���г��趨
	
	vision_get.pattern = 0;
	vision_get.Pitch = 0;
	vision_get.Yaw = 0;
	
	
	while(1)
	{
		//�����߼���tim.c��

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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	
	delay_ms(100);
	
	RM_GPIO_Init();				//gpio��ʼ��
	LED_Init();					//led��ʼ��
	Beep_Init();				//��������ʼ��
	Laser_Init();				//�����ʼ��
	
	//uart���ڳ�ʼ��
	USART2_Init(115200);		
	USART3_Init(115200);
	USART6_Init(115200);
	//can1��ʼ��
	CAN1_Init( CAN_SJW_4tq, CAN_BS2_5tq, CAN_BS1_3tq, 5, CAN_Mode_Normal);
	
	RC_Init();					//ң�س�ʼ��
	Judge_Init();				//����ϵͳ��ʼ��
	
	//PWM��ʼ����Ħ���֣�
	PWM_Init( PWM_PA0, 50, 20000, TIM_OCPolarity_High);
	PWM_Init( PWM_PA1, 50, 20000, TIM_OCPolarity_High); 
	
	delay_ms(1000);
	
	DMA_USART2_Init();			//DMA��ʼ��
	MPU6050_Initialize();		//MPU6050(�ߵ�)��ʼ��
	Chassis_Init();				//���̳�ʼ��
	Gimbal_Init();				//��̨��ʼ��
	Ammunition_Init();			//Ħ���ֳ�ʼ��


	//���Ź���ʼ��
//	IWDG_Init( 4, 500);
//	WWDG_Init( 0x7f, 0x5f, WWDG_Prescaler_8);			//��ʱ������

	//��ʱ����ʼ��
	TIM6_Init(1);
	TIM7_Init(1);
}

