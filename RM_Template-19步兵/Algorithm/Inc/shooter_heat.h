#ifndef SHOOTER_HEAT_H_INCLUDED
#define SHOOTER_HEAT_H_INCLUDED

#include "sys.h"


//英雄，步兵17mm弹丸热量上限
#define SHOOTER_HEAT_LIMIT_17_LEVEL1 90 
#define SHOOTER_HEAT_LIMIT_17_LEVEL2 360
#define SHOOTER_HEAT_LIMIT_17_LEVEL3 480

//42mm弹丸热量上限
#define SHOOTER_HEAT_LIMIT_42_LEVEL1 150
#define SHOOTER_HEAT_LIMIT_42_LEVEL2 250
#define SHOOTER_HEAT_LIMIT_42_LEVEL3 400

//哨兵热量上限
#define SHOOTER_HEAT_LIMIT_SENTRY 480




#define HEAT_EXCEED_LIMIT 		1
#define HEAT_UNEXCEED_LIMIT		2


void Shoot_Data_Save( uint16_t new_shoot_speed);
void Shoot_Heat_Limit(void);




extern uint8_t Shooter17_Exceed_Limit_Flag;
extern uint8_t Shooter42_Exceed_Limit_Flag;





#endif // SHOOTER_HEAT_H_INCLUDED
