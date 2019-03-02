#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include "sys.h"



//5位定时器编号，4位定时器通道编号，3位io口字母编号(a1d2h3i4)，21位io口序号
enum PWM_GPIO{ 	PWM_PA0 	= 21100, PWM_PA1 	= 22101, PWM_PA2 	= 23102, PWM_PA3 	= 24103, 				//通用定时器
				PWM_PD12 	= 41212, PWM_PD13 	= 42213, PWM_PD14 	= 43214, PWM_PD15 	= 44215,				//通用定时器
				PWM_PH10	= 51310, PWM_PH11 	= 52311, PWM_PH12 	= 53312, PWM_PI0 	= 54400, 				//通用定时器
				PWM_PI5 	= 81405, PWM_PI6 	= 82406, PWM_PI7 	= 83407,PWM_PI2 	= 84402};				//高级定时器


				
void PWM_Init( u32 pwm_gpio, u32 rate, u32 arr, u16 polarity);
void PWM_Set( u32 pwm_gpio, u32 scale);
				

void PWM_Beep_Init(void);			//Beep用
void Beep_Freq_Set(int tone_freq);			
				


#endif // PWM_H_INCLUDED
