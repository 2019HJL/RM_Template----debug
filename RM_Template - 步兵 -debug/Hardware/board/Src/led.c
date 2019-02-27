#include "led.h"

u8 LED_State_Red, LED_State_Green;

void LED_Init(void)
{
	LED_Ctrl( LED_COLOR_RED, LED_SWITCH_OFF);
	LED_Ctrl( LED_COLOR_GREEN, LED_SWITCH_OFF);
}


//LED¿ª¹Ø
void LED_Ctrl( u8 color, u8 button)
{
	switch(color)
	{
		case LED_COLOR_GREEN:
			if( button == LED_SWITCH_ON)
			{
				GPIO_ResetBits( GPIOF, GPIO_Pin_14);
				LED_State_Green = LED_SWITCH_ON;
			}
			else if( button == LED_SWITCH_OFF)
			{
				GPIO_SetBits( GPIOF, GPIO_Pin_14);
				LED_State_Green = LED_SWITCH_OFF;
			}
		break;
		case LED_COLOR_RED:
			if( button == LED_SWITCH_ON)
			{
				GPIO_ResetBits( GPIOE, GPIO_Pin_7);
				LED_State_Red = LED_SWITCH_ON;
			}
			else if( button == LED_SWITCH_OFF)
			{
				GPIO_SetBits( GPIOE, GPIO_Pin_7);
				LED_State_Red = LED_SWITCH_OFF;
			}
		break;
	}
}




void LED_Ctrl_Switch( u8 color)
{
	switch(color)
	{
		case LED_COLOR_GREEN:
			if( LED_State_Green == LED_SWITCH_ON)
			{
				GPIO_SetBits( GPIOF, GPIO_Pin_14);
				LED_State_Green = LED_SWITCH_OFF;
			}
			else if( LED_State_Green == LED_SWITCH_OFF)
			{
				GPIO_ResetBits( GPIOF, GPIO_Pin_14);
				LED_State_Green = LED_SWITCH_ON;
			}
		break;
		case LED_COLOR_RED:
			if( LED_State_Red == LED_SWITCH_ON)
			{
				GPIO_SetBits( GPIOE, GPIO_Pin_7);
				LED_State_Red = LED_SWITCH_OFF;
			}
			else if( LED_State_Red == LED_SWITCH_OFF)
			{
				GPIO_ResetBits( GPIOE, GPIO_Pin_7);
				LED_State_Red = LED_SWITCH_ON;
			}
		break;
	}
}


