#ifndef AMMUNITION_FEED_CTRL_H_INCLUDED
#define AMMUNITION_FEED_CTRL_H_INCLUDED


#include "sys.h"


void Ammunition_Init(void);
void Ammunition_Ctrl(void);


extern volatile u8 shoot_num, dartle;


#endif // AMMUNITION_FEED_CTRL_H_INCLUDED
