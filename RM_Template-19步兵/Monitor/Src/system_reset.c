#include "system_reset.h"


//������λ������������
void System_Reset(void)
{
	__set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();// ��λ

}


