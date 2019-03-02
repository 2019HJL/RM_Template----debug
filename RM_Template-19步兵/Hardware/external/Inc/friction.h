#ifndef FRICTION_H_INCLUDED
#define FRICTION_H_INCLUDED

#include "sys.h"

//void Friction_Init();
void Friction_Range_Set( u32 pwm_gpio);					//油门行程设定，摩擦轮的电调第一次使用时需要设置油门行程
void Friction_Speed_Set( u32 pwm_gpio, u32 scale);		//油门，最大值999
void Friction_Range_Set_Fast(void);						////PA0,PA1摩擦轮行程设定，更快



#endif // FRICTION_H_INCLUDED
