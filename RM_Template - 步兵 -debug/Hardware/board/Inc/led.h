#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "sys.h"


enum LED_Color{ LED_COLOR_RED, LED_COLOR_GREEN};		//LED颜色
enum LED_Switch{ LED_SWITCH_ON, LED_SWITCH_OFF};		//LED状态


void LED_Init(void);							//LED初始化（全部关闭）
void LED_Ctrl( u8 color, u8 button);			//LED控制（颜色，开关）
void LED_Ctrl_Switch( u8 color);				//LED开关状态反转





#endif // LED_H_INCLUDED
