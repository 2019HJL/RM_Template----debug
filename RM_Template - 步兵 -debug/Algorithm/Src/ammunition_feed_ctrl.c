#include "ammunition_feed_ctrl.h"
#include "ammunition_feed.h"
#include "delay.h"
#include "key.h"
#include "shooter_heat.h"
#include "DR16.h"


volatile u8 shoot_num = 0,dartle = RESET;



void Ammunition_Init(void)
{
	Ammunition_Feed_Init();
}



void Ammunition_Ctrl(void)
{
//	if( ( shoot_num > 0 || dartle == SET) && Shooter17_Exceed_Limit_Flag == HEAT_UNEXCEED_LIMIT && RC_CtrlData.rc.s2 == 1)
//	{
//		AmmunitionMotorSpeedSet(800);
//	//	if( encoder_data.encoder == 0)
//		if( KEY_Scan() == KEY_PRESSED)
//		{
//			AmmunitionMotorSpeedSet(-800);
//			delay_ms(200);		//可能需要改
//		}	
//	}
	
	if( RC_CtrlData.rc.s2 == 1)
	{
		AmmunitionMotorSpeedSet(850);
		delay_ms(2);
		if( encoder_data.encoder == 0)
		{
			AmmunitionMotorSpeedSet( -700);	
			delay_ms( 200);
		}
		

	}
	else
	{
		AmmunitionMotorSpeedSet(0);
	}

}

