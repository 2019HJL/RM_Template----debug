#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//此头文件仅供main.c使用

#include "sys.h"
#include "delay.h"


#include "gpio.h"
#include "led.h"
#include "can.h"
#include "usart.h"
#include "dma.h"
#include "DR16.h"
#include "pwm.h"
#include "friction.h"
#include "beep.h"
#include "tim.h"
#include "key.h"
#include "pid_set.h"
#include "laser.h"
#include "watch_dog.h"
#include "chassis_ctrl.h"
#include "gimbal_ctrl.h"
#include "spi.h"
#include "mpu6500.h"
#include "ammunition_feed_ctrl.h"
#include "judge_data.h"

void RM_Init(void);




#endif


