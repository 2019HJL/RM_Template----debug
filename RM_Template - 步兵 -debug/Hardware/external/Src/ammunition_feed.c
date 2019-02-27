#include "ammunition_feed.h"
#include "pid.h"
#include "pwm.h"



Encoder_data encoder_data;

pid_t ammunition_spd_pid;


pid_t pid_vecolity_pwm;
//extern int16_t EncCnt;			

//int16_t Encoder;			//编码器值
extern int Angle;
extern int16_t direction;
//extern int Apwm;

//static char Encoder_Dir = 0;




//void MOTOR_GPIO_Config(void)
//{		
////  GPIO_InitTypeDef  GPIO_InitStructure;


////  //GPIOF9,F10初始化设置
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
////  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
//	
//	GPIO_SetBits(GPIOE,GPIO_Pin_4 | GPIO_Pin_5);//GPIOF9,F10设置高，灯灭
//	 
//}

void Ammunition_Feed_Encoder_Init(void)					//拨弹电机编码器初始化
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 	//使能PORTA时钟	
	

	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //PH10复用位定时器5
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5); //PH11复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=500*4-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3，上升下降都计数
	TIM_ICStructInit(&TIM5_ICInitStructure);//将结构体中的内容缺省输入
	TIM5_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3
		
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM3的更新标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//运行更新中断

	TIM_Cmd(TIM5, ENABLE); 	//使能定时器5
}

  
int32_t Get_Encoder_Orginial_Data(void)						//获取编码器原始数据
{
	int32_t Encoder;			//编码器值
 	Encoder = TIM5->CNT;
	TIM5->CNT = 0;
	return Encoder;

}

//void TIM7_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
//	{

//	}
//	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
//}


void Get_Encoder_Data()										//获取拨弹电机编码器数据（解析）					
{
	encoder_data.encoder = Get_Encoder_Orginial_Data();
	//printf("encoder_data.encoder=%d\r\n",encoder_data.encoder);
	
	encoder_data.rpm = encoder_data.encoder * 200 * 60 / 500 / 4;		//5ms->60s,500脉冲每圈，1个脉冲被计数了4次
	
	if( encoder_data.dir == ANTI_CLOCKWISE)
		encoder_data.angle += encoder_data.encoder * 360.0f / 500.0f;
	else if( encoder_data.dir == CLOCKWISE)
		encoder_data.angle -= encoder_data.encoder * 360.0f / 500.0f;

}






//void Ammunition_Jam( int32_t speed, float angle)//卡弹 反向转
//{
// if(EncCnt==0)
// {
////	PWM4=UserMotorSpeedSet(-800);
////	delay_ms(200);	 
// }
// else
// {
//	 
// }
//}
//void AmmunitionMotorSpeedSet( int16_t MotorSpeed)//拨弹电机转速/转向设置
//{		
//	int16_t pwm_set;

//	
//	if(MotorSpeed<0) 
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_4);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5);//反转 
//		pwm_set = -MotorSpeed;
//	}
//	else if( MotorSpeed > 0)
//	{	
//		GPIO_SetBits(GPIOA, GPIO_Pin_5);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//		pwm_set = MotorSpeed;
//	}
//	else 
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//	}
////	Get_Encoder_Data();
//	PWM_Set( PWM_PD12, pwm_set);//pid_calc( &ammunition_spd_pid, encoder_data.encoder, pwm_set));
////	printf("ammupid=%f,encoder=%d\r\n", ammunition_spd_pid.pos_out,encoder_data.encoder);
//	
//	
////	return PwmOne_Set;

//}



void AmmunitionMotorSpeedSet( int16_t MotorSpeed)//拨弹电机转速/转向设置
{		
	int16_t pwm_set;

	
	if(MotorSpeed<0) 
	{
		PWM_Set( PWM_PD12, -MotorSpeed);//pwm_set);
		PWM_Set( PWM_PD13, 0);
	}
	else if( MotorSpeed > 0)
	{	
		PWM_Set( PWM_PD12, 0);
		PWM_Set( PWM_PD13, MotorSpeed);
	}
	else 
	{
		PWM_Set( PWM_PD12, 0);
		PWM_Set( PWM_PD13, 0);
	}
//	Get_Encoder_Data();
	//pid_calc( &ammunition_spd_pid, encoder_data.encoder, pwm_set));
//	printf("ammupid=%f,encoder=%d\r\n", ammunition_spd_pid.pos_out,encoder_data.encoder);
	
	
//	return PwmOne_Set;

}




//拨弹初始化
void Ammunition_Feed_Init(void)						//拨弹初始化
{
	Ammunition_Feed_Encoder_Init();						//拨弹电机编码器初始化
	pid_init( &ammunition_spd_pid, POSITION_PID, 4500, 2000 , -10.0f, 0.0f, 0);
	
	PWM_Init( PWM_PD12, 20000, 4500, TIM_OCPolarity_Low);
	PWM_Init( PWM_PD13, 20000, 4500, TIM_OCPolarity_Low);
}


//void Ammunition_Feed_Init(void)
//{
//	GPIO_SetBits(GPIOE,GPIO_Pin_4 | GPIO_Pin_5);		//停止拨弹电机
//	Ammunition_Feed_Encoder_Init();						
//}


