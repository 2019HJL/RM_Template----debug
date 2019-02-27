#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED


#include "sys.h"
#include "stm32f4xx_conf.h"
#include "stdio.h"	


#define PRINT_USART 	USART3			//调试用串口
//#define JUDGE_USART		USART2			//裁判串口
//#define VISION_USART	USART3			//视觉串口

//extern int cnttttt;

void USART1_Init(void);				//遥控器数据接收专用
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void USART6_Init(u32 bound);


#define USART_REC_LEN 200

//extern const u8 USART_REC_LEN; 	//定义调试串口最大接收字节数 200
extern u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART_RX_STA;       //接收状态标记	


typedef struct 
{
	float Pitch; //P轴 
	float Yaw;	//Y轴  
	u16 pattern; //进入瞄准模式，0表示数据无效，1表示为近距离模式，2表示远距离模式
	//u8 BOSS; //大神符模式，目标CELL位于左上1 右下9
} vision_data;

extern vision_data vision_get;



#endif // USART_H_INCLUDED
