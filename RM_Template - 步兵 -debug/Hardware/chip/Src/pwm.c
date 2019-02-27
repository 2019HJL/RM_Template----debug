#include "pwm.h"
#include "usart.h"

//TIM2,TIM4,TIM5,TIM8_CH1~CH4用于外部PWM接口，TIM3_CH1用于蜂鸣器

//APB1时钟频率90MHz，APB2时钟频率?


//外部PWM所用引脚，pwm频率，pwm重装载值，pwm输出极性
void PWM_Init( u32 pwm_gpio, u32 rate, u32 arr, u16 polarity)
{
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef tim_oc;

	
	//使能对应pwm的时钟，复用对应功能
	if( pwm_gpio >= PWM_PA0 && pwm_gpio <= PWM_PA3)							//PA0~PA3
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
		GPIO_PinAFConfig( GPIOA, GPIO_PinSource0 + pwm_gpio % 100, GPIO_AF_TIM2);//printf("done1\r\n");
	}		
	else if( pwm_gpio >= PWM_PD12 && pwm_gpio <= PWM_PD15)					//PD12~PD15
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
		GPIO_PinAFConfig( GPIOD, GPIO_PinSource0 + pwm_gpio % 100, GPIO_AF_TIM4);
	}
	else if( pwm_gpio >= PWM_PH10 && pwm_gpio <= PWM_PI0)					//PH10~PH12,PI0
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE);		
		GPIO_PinAFConfig( pwm_gpio / 100 % 10 == 3? GPIOH: GPIOI, GPIO_PinSource0 + pwm_gpio % 100, GPIO_AF_TIM2);
	}
	else if( pwm_gpio >= PWM_PI5 && pwm_gpio <= PWM_PI2)					//PI5~PI7,PI2
	{
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);
		GPIO_PinAFConfig( GPIOI, GPIO_PinSource0 + pwm_gpio % 100, GPIO_AF_TIM2);
	}
	

	//定时器配置
	timer.TIM_Prescaler = (float)90000.0f / (float)rate / (float)arr * 1000.0 -1.0f;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = arr - 1;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	
	//pwm配置
	tim_oc.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_OCPolarity = polarity;//TIM_OCPolarity_High;
	//tim_oc.TIM_Pulse = 2000-1;
	tim_oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	//printf("%d\r\n",pwm_gpio);
	//printf("pwm_gpio %% 100 = %d, pwm_gpio / 100 %% 10 = %d,pwm_gpio / 10000 = %d, pwm_gpio / 1000 %% 10 = %d\r\n",pwm_gpio % 100, pwm_gpio / 100 % 10,pwm_gpio / 10000, pwm_gpio / 1000 % 10);
	
	
	
	//定时器和pwm使能
	if( pwm_gpio / 10000 == 2)
	{
		TIM_TimeBaseInit( TIM2, &timer);
		
		switch( pwm_gpio / 1000 % 10)
		{
			case 1:
				TIM_OC1Init( TIM2, &tim_oc);
				TIM_OC1PreloadConfig( TIM2, TIM_OCPreload_Enable);//printf("done2\r\n");
			break;
			case 2:
				TIM_OC2Init( TIM2, &tim_oc);
				TIM_OC2PreloadConfig( TIM2, TIM_OCPreload_Enable);
			break;
			case 3:
				TIM_OC3Init( TIM2, &tim_oc);
				TIM_OC3PreloadConfig( TIM2, TIM_OCPreload_Enable);
			break;
			case 4:
				TIM_OC4Init( TIM2, &tim_oc);
				TIM_OC4PreloadConfig( TIM2, TIM_OCPreload_Enable);
			break;
		}	
		
		TIM_ARRPreloadConfig( TIM2, ENABLE);
		TIM_Cmd( TIM2, ENABLE);
	}	
	else if( pwm_gpio / 10000 == 4)
	{
		TIM_TimeBaseInit( TIM4, &timer);
		
		switch( pwm_gpio / 1000 % 10)
		{
			case 1:
				TIM_OC1Init( TIM4, &tim_oc);
				TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable);
			break;
			case 2:
				TIM_OC2Init( TIM4, &tim_oc);
				TIM_OC2PreloadConfig( TIM4, TIM_OCPreload_Enable);
			break;
			case 3:
				TIM_OC3Init( TIM4, &tim_oc);
				TIM_OC3PreloadConfig( TIM4, TIM_OCPreload_Enable);
			break;
			case 4:
				TIM_OC4Init( TIM4, &tim_oc);
				TIM_OC4PreloadConfig( TIM4, TIM_OCPreload_Enable);
			break;
		}	
		
		TIM_ARRPreloadConfig( TIM4, ENABLE);
		TIM_Cmd( TIM4, ENABLE);
	}
	else if( pwm_gpio / 10000 == 5)
	{
		TIM_TimeBaseInit( TIM5, &timer);	
		
		switch( pwm_gpio / 1000 % 10)
		{
			case 1:
				TIM_OC1Init( TIM5, &tim_oc);
				TIM_OC1PreloadConfig( TIM5, TIM_OCPreload_Enable);
			break;
			case 2:
				TIM_OC2Init( TIM5, &tim_oc);
				TIM_OC2PreloadConfig( TIM5, TIM_OCPreload_Enable);
			break;
			case 3:
				TIM_OC3Init( TIM5, &tim_oc);
				TIM_OC3PreloadConfig( TIM5, TIM_OCPreload_Enable);
			break;
			case 4:
				TIM_OC4Init( TIM5, &tim_oc);
				TIM_OC4PreloadConfig( TIM5, TIM_OCPreload_Enable);
			break;
		}	
		
		TIM_ARRPreloadConfig( TIM5, ENABLE);
		TIM_Cmd( TIM5, ENABLE);
	}
	else if( pwm_gpio / 10000 == 8)
	{
		TIM_TimeBaseInit( TIM8, &timer);	
		
		switch( pwm_gpio / 1000 % 10)
		{
			case 1:
				TIM_OC1Init( TIM8, &tim_oc);
				TIM_OC1PreloadConfig( TIM8, TIM_OCPreload_Enable);
			break;
			case 2:
				TIM_OC2Init( TIM8, &tim_oc);
				TIM_OC2PreloadConfig( TIM8, TIM_OCPreload_Enable);
			break;
			case 3:
				TIM_OC3Init( TIM8, &tim_oc);
				TIM_OC3PreloadConfig( TIM2, TIM_OCPreload_Enable);
			break;
			case 4:
				TIM_OC4Init( TIM8, &tim_oc);
				TIM_OC4PreloadConfig( TIM8, TIM_OCPreload_Enable);
			break;
		}	
		
		TIM_ARRPreloadConfig( TIM8, ENABLE);
		TIM_Cmd( TIM8, ENABLE);
		
		TIM_CtrlPWMOutputs( TIM8, ENABLE);			//高级定时器主输出使能
	}
	
}



void PWM_Set( u32 pwm_gpio, u32 scale)
{
	void ( *tim_setcompare[4])(TIM_TypeDef* TIMx, uint32_t Compare1);
	
	tim_setcompare[0] = TIM_SetCompare1;
	tim_setcompare[1] = TIM_SetCompare2;
	tim_setcompare[2] = TIM_SetCompare3;
	tim_setcompare[3] = TIM_SetCompare4;
	
	switch( pwm_gpio / 10000)
	{
		case 2:tim_setcompare[ pwm_gpio / 1000 % 10 - 1]( TIM2, scale);/*printf(" pwm_gpio / 1000 %% 10 - 1 = %d,scale=%d\r\n", pwm_gpio / 1000 % 10 - 1,scale);*/break;
		case 4:tim_setcompare[ pwm_gpio / 1000 % 10 - 1]( TIM4, scale);break;
		case 5:tim_setcompare[ pwm_gpio / 1000 % 10 - 1]( TIM5, scale);break;
		case 8:tim_setcompare[ pwm_gpio / 1000 % 10 - 1]( TIM8, scale);break;
	}
	
}










//Beep使用的PWM
void PWM_Beep_Init(void)
{
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef tim_oc;
	
	//时钟使能
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	//功能复用
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);

	timer.TIM_Prescaler = 256 - 1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 128 - 1;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM3, &timer);

	tim_oc.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	TIM_OC1Init( TIM3, &tim_oc);
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig( TIM3, ENABLE);
	
	//TIM_Cmd( TIM3, ENABLE);
}
