#include "key.h"
	
static int key_state = KEY_UNPRESSED;

//通过记录数据并检测实现消抖，详见tim.c

//扫描按键状态并记录
void Key_Scan(void)
{
	static int last_key_state = KEY_UNPRESSED;
	if(  GPIO_ReadInputDataBit( GPIOD, GPIO_Pin_10)== 0 && last_key_state == KEY_PRESSED)
		 key_state = KEY_PRESSED;
	else key_state = KEY_UNPRESSED;

	if( GPIO_ReadInputDataBit( GPIOD, GPIO_Pin_10)== 0)
		last_key_state = KEY_PRESSED;
	else last_key_state = KEY_UNPRESSED;
}

//获取按键状态
u8 Key_State(void)
{
	return key_state;
}

