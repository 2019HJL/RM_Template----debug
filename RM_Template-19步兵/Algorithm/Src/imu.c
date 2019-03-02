#include "imu.h"
#include "pid.h"
#include "math.h"

extern volatile float mygetqval[9];	//���ڴ�Ŵ�����ת�����������

//��ȡIMU����
void IMU_Get(int *Yaw, int * Pitch)
{
	static uint32_t yaw_cnt = 0;
	static int yaw=0, pitch=0;
	//����Ư����
	//if(yaw > 0 ) yaw+=1;
	//if(yaw < 0 ) yaw+=1;
	
	if(yaw_cnt % 18 == 0)
	{
		//����Ư����
		if(yaw > 0 ) yaw+=1;
		//if(yaw < 0 ) yaw+=1;
	}
	
	if(yaw_cnt % 30 == 0)
	{
		//����Ư����
		//if(yaw > 0 ) yaw+=1;
		if(yaw < 0 ) yaw+=1;
	}
	
	//yaw��ԭʼ���ݲ��������
	yaw += mygetqval[5];
	
	//pitch������һ�׻����˲�
	pitch = one_filter(mygetqval[0],mygetqval[3]);
	
	*Yaw = yaw;
	*Pitch = pitch;
	
	return ;
}


//һ�׻����˲�����ǰ��ʽ����ͬʱ���ڶ������ݣ�
float one_filter(float angle_m,float gyro_m)
{
	static volatile float  one_filter_angle = 0;
    float K1 =0.1; // �Լ��ٶȼ�ȡֵ��Ȩ��
    float dt=0.005;//ע�⣺dt��ȡֵΪ�˲�������ʱ��

    one_filter_angle = K1 * angle_m+ (1-K1) * (one_filter_angle + gyro_m * dt);
    return one_filter_angle;
}

//�������˲�����ǰ��ʽ����ͬʱ���ڶ������ݣ�
float kalman_filter(float angle_m,float gyro_m)
{
	static volatile float  kalman_filter_angle = 0, kalman_filter_angle_dot = 0;
    //�˲�����
    float  dt = 0.005;   //����������ʱ��
    float  P[2][2]    = {{1,0},{0,1}};
    float  Pdot[4]    = {0,0,0,0};
    float  Q_angle = 0.001;//�Ƕ��������Ŷ�,������Э����
    float  Q_gyro = 0.005;     //���ٶ��������Ŷȣ�������Ʈ������Э����
    float  R_angle = 0.5;    //���ٶȼ�Э����
    char     C_0 = 1;
    float  q_bias = 0,angle_err = 0; //q_biasΪ������Ʈ��
    float  PCt_0 = 0,PCt_1 = 0,E = 0;
    float  K_0 = 0,  K_1 = 0,  t_0 = 0,  t_1 = 0;


    kalman_filter_angle+= (gyro_m - q_bias) * dt;    //������Ԥ�ⷽ�̣���Ϊÿ��Ʈ����ͬ��


    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = - P[1][1];
    Pdot[2] = - P[1][1];
    Pdot[3]=Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * P[0][0];     //����˷��м����
    PCt_1 = C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;     //��ĸ

    K_0 = PCt_0 / E;   //����ֵ
    K_1 = PCt_1 / E;

    angle_err = angle_m - kalman_filter_angle;    
    kalman_filter_angle += K_0 * angle_err; //��״̬�Ŀ��������ƣ����ŽǶ�
    q_bias += K_1 * angle_err;
    kalman_filter_angle_dot = gyro_m-q_bias;//���Ž��ٶ�

    t_0 = PCt_0;     //��������м����
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;

    return kalman_filter_angle;
}
