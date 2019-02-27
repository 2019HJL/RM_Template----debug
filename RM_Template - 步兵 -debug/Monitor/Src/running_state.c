#include "running_state.h"

static char Running_State[40][40];
static char Running_State_Cnt;

void Running_State_Init(void)
{
	Running_State_Cnt = 0;
}



void Running_State_Add( char *running_state_str)
{
	memcpy( Running_State + Running_State_Cnt++, running_state_str, strlen( running_state_str));
}


void Running_State_Report(void)
{
	char a;
	
	for( a = 0; a < Running_State_Cnt; ++a)
	{
		printf("%s\n", Running_State[a]);
	}
	Running_State_Cnt = 0;
}

