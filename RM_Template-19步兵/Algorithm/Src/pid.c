#include "pid.h"
#include "usart.h"


//����ֵ
void abs_limit( float *a, float abs_max)
{
	if( *a > abs_max)
		*a = abs_max;
	if( *a < -abs_max)
		*a = -abs_max;

}

//PID��ʼ��
void pid_init( pid_t *pid, uint32_t mode, uint32_t max_out, uint32_t integral_limit, float kp, float ki, float kd)
{
	pid->IntegralLimit = integral_limit;
	pid->MaxOutput = max_out;
	pid->pid_mode = mode;
	
	pid->p = kp;
	pid->i = ki;
	pid->d = kd;
	
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	
}

//����PID
void pid_reset( pid_t *pid, float kp, float ki, float kd)
{
	pid->p = kp;
	pid->i = ki;
	pid->d = kd;
	
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	
	printf("pid reset success.p=%f,i=%f,d=%f\r\n", kp, ki, kd);
}



//����PID
float pid_calc( pid_t *pid, float get, float set)
{
	pid->get[NOW] = get;
	pid->set[NOW] = set;
	pid->err[NOW] = set - get;
	
	//����ƫ�����ֵ
	if( pid->max_err != 0 && ABS( pid->err[NOW]) > pid->max_err)
		return 0;
	//��������
	if( pid->deadband != 0 && ABS( pid->err[NOW]) < pid->deadband)
		return 0;
	
	//λ��ʽPID
	if( pid->pid_mode == POSITION_PID)
	{
		pid->pout = pid->p * pid->err[NOW];
        pid->iout += pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST] );
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out;
		
		//printf("\t\tget=%d,set=%d,p=%d,i=%d,err=%d,pid_out=%d\r\n",(int)get,(int)set,(int)pid->pout,(int)pid->iout,(int)pid->err[NOW],(int)pid->pos_out);
	}
	//����ʽPID
	else if(pid->pid_mode == DELTA_PID)
    {
        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
        pid->iout = pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
        
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->delta_u = pid->pout + pid->iout + pid->dout;
        pid->delta_out = pid->last_delta_out + pid->delta_u;
        abs_limit(&(pid->delta_out), pid->MaxOutput);
        pid->last_delta_out = pid->delta_out;	//update last time
    }
	//��������
	pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
	
    return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;
}




