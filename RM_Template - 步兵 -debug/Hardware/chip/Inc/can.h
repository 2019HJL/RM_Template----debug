#ifndef CAN_H_INCLUDED
#define CAN_H_INCLUDED


#include "sys.h"


void CAN1_Init( u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);
u8 CAN1_Send_Msg( uint32_t stdid, u8 *msg, u8 len);						





extern u8 Can1_ReceiveBuffer[8];		//can1Ω” ’ª∫≥Â«¯


#endif // CAN_H_INCLUDED
