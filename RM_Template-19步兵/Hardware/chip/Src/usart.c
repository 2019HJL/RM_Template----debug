#include "usart.h"
#include "pid_set.h"
#include "judge_data.h"
#include "dma.h"

//---------------------------------
//该文件改动较多，可能有bug
//---------------------------------

//const u8 USART_REC_LEN = 200; 	//定义调试串口最大接收字节数 200
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA = 0;       //接收状态标记	

extern uint32_t judge_data_cnt;


vision_data vision_get;
void vision_data_get(void);




//printf支持函数  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	
	while(USART_GetFlagStatus(PRINT_USART,USART_FLAG_TC)==RESET);
	USART_SendData(PRINT_USART,(uint8_t)ch);    
	return ch;
//	while((PRINT_USART->SR&0X40)==0);//循环发送,直到发送完毕   
//	PRINT_USART->DR = (u8) ch;      
//	return ch;
}
#endif




//串口1，只能用作遥控数据接收
void USART1_Init(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	//NVIC_InitTypeDef nvic;  中断写在dma处
	
	
	//USART1时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//串口1复用功能配置
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_USART1);

	
		//USART1-->PB7(Only RX)
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_7;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOB, &gpio);
	
	
	//串口初始化
	USART_DeInit(USART1);       
	usart.USART_BaudRate            = 100000;  
	usart.USART_WordLength          = USART_WordLength_8b; 
    usart.USART_StopBits            = USART_StopBits_1; 
    usart.USART_Parity              = USART_Parity_Even;					//偶校验
	usart.USART_Mode                = USART_Mode_Rx;    					//收模式
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	
	USART_Init(USART1,&usart);     
	USART_Cmd(USART1,ENABLE); 
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  			//串口1，dma接收使能
	
}




void USART2_Init( u32 bound)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;  
	
	//USART2时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//串口2复用功能配置
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5复用为USART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6复用为USART2

	
	//USART2-->PD5,PD6
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOD, &gpio);
	
	//串口初始化
	USART_DeInit(USART2);       
	usart.USART_BaudRate            = bound;  
	usart.USART_WordLength          = USART_WordLength_8b; 
    usart.USART_StopBits            = USART_StopBits_1; 
    usart.USART_Parity              = USART_Parity_No;					//无校验
	usart.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	


//	USART_DMACmd( USART2, USART_DMAReq_Rx, ENABLE);		//串口2DMA收发使能
	USART_Init( USART2,&usart);     		//串口2初始化
	USART_Cmd( USART2,ENABLE); 			//串口2使能

	USART_ClearFlag( USART2, USART_FLAG_TC);
	
	
	
	USART_ITConfig( USART2, USART_IT_IDLE, ENABLE);//串口2空闲中断

	//USART2 NVIC 配置
	nvic.NVIC_IRQChannel 					= USART2_IRQn;		//串口2中断通道
	nvic.NVIC_IRQChannelPreemptionPriority	= 3;				//抢占优先级3
	nvic.NVIC_IRQChannelSubPriority 		= 1;				//子优先级1
	nvic.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
	NVIC_Init(&nvic);	//根据指定的参数初始化NVIC寄存器

}







void USART3_Init( u32 bound)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;  
	
	//USART3时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//串口3复用功能配置
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOD9复用为USART3
	
	
	//USART3-->PD8,PD9
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);
	
	

	
	//串口初始化
	USART_DeInit(USART3);       
	usart.USART_BaudRate            = bound;  
	usart.USART_WordLength          = USART_WordLength_8b; 
    usart.USART_StopBits            = USART_StopBits_1; 
    usart.USART_Parity              = USART_Parity_No;					//无校验
	usart.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	
	USART_Init(USART3,&usart);     		//串口3初始化
	USART_Cmd(USART3,ENABLE); 			//串口3使能

	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//串口3接收非空中断使能

	//USART3 NVIC 配置
	nvic.NVIC_IRQChannel 					= USART3_IRQn;			//串口3中断通道
	nvic.NVIC_IRQChannelPreemptionPriority	= 3;					//抢占优先级3
	nvic.NVIC_IRQChannelSubPriority 		= 2;					//子优先级2
	nvic.NVIC_IRQChannelCmd 				= ENABLE;				//IRQ通道使能
	NVIC_Init(&nvic);	//根据指定的参数初始化NVIC寄存器

}





void USART6_Init( u32 bound)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;  
	
	//USART6时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	//串口3复用功能配置
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); 	//GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); 	//GPIOG14复用为USART6
	
	
		
	//USART6-->PG9,PG14
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_9 | GPIO_Pin_14;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOG, &gpio);
	
	

	
	//串口初始化
	USART_DeInit(USART6);       
	usart.USART_BaudRate            = bound;  
	usart.USART_WordLength          = USART_WordLength_8b; 
    usart.USART_StopBits            = USART_StopBits_1; 
    usart.USART_Parity              = USART_Parity_No;					//无校验
	usart.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	
	USART_Init(USART6,&usart);     		//串口6初始化
	USART_Cmd(USART6,ENABLE); 			//串口6使能

	USART_ClearFlag(USART6, USART_FLAG_TC);
	
	
	
//	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//串口6接收非空中断

	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//串口6接收空闲中断

	//USART6 NVIC 配置
	nvic.NVIC_IRQChannel 					= USART6_IRQn;			//串口6中断通道
	nvic.NVIC_IRQChannelPreemptionPriority	= 3;					//抢占优先级3
	nvic.NVIC_IRQChannelSubPriority 		= 3;					//子优先级3
	nvic.NVIC_IRQChannelCmd 				= ENABLE;				//IRQ通道使能
	NVIC_Init(&nvic);	//根据指定的参数初始化NVIC寄存器

}




void PRINT_USART_IRQHandler(void)                	//调试串口中断服务
{
	u8 Res;

	if(USART_GetITStatus(PRINT_USART, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(PRINT_USART);//(USART6->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else 
				{
					USART_RX_STA|=0x8000;	//接收完成了 
					PID_Set();
				}
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	} 

} 



//void USART2_IRQHandler(void)                	//串口2中断服务
//{

//	if( USART_GetITStatus( USART2, USART_IT_IDLE) != RESET)
//	{
//		(void)USART2->SR;
//		(void)USART2->DR;
//			

////		USART_ClearFlag( USART2, USART_FLAG_IDLE);
////		USART_ClearITPendingBit( USART2, USART_IT_IDLE);
//		
//		DMA_Cmd( DMA1_Stream5, DISABLE);
//		judge_data_get();
//	

//		
//			
//		//重启DMA
//		DMA_ClearFlag( DMA1_Stream5, DMA_FLAG_TCIF5);
//		while( DMA_GetCmdStatus( DMA1_Stream5) != DISABLE);
//		DMA_SetCurrDataCounter( DMA1_Stream5, JUDGE_DMA_RXBUF_SIZE);
//		DMA_Cmd( DMA1_Stream5, ENABLE);
////		if( DMA_GetCurrentMemoryTarget( DMA2_Stream2) == 0)
////		{
////			DMA_Cmd( DMA2_Stream2, DISABLE);
////			judge_data_get();
////			DMA_Cmd( DMA2_Stream2, ENABLE);
////		}
////		else
////		{
////			DMA_Cmd( DMA2_Stream2, DISABLE);
////			judge_data_get();
////			DMA_Cmd( DMA2_Stream2, ENABLE);
////		}
//	}
//}


void USART6_IRQHandler(void)                	//串口6中断服务
{

	if( USART_GetITStatus( USART6, USART_IT_IDLE) != RESET)
	{
		//先读SR再读DR，清除IDLE中断
		(void)USART6->SR;
		(void)USART6->DR;
			
//		USART_ClearFlag( USART2, USART_FLAG_IDLE);
//		USART_ClearITPendingBit( USART2, USART_IT_IDLE);
		
		//关闭DMA，避免接收混乱
		DMA_Cmd( DMA2_Stream1, DISABLE);
		
		judge_data_get();
		judge_data_cnt = 0;
		
			
		//重启DMA
		DMA_ClearFlag( DMA2_Stream1, DMA_FLAG_TCIF1);
		while( DMA_GetCmdStatus( DMA2_Stream1) != DISABLE);
		DMA_SetCurrDataCounter( DMA2_Stream1, JUDGE_DMA_RXBUF_SIZE);
		DMA_Cmd( DMA2_Stream1, ENABLE);
//		if( DMA_GetCurrentMemoryTarget( DMA2_Stream2) == 0)
//		{
//			DMA_Cmd( DMA2_Stream2, DISABLE);
//			judge_data_get();
//			DMA_Cmd( DMA2_Stream2, ENABLE);
//		}
//		else
//		{
//			DMA_Cmd( DMA2_Stream2, DISABLE);
//			judge_data_get();
//			DMA_Cmd( DMA2_Stream2, ENABLE);
//		}
	}
}



//void USART2_IRQHandler(void)                	//串口2中断服务程序
//{
//	static u8 judge = 0;	//接收开关判断
//	static u8 number=0;
//	u8 ch;
//	int16_t yaw,pitch;

////	static u16 p,y;
//	
//	static union
//	{
//	u8 rev[6];
//	u16 data[3];
//	} type;
//	
//	yaw = 0;
//	pitch = 0;
//	
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		
//		USART_ClearITPendingBit (USART2,USART_IT_RXNE);
//		ch=USART_ReceiveData(USART2);	
//		switch(judge)
//		{
//			case 0:
//				if(ch == 0xFF)	
//				{
//					judge++;
//				}
//			break;
//			case 1:
//				type.rev[number]=ch;		
//				number++;			
//				if(number == 6)
//				{
//					judge++;	
//					number=0;				
//				}
//			break;
//			case 2:
//				if(ch == 0xFE)
//				{
//					pitch = 	( ( ( type.data[0] & 0xff00) >> 8) | ( ( type.data[0] & 0x00ff) << 8));
//					yaw   = 	( ( ( type.data[1] & 0xff00) >> 8) | ( ( type.data[1] & 0x00ff) << 8));
//					
//					vision_get.Pitch = ( float)pitch / 100.0f;
//					vision_get.Yaw = ( float)yaw / 100.0f;
//					
//					printf("p=%d,y=%d\r\n",pitch,yaw);
//					printf("%x,%x,%x,%x,%x,%x\r\n",type.rev[0],type.rev[1],type.rev[2],type.rev[3],type.rev[4],type.rev[5]);
//					printf("vision_yaw=%f,vision_pitch=%f\r\n\r\n",vision_get.Yaw, vision_get.Pitch);

//					
//					vision_get.pattern= ( ( ( type.data[2] & 0xff00) >> 8) | ( ( type.data[2] & 0x00ff) << 8));
//										
//					//p=get.P;
//					//y=get.Y;	
//				}
//				else
//				{
//					number=0;
//					judge=0;				
//				}

//			break;			
//		}
//	}
//	

//}
//	
	
//	
//	
//void USART2_IRQHandler(void)                	//串口2中断服务程序
//{
//	if( USART_GetITStatus( USART2, USART_IT_IDLE) != RESET)
//	{
//		(void)USART2->SR;
//		(void)USART2->DR;
//			

////		USART_ClearFlag( USART2, USART_FLAG_IDLE);
////		USART_ClearITPendingBit( USART2, USART_IT_IDLE);
//		
//		DMA_Cmd( DMA1_Stream5, DISABLE);
//		vision_data_get();
////	printf("lll\r\n");

//		//重启DMA
//		DMA_ClearFlag( DMA1_Stream5, DMA_FLAG_TCIF5);
//		while( DMA_GetCmdStatus( DMA1_Stream5) != DISABLE);
//		DMA_SetCurrDataCounter( DMA1_Stream5, VISION_DMA_RXBUF_SIZE);
//		DMA_Cmd( DMA1_Stream5, ENABLE);

//	}
//}
//	

//void vision_data_get()
//{
//	uint8_t buffer[8];
//	int16_t yaw,pitch;
//	u8 a;
//	
//	yaw = 0;
//	pitch = 0;
//	
////	//数据长度
////	if( VISION_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter( DMA1_Stream5) != 8)
////	{
////		printf("VISION_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter=%d\r\n",VISION_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter( DMA1_Stream5));
////		return;
////	}
//		
//	for( a = 0; a < 8; ++a)
//		buffer[a] = vision_dma_rxbuf[a];
//	

//	if( buffer[0] == 0xff && buffer[7] == 0xfe)
//	{
//		yaw 	= ( int16_t)( buffer[2] << 8) | buffer[1];
//		pitch	= ( int16_t)( buffer[4] << 8) | buffer[3];
//		
//		vision_get.Yaw		= ( float)yaw / 100.0f;
//		vision_get.Pitch	= ( float)pitch / 100.0f;
//		
////		printf("%x,%x,%x,%x,%x,%x,%x,%x\r\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
////		printf("y=%d,p=%d\r\n",yaw,pitch);
////		printf("vision_yaw=%f,vision_pitch=%f\r\n\r\n",vision_get.Yaw, vision_get.Pitch);
//	}
//	else 
//	{
//		vision_get.Yaw		= 0.0f;
//		vision_get.Pitch	= 0.0f;
//	}
//	
//}
//	
//	
//	
//	




