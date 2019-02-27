#include "ammunition_feed.h"
#include "pid.h"
#include "pwm.h"



Encoder_data encoder_data;

pid_t ammunition_spd_pid;


pid_t pid_vecolity_pwm;
//extern int16_t EncCnt;			

//int16_t Encoder;			//������ֵ
extern int Angle;
extern int16_t direction;
//extern int Apwm;

//static char Encoder_Dir = 0;




//void MOTOR_GPIO_Config(void)
//{		
////  GPIO_InitTypeDef  GPIO_InitStructure;


////  //GPIOF9,F10��ʼ������
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
////  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
//	
//	GPIO_SetBits(GPIOE,GPIO_Pin_4 | GPIO_Pin_5);//GPIOF9,F10���øߣ�����
//	 
//}

void Ammunition_Feed_Encoder_Init(void)					//���������������ʼ��
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 	//ʹ��PORTAʱ��	
	

	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //PH10����λ��ʱ��5
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5); //PH11����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=500*4-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM5_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM5_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3
		
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM3�ĸ��±�־λ
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//���и����ж�

	TIM_Cmd(TIM5, ENABLE); 	//ʹ�ܶ�ʱ��5
}

  
int32_t Get_Encoder_Orginial_Data(void)						//��ȡ������ԭʼ����
{
	int32_t Encoder;			//������ֵ
 	Encoder = TIM5->CNT;
	TIM5->CNT = 0;
	return Encoder;

}

//void TIM7_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
//	{

//	}
//	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ
//}


void Get_Encoder_Data()										//��ȡ����������������ݣ�������					
{
	encoder_data.encoder = Get_Encoder_Orginial_Data();
	//printf("encoder_data.encoder=%d\r\n",encoder_data.encoder);
	
	encoder_data.rpm = encoder_data.encoder * 200 * 60 / 500 / 4;		//5ms->60s,500����ÿȦ��1�����屻������4��
	
	if( encoder_data.dir == ANTI_CLOCKWISE)
		encoder_data.angle += encoder_data.encoder * 360.0f / 500.0f;
	else if( encoder_data.dir == CLOCKWISE)
		encoder_data.angle -= encoder_data.encoder * 360.0f / 500.0f;

}






//void Ammunition_Jam( int32_t speed, float angle)//���� ����ת
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
//void AmmunitionMotorSpeedSet( int16_t MotorSpeed)//�������ת��/ת������
//{		
//	int16_t pwm_set;

//	
//	if(MotorSpeed<0) 
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_4);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5);//��ת 
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



void AmmunitionMotorSpeedSet( int16_t MotorSpeed)//�������ת��/ת������
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




//������ʼ��
void Ammunition_Feed_Init(void)						//������ʼ��
{
	Ammunition_Feed_Encoder_Init();						//���������������ʼ��
	pid_init( &ammunition_spd_pid, POSITION_PID, 4500, 2000 , -10.0f, 0.0f, 0);
	
	PWM_Init( PWM_PD12, 20000, 4500, TIM_OCPolarity_Low);
	PWM_Init( PWM_PD13, 20000, 4500, TIM_OCPolarity_Low);
}


//void Ammunition_Feed_Init(void)
//{
//	GPIO_SetBits(GPIOE,GPIO_Pin_4 | GPIO_Pin_5);		//ֹͣ�������
//	Ammunition_Feed_Encoder_Init();						
//}


