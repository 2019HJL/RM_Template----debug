#include "system_reset.h"


//主动软复位（程序重启）
void System_Reset(void)
{
	__set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();// 复位

}


