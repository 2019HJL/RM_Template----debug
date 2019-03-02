#include "can.h"
#include "chassis.h"
#include "gimbal.h"
#include "UWB.h"
#include "usart.h"
#include "ammunition_feed.h"

u8 Can1_ReceiveBuffer[8];		//can1接收缓冲区


void CAN1_Init( u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{
	GPIO_InitTypeDef gpio;

	CAN_InitTypeDef 		can;
	CAN_FilterInitTypeDef 	can_filter;
	NVIC_InitTypeDef 		nvic;

	
	//时钟初始化
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE);
	
	//CAN1-->PD0,PD1
	GPIO_StructInit( &gpio);
	gpio.GPIO_Pin		= GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode 		= GPIO_Mode_AF;
	gpio.GPIO_OType		= GPIO_OType_PP;
	gpio.GPIO_Speed 	= GPIO_Speed_100MHz;
	gpio.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init( GPIOD, &gpio);	
	//复用功能配置
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	

	
	//CAN1初始化
	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = ENABLE;
	can.CAN_AWUM = DISABLE;
	can.CAN_NART = DISABLE;
	can.CAN_RFLM = DISABLE;
	can.CAN_TXFP = DISABLE;
	can.CAN_Mode = mode;
	can.CAN_SJW = tsjw;
	can.CAN_BS1 = tbs1;
	can.CAN_BS2 = tbs2;
	can.CAN_Prescaler = brp;
	CAN_Init( CAN1, &can);
	
	
	
	//配置can1过滤器
	can_filter.CAN_FilterNumber = 0;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh = 0x0000;
	can_filter.CAN_FilterIdLow = 0x0000;
	can_filter.CAN_FilterMaskIdHigh = 0x0000;
	can_filter.CAN_FilterMaskIdLow = 0x0000;
	can_filter.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit( &can_filter);
	

	//配置can1接收中断（消息挂起）
	CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE);
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvic);
}


//can1_rx0接收中断
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	int i = 0;
	CAN_Receive( CAN1, CAN_FIFO0, &RxMessage);
	for( i = 0; i < RxMessage.DLC; ++i)
	{
		Can1_ReceiveBuffer[i] = RxMessage.Data[i];
	}
	
	//从缓冲区提取信息
	Get_Chassis_Motor_Info( RxMessage.StdId, Can1_ReceiveBuffer);
	Get_Gimbal_Motor_Info( RxMessage.StdId, Can1_ReceiveBuffer);
	Get_UWB_Info( RxMessage.StdId, Can1_ReceiveBuffer);
	Get_Ammunition_Motor_Info( RxMessage.StdId, Can1_ReceiveBuffer);

}

//CAN1发送函数（标准ID）
u8 CAN1_Send_Msg( uint32_t stdid, u8 *msg, u8 len)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = stdid;
	TxMessage.ExtId = 0x12;
	TxMessage.IDE = 0;			//标准标识符
	TxMessage.RTR = 0;			//数据帧
	TxMessage.DLC = len;
	
	for( i = 0; i < len; ++i)
		TxMessage.Data[i] = msg[i];
	
	mbox = CAN_Transmit( CAN1, &TxMessage);
	i = 0;
	
	while( ( CAN_TransmitStatus( CAN1, mbox) == CAN_TxStatus_Failed) && ( i < 0xfff))
		++i;
	if( i >= 0xfff)
		return 1;
	return 0;
}
