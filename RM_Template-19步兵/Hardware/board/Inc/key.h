#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "sys.h"

#define KEY_PRESSED 	1			//����
#define KEY_UNPRESSED 	2			//�ɿ�

void Key_Scan(void);
u8 Key_State(void);


#endif // KEY_H_INCLUDED
