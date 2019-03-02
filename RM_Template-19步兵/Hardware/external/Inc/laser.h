#ifndef LASER_H_INCLUDED
#define LASER_H_INCLUDED

#include "sys.h"

#define LASER_SWITCH_ON 	1
#define LASER_SWITCH_OFF	2


void Laser_Init(void);
void Laser_Ctrl( u8 laser_state);
void Laser_Switch(void);



#endif // LASER_H_INCLUDED
