#include "spi.h"



void SPI5_Init(void)
{

	SPI_InitTypeDef spi;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI5, ENABLE);
	
	//SPI5���ù�������
	//GPIO_PinAFConfig( GPIOF, GPIO_PinSource6,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource7,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource8,GPIO_AF_SPI5);
	GPIO_PinAFConfig( GPIOF, GPIO_PinSource9,GPIO_AF_SPI5);
	
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	
	
	
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_Low;//Low;			//����״̬�͵�ƽ
	spi.SPI_CPHA = SPI_CPHA_1Edge;//1Edge;			//��һ�������ر�����
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//128;		//128Ԥ��Ƶ
	spi.SPI_FirstBit = SPI_FirstBit_MSB;						//MSB��ʼ����
	spi.SPI_CRCPolynomial = 10;//10;
	
	SPI_Init( SPI5, &spi);
	SPI_Cmd( SPI5, ENABLE);
	SPI_I2S_ClearITPendingBit( SPI5, SPI_I2S_IT_RXNE);

}


//����Ϊ����ֵ������ֵΪ����ֵ
u8 SPI5_ReadWriteByte(u8 TxData)
{
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		//if(retry>200)return 0;
	}				//�ȴ���������  	
	SPI_I2S_SendData(SPI5, TxData); 											//ͨ������SPIx����һ��byte  ����
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		//if(retry>200)return 0;
	}		//�ȴ�������һ��byte  
	return SPI_I2S_ReceiveData(SPI5);											//����ͨ��SPIx������յ�����	
 		    
}



