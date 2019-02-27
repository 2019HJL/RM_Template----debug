#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "sys.h"


enum LED_Color{ LED_COLOR_RED, LED_COLOR_GREEN};		//LED��ɫ
enum LED_Switch{ LED_SWITCH_ON, LED_SWITCH_OFF};		//LED״̬


void LED_Init(void);							//LED��ʼ����ȫ���رգ�
void LED_Ctrl( u8 color, u8 button);			//LED���ƣ���ɫ�����أ�
void LED_Ctrl_Switch( u8 color);				//LED����״̬��ת





#endif // LED_H_INCLUDED
