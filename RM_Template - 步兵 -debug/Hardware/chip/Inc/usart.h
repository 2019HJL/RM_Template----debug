#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED


#include "sys.h"
#include "stm32f4xx_conf.h"
#include "stdio.h"	


#define PRINT_USART 	USART3			//�����ô���
//#define JUDGE_USART		USART2			//���д���
//#define VISION_USART	USART3			//�Ӿ�����

//extern int cnttttt;

void USART1_Init(void);				//ң�������ݽ���ר��
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void USART6_Init(u32 bound);


#define USART_REC_LEN 200

//extern const u8 USART_REC_LEN; 	//������Դ����������ֽ��� 200
extern u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART_RX_STA;       //����״̬���	


typedef struct 
{
	float Pitch; //P�� 
	float Yaw;	//Y��  
	u16 pattern; //������׼ģʽ��0��ʾ������Ч��1��ʾΪ������ģʽ��2��ʾԶ����ģʽ
	//u8 BOSS; //�����ģʽ��Ŀ��CELLλ������1 ����9
} vision_data;

extern vision_data vision_get;



#endif // USART_H_INCLUDED
