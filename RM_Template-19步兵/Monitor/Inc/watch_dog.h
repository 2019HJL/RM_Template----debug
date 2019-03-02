#ifndef WATCH_DOG_H_INCLUDED
#define WATCH_DOG_H_INCLUDED


#include "sys.h"


//独立看门狗
void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);


//窗口看门狗
void WWDG_Init(u8 tr,u8 wr,u32 fprer);




#endif // WATCH_DOG_H_INLCLUDED
