#include "imu.h"
#include "pid.h"
#include "math.h"

extern volatile float mygetqval[9];	//用于存放传感器转换结果的数组

//获取IMU数据
void IMU_Get(int *Yaw, int * Pitch)
{
	static uint32_t yaw_cnt = 0;
	static int yaw=0, pitch=0;
	//补偿漂移量
	//if(yaw > 0 ) yaw+=1;
	//if(yaw < 0 ) yaw+=1;
	
	if(yaw_cnt % 18 == 0)
	{
		//补偿漂移量
		if(yaw > 0 ) yaw+=1;
		//if(yaw < 0 ) yaw+=1;
	}
	
	if(yaw_cnt % 30 == 0)
	{
		//补偿漂移量
		//if(yaw > 0 ) yaw+=1;
		if(yaw < 0 ) yaw+=1;
	}
	
	//yaw轴原始数据补偿后积分
	yaw += mygetqval[5];
	
	//pitch轴数据一阶互补滤波
	pitch = one_filter(mygetqval[0],mygetqval[3]);
	
	*Yaw = yaw;
	*Pitch = pitch;
	
	return ;
}


//一阶互补滤波（当前形式不能同时用于多组数据）
float one_filter(float angle_m,float gyro_m)
{
	static volatile float  one_filter_angle = 0;
    float K1 =0.1; // 对加速度计取值的权重
    float dt=0.005;//注意：dt的取值为滤波器采样时间

    one_filter_angle = K1 * angle_m+ (1-K1) * (one_filter_angle + gyro_m * dt);
    return one_filter_angle;
}

//卡尔曼滤波（当前形式不能同时用于多组数据）
float kalman_filter(float angle_m,float gyro_m)
{
	static volatile float  kalman_filter_angle = 0, kalman_filter_angle_dot = 0;
    //滤波参数
    float  dt = 0.005;   //卡尔曼采样时间
    float  P[2][2]    = {{1,0},{0,1}};
    float  Pdot[4]    = {0,0,0,0};
    float  Q_angle = 0.001;//角度数据置信度,陀螺仪协方差
    float  Q_gyro = 0.005;     //角速度数据置信度，陀螺仪飘移噪声协方差
    float  R_angle = 0.5;    //加速度计协方差
    char     C_0 = 1;
    float  q_bias = 0,angle_err = 0; //q_bias为陀螺仪飘移
    float  PCt_0 = 0,PCt_1 = 0,E = 0;
    float  K_0 = 0,  K_1 = 0,  t_0 = 0,  t_1 = 0;


    kalman_filter_angle+= (gyro_m - q_bias) * dt;    //卡尔曼预测方程，认为每次飘移相同，


    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = - P[1][1];
    Pdot[2] = - P[1][1];
    Pdot[3]=Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * P[0][0];     //矩阵乘法中间变量
    PCt_1 = C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;     //分母

    K_0 = PCt_0 / E;   //增益值
    K_1 = PCt_1 / E;

    angle_err = angle_m - kalman_filter_angle;    
    kalman_filter_angle += K_0 * angle_err; //对状态的卡尔曼估计，最优角度
    q_bias += K_1 * angle_err;
    kalman_filter_angle_dot = gyro_m-q_bias;//最优角速度

    t_0 = PCt_0;     //矩阵计算中间变量
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;

    return kalman_filter_angle;
}
