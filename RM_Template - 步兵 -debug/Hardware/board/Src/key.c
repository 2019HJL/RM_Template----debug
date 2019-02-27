#include "key.h"

//Î´¼ÓÏû¶¶
u8 KEY_Scan(void)
{
	if( GPIO_ReadInputDataBit( GPIOD, GPIO_Pin_10) == 0)
		return KEY_PRESSED;
	else return KEY_UNPRESSED;

}

