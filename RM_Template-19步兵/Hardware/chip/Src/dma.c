#include "dma.h"
#include "usart.h"
#include "judge_data.h"


//此处注释网上见


volatile unsigned char dbus_buf[RC_FRAME_LENGTH];
volatile unsigned char judge_dma_rxbuf[JUDGE_DMA_RXBUF_SIZE];
volatile unsigned char vision_dma_rxbuf[VISION_DMA_RXBUF_SIZE];




//遥控器用串口DMA
void DMA_USART1_Init(void)
{
	DMA_InitTypeDef   dma; 

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2, ENABLE);  


	while( DMA_GetCmdStatus( DMA2_Stream5) != DISABLE){}//等待DMA可配置 


	DMA_DeInit(DMA2_Stream5);
	DMA_StructInit(&dma);
		
	dma.DMA_Channel              = DMA_Channel_4;  
	dma.DMA_PeripheralBaseAddr   = (uint32_t)&(USART1->DR);
	dma.DMA_Memory0BaseAddr      = (uint32_t)&dbus_buf[0]; 
	dma.DMA_DIR                  = DMA_DIR_PeripheralToMemory;     
	dma.DMA_BufferSize           = RC_FRAME_LENGTH;  
	dma.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;   
	dma.DMA_MemoryInc            = DMA_MemoryInc_Enable;  
	dma.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte;    
	dma.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;  
	dma.DMA_Mode                 = DMA_Mode_Circular;     
	dma.DMA_Priority             = DMA_Priority_VeryHigh;     
	dma.DMA_FIFOMode             = DMA_FIFOMode_Disable;   
	dma.DMA_FIFOThreshold        = DMA_FIFOThreshold_1QuarterFull; 
	dma.DMA_MemoryBurst          = DMA_MemoryBurst_Single;    
	dma.DMA_PeripheralBurst      = DMA_PeripheralBurst_Single;


	DMA_Init(DMA2_Stream5,&dma); 

	DMA_ITConfig( DMA2_Stream5, DMA_IT_TC, ENABLE);			//DMA中断使能

	DMA_Cmd(DMA2_Stream5,ENABLE);  
		
	/* -------------- Configure NVIC  ---------------------------------------*/    
	{        
		NVIC_InitTypeDef  nvic; 
 
//        nvic.NVIC_IRQChannel                   = USART1_IRQn;   
        nvic.NVIC_IRQChannel                   = DMA2_Stream5_IRQn;   

		nvic.NVIC_IRQChannelPreemptionPriority = 0;  
		nvic.NVIC_IRQChannelSubPriority        = 1;  
		nvic.NVIC_IRQChannelCmd                = ENABLE; 
		NVIC_Init(&nvic);   
	} 
}




//DMA_USART1中断
void DMA2_Stream5_IRQHandler(void)
{
	if( DMA_GetITStatus( DMA2_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearFlag( DMA2_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
		
		RemoteDataProcess( ( uint8_t*)dbus_buf);			//解析数据
		//printf("dma\r\n");
	}	
} 






//裁判系统用串口DMA
void DMA_USART6_Init(void)
{
	DMA_InitTypeDef   dma; 

//	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2, ENABLE);  

	
//	while( DMA_GetCmdStatus( DMA1_Stream5) != DISABLE){}//等待DMA可配置 
	while( DMA_GetCmdStatus( DMA2_Stream1) != DISABLE){}//等待DMA可配置 


//	DMA_DeInit(DMA1_Stream5);
	DMA_DeInit(DMA2_Stream1);
	
	DMA_StructInit(&dma);
		
//	dma.DMA_Channel              = DMA_Channel_4;  
//	dma.DMA_PeripheralBaseAddr   = (uint32_t)&(USART2->DR);
	dma.DMA_Channel              = DMA_Channel_5;  
	dma.DMA_PeripheralBaseAddr   = (uint32_t)&(USART6->DR);

	dma.DMA_Memory0BaseAddr      = (uint32_t)&judge_dma_rxbuf[0]; 
	dma.DMA_DIR                  = DMA_DIR_PeripheralToMemory;     
	dma.DMA_BufferSize           = JUDGE_DMA_RXBUF_SIZE;  
	dma.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;   
	dma.DMA_MemoryInc            = DMA_MemoryInc_Enable;  
	dma.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte;    
	dma.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;  
	dma.DMA_Mode                 = DMA_Mode_Normal;     
	dma.DMA_Priority             = DMA_Priority_High;     
	dma.DMA_FIFOMode             = DMA_FIFOMode_Disable;   
	dma.DMA_FIFOThreshold        = DMA_FIFOThreshold_Full; 
	dma.DMA_MemoryBurst          = DMA_MemoryBurst_Single;    
	dma.DMA_PeripheralBurst      = DMA_PeripheralBurst_Single;
	
//	DMA_DoubleBufferModeConfig( DMA1_Stream5, (uint32_t)&judge_dma_rxbuf[1][0], DMA_Memory_0);
//	DMA_DoubleBufferModeCmd( DMA1_Stream5, ENABLE);
//	
//	DMA_Init( DMA1_Stream5, &dma); 
////	
////	DMA_ITConfig( DMA1_Stream5, DMA_IT_TC, ENABLE);			//DMA中断使能
//// 
//	USART_DMACmd( USART2, USART_DMAReq_Rx, ENABLE);		//串口2DMA收发使能

//	DMA_Cmd( DMA1_Stream5, ENABLE);  

	DMA_Init( DMA2_Stream1, &dma); 
//	
//	DMA_ITConfig( DMA1_Stream5, DMA_IT_TC, ENABLE);			//DMA中断使能
// 
	USART_DMACmd( USART6, USART_DMAReq_Rx, ENABLE);		//串口6DMA收发使能

	DMA_Cmd( DMA2_Stream1, ENABLE);
		
		    /* -------------- Configure NVIC  ---------------------------------------*/    
//	{        
//	NVIC_InitTypeDef  nvic; 
// 
//	nvic.NVIC_IRQChannel                   = DMA1_Stream5_IRQn;   
//	nvic.NVIC_IRQChannelPreemptionPriority = 0;  
//	nvic.NVIC_IRQChannelSubPriority        = 2;  
//	nvic.NVIC_IRQChannelCmd                = ENABLE; 
//	NVIC_Init(&nvic);   
//	} 
}



////DMA_USART2中断,裁判系统
//void DMA1_Stream5_IRQHandler(void)
//{
//	if( DMA_GetITStatus( DMA1_Stream5, DMA_IT_TCIF2) != RESET)
//	{
////		(void)USART6->SR;
////		(void)USART6->DR;
//		
//		DMA_ClearFlag( DMA1_Stream5, DMA_FLAG_TCIF2);
//		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF2);
//		
//		if( DMA_GetCurrentMemoryTarget( DMA1_Stream5) == 0)
//		{
//			DMA_Cmd( DMA1_Stream5, DISABLE);
//			judge_data_get();
//			
//			DMA_MemoryTargetConfig( DMA1_Stream5, (uint32_t)&judge_dma_rxbuf[0][0], DMA_Memory_0);
//			
//			
////			if( DMA_GetCurrDataCounter( DMA1_Stream5) == 0)
//				
////				RemoteDataProcess( judge_dma_rxbuf[0]);			//解析数据
//			DMA_Cmd( DMA1_Stream5, ENABLE);
//		}
//		else
//		{
//			DMA_Cmd( DMA1_Stream5, DISABLE);
//			
//			judge_data_get();
//			
//			DMA_MemoryTargetConfig( DMA1_Stream5, (uint32_t)&judge_dma_rxbuf[0][0], DMA_Memory_1);
//			
////			if( DMA_GetCurrDataCounter( DMA1_Stream5) == 0)
//				
////				RemoteDataProcess( judge_dma_rxbuf[1]);			//解析数据
//			DMA_Cmd( DMA1_Stream5, ENABLE);
//		}
//		
//		//printf("dma\r\n");
//	}	
//} 




void DMA_USART2_Init(void)
{
	DMA_InitTypeDef   dma; 

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE);  
 
	while( DMA_GetCmdStatus( DMA1_Stream5) != DISABLE){}//等待DMA可配置 

	DMA_DeInit(DMA1_Stream5);

	
	DMA_StructInit(&dma);
		
	dma.DMA_Channel              = DMA_Channel_4;  
	dma.DMA_PeripheralBaseAddr   = (uint32_t)&(USART2->DR);
	dma.DMA_Memory0BaseAddr      = (uint32_t)&vision_dma_rxbuf[0]; 
	dma.DMA_DIR                  = DMA_DIR_PeripheralToMemory;     
	dma.DMA_BufferSize           = VISION_DMA_RXBUF_SIZE;  
	dma.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;   
	dma.DMA_MemoryInc            = DMA_MemoryInc_Enable;  
	dma.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte;    
	dma.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;  
	dma.DMA_Mode                 = DMA_Mode_Normal;     
	dma.DMA_Priority             = DMA_Priority_Medium;     
	dma.DMA_FIFOMode             = DMA_FIFOMode_Disable;   
	dma.DMA_FIFOThreshold        = DMA_FIFOThreshold_Full; 
	dma.DMA_MemoryBurst          = DMA_MemoryBurst_Single;    
	dma.DMA_PeripheralBurst      = DMA_PeripheralBurst_Single;
	

	DMA_Init( DMA1_Stream5, &dma); 

	USART_DMACmd( USART2, USART_DMAReq_Rx, ENABLE);		//串口2DMA收发使能

	DMA_Cmd( DMA1_Stream5, ENABLE);  
 	

}



