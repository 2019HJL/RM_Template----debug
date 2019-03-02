#include "gpio.h"

//GPIO初始化
void RM_GPIO_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	
	//GPIO时钟初始化
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOI, ENABLE);
	
	
	//LED-->PE7(Red),PF14(Green)(原理图上写反了)
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_7;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOE, &gpio);
	gpio.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init( GPIOF, &gpio);
	
	
	//Beep-->PB4
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_4;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_DOWN;
	GPIO_Init( GPIOB, &gpio);
	
	
	
	//KEY-->PD10
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_10;
	gpio.GPIO_Mode 		= GPIO_Mode_IN;
	//gpio.GPIO_OType		= GPIO_OType_;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);

	
	
//	//CAN1-->PD0,PD1
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_0 | GPIO_Pin_1;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
//	GPIO_Init( GPIOD, &gpio);
	
	
	
	//SPI5-->PF6,PF7,PF8,PF9
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_25MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
//	GPIO_Init( GPIOF, &gpio);
	
	
	//Laser(激光)-->PG13
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_13;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOG, &gpio);


	//------串口gpio在这里初始化会发送一个无意义字节
//	//USART1-->PB7(Only RX)
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_7;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
//	GPIO_Init( GPIOB, &gpio);
//	
//	
//	
//	//USART2-->PD5,PD6
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_5 | GPIO_Pin_6;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
//	GPIO_Init( GPIOD, &gpio);
//	
//	
//	
//	//USART3-->PD8,PD9
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_9;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
//	GPIO_Init( GPIOD, &gpio);
//	
//	
//	
//	//USART6-->PG9,PG14
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_9 | GPIO_Pin_14;
//	gpio.GPIO_Mode 		= GPIO_Mode_AF;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
//	GPIO_Init( GPIOG, &gpio);
	
	
	
	//PWM-->PA0
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_0;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_DOWN;
	GPIO_Init( GPIOA, &gpio);
	
	
	
	//PWM-->PA1
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_1;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOA, &gpio);
	
	
	
	//PWM-->PA2
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_2;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOA, &gpio);
	
	
	
	
	//PWM-->PA3
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_3;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOA, &gpio);
	
	
	
	
	//PWM-->PD12
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_12;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);
	
	
	
	
	//PWM-->PD13
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_13;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);
	
	
	
	//PWM-->PD14
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_14;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);
	
	
	
	
	//PWM-->PD15
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_15;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);
	
	
	
	
	//PWM-->PH10，读编码器
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_10;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_OD;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOH, &gpio);
	
	
	
	
	//PWM-->PH11，读编码器
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_11;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_OD;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOH, &gpio);
	
	
	
	
	
	//PWM-->PH12
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_12;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOH, &gpio);
	
	
	
	//PWM-->PI0
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_0;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOI, &gpio);
	
	
	
	//PWM-->PI2
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_2;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOI, &gpio);
	
	
	
	//PWM-->PI5
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_5;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOI, &gpio);
	
	
	
	//PWM-->PI6
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_6;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOI, &gpio);
	
	
	
	//PWM-->PI7
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_7;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOI, &gpio);
	
	
	
//	
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_;
//	gpio.GPIO_Mode 		= GPIO_Mode_;
//	gpio.GPIO_OType		= GPIO_OType_;
//	gpio.GPIO_Speed 	= GPIO_Speed_;
//	gpio.GPIO_PuPd		= GPIO_PuPd_;
//	GPIO_Init( GPIO, &gpio);
//	
//	
//	
//	
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_;
//	gpio.GPIO_Mode 		= GPIO_Mode_;
//	gpio.GPIO_OType		= GPIO_OType_;
//	gpio.GPIO_Speed 	= GPIO_Speed_;
//	gpio.GPIO_PuPd		= GPIO_PuPd_;
//	GPIO_Init( GPIO, &gpio);
//	
//	
//	
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_;
//	gpio.GPIO_Mode 		= GPIO_Mode_;
//	gpio.GPIO_OType		= GPIO_OType_;
//	gpio.GPIO_Speed 	= GPIO_Speed_;
//	gpio.GPIO_PuPd		= GPIO_PuPd_;
//	GPIO_Init( GPIO, &gpio);
//	








	//自定义IO口
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_10;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOF, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_9;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOI, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_5;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOA, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_4;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOA, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_5;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_1;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_4;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_0;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);
	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_3;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_1;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOB, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_2;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOC, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_8;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOB, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_6;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOE, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_12;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOE, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_5;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOE, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_4;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOE, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_1;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOF, &gpio);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_0;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOF, &gpio);
	
	
	
}
