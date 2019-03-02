#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "sys.h"

enum
{
	LLAST = 0,
	LAST = 1,
	NOW = 2,
	
	POSITION_PID,		//位置式pid
	DELTA_PID			//增量式pid
};


typedef struct
{
	float p;
    float i;
    float d;
    
    float set[3];						//目标值,包含NOW， LAST， LLAST上上次
    float get[3];						//测量值
    float err[3];						//误差
	
    
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
    
    float pos_out;						//本次位置式输出
    float last_pos_out;					//上次输出
    float delta_u;						//本次增量值
    float delta_out;					//本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;
    
	float max_err;
	float deadband;						//err < deadband return
	
    uint32_t pid_mode;
    uint32_t MaxOutput;					//输出限幅
    uint32_t IntegralLimit;				//积分限幅
    
} pid_t;



void pid_init( pid_t *pid, uint32_t mode, uint32_t max_out, uint32_t integral_limit, float kp, float ki, float kd);					//PID初始化
float pid_calc( pid_t *pid, float get, float set);																					//计算PID
void pid_reset( pid_t *pid, float kp, float ki, float kd);																			//重设PID





#endif // PID_H_INCLUDED
