#include "friction_ctrl.h"
#include "friction.h"
#include "DR16.h"
#include "pwm.h"


//摩擦轮控制
void Friction_Ctrl( void)
{
//	static u8 friction_state = RESET;
//	//q按下
//	if( RC_Key_Action_Read( RC_KEY_Q, RC_ACTION_KEY_PRESS) == 1)
//	{
//		if( friction_state == RESET)
//		{
//			Friction_Speed_Set( PWM_PA0, 0);
//			Friction_Speed_Set( PWM_PA1, 0);
//			friction_state = SET;
//		}
//		else if( friction_state == SET)
//		{
//			Friction_Speed_Set( PWM_PA0, 200);
//			Friction_Speed_Set( PWM_PA1, 200);
//			friction_state = RESET;
//		}
//	}
	
		if( RC_CtrlData.rc.s1 == 1 || RC_CtrlData.rc.s1 == 3)
		{
			Friction_Speed_Set( PWM_PA0, 180);
			Friction_Speed_Set( PWM_PA1, 180);
		}
		else 
		{
			Friction_Speed_Set( PWM_PA0, 0);
			Friction_Speed_Set( PWM_PA1, 0);
		}

}






