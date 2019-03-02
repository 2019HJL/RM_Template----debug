#include "key.h"
	
static int key_state = KEY_UNPRESSED;

//ͨ����¼���ݲ����ʵ�����������tim.c

//ɨ�谴��״̬����¼
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

//��ȡ����״̬
u8 Key_State(void)
{
	return key_state;
}

