#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "sys.h"

#define KEY_PRESSED 	1			//°´ÏÂ
#define KEY_UNPRESSED 	2			//ËÉ¿ª

void Key_Scan(void);
u8 Key_State(void);


#endif // KEY_H_INCLUDED
