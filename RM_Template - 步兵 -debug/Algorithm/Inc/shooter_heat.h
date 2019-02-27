#ifndef SHOOTER_HEAT_H_INCLUDED
#define SHOOTER_HEAT_H_INCLUDED

#include "sys.h"


//英雄，步兵17mm弹丸热量上限
#define SHOOTER_HEAT_LIMIT_17_LEVEL1 1600 
#define SHOOTER_HEAT_LIMIT_17_LEVEL2 3000
#define SHOOTER_HEAT_LIMIT_17_LEVEL3 6000

//42mm弹丸热量上限
#define SHOOTER_HEAT_LIMIT_42_LEVEL1 3200
#define SHOOTER_HEAT_LIMIT_42_LEVEL2 6400
#define SHOOTER_HEAT_LIMIT_42_LEVEL3 12800

//哨兵热量上限
#define SHOOTER_HEAT_LIMIT_SENTRY 4500

//空中热量上限
#define SHOOTER_HEAT_LIMIT_AIR 3000


#define HEAT_EXCEED_LIMIT 		1
#define HEAT_UNEXCEED_LIMIT		2


void Shoot_Heat_Calc(void);



extern uint8_t Shooter17_Exceed_Limit_Flag;
extern uint8_t Shooter42_Exceed_Limit_Flag;





#endif // SHOOTER_HEAT_H_INCLUDED
