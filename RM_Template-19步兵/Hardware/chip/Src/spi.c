#include "spi.h"

//板载的MPU6500用的，暂时没什么用

void SPI5_Init(void)
{

	SPI_InitTypeDef spi;
	
	GPIO_InitTypeDef gpio;

	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_2MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOF, &gpio);
	
	
	GPIO_ResetBits( GPIOF, GPIO_Pin_6);	
	
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_6;
	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_2MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init( GPIOF, &gpio);	
	
//	GPIO_StructInit( &gpio);
//	gpio.GPIO_Pin		= GPIO_Pin_1;
//	gpio.GPIO_Mode 		= GPIO_Mode_OUT;
//	gpio.GPIO_OType		= GPIO_OType_PP;
//	gpio.GPIO_Speed 	= GPIO_Speed_2MHz;
//	gpio.GPIO_PuPd		= GPIO_PuPd_NOPULL;
//	GPIO_Init( GPIOE, &gpio);
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI5, ENABLE);
	
	//SPI5复用功能配置
	//GPIO_PinAFConfig( GPIOF, GPIO_PinSource6,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource7,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource8,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource9,GPIO_AF_SPI5);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,ENABLE);//复位 SPI1 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,DISABLE);//停止复位 SPI1 
	
	
	//RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	
	
	
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_Low;//Low;			//空闲状态低电平
	spi.SPI_CPHA = SPI_CPHA_1Edge;//1Edge;			//第一个跳变沿被采样
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//128;		//128预分频
	spi.SPI_FirstBit = SPI_FirstBit_MSB;						//MSB开始传输
	spi.SPI_CRCPolynomial = 10;//10;
	
	SPI_Init( SPI5, &spi);
	SPI_Cmd( SPI5, ENABLE);
	//SPI_I2S_ClearITPendingBit( SPI5, SPI_I2S_IT_RXNE);
	
	SPI5_ReadWriteByte(0xff);//启动传输 
	SPI5_ReadWriteByte(0xff);//启动传输 
	SPI5_ReadWriteByte(0xff);//启动传输 
	SPI5_ReadWriteByte(0xff);//启动传输 

}


//参数为发送值，返回值为接收值
u8 SPI5_ReadWriteByte(u8 TxData)
{
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		//if(retry>200)return 0;
	}				//等待发送区空  	
	SPI_I2S_SendData(SPI5, TxData); 											//通过外设SPIx发送一个byte  数据
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		//if(retry>200)return 0;
	}		//等待接收完一个byte  
	return SPI_I2S_ReceiveData(SPI5);											//返回通过SPIx最近接收的数据	
 		    
}



