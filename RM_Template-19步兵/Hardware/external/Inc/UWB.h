#ifndef UWB_H_INCLUDED
#define UWB_H_INCLUDED

#include "sys.h"
//#include "stdint.h"

typedef struct
{
	int16_t 	x;						//��λ����x�����ף�
	int16_t 	y;						//��λ����y�����ף�
	uint16_t 	yaw;					//����Ƕ�yaw���Ƕȣ����ǶȻ�׼���شų��ϼ����򣬽Ƕȷ�����ʱ�룬�����Χ��0-36000����Ӧ�Ƕ�0-360�ȣ�
	int16_t 	distance[6];			//��ǩģ�鵽������վģ��ľ���
	uint16_t 	error_type;				//��������
	uint16_t 	signal_intensity;		//�ź�ǿ��
	
} UWB_Data;


#define UWB_ERROR_RANGING_FAILURE					1			//���ʧ�ܣ����ֵΪ0-2��
#define UWB_ERROR_RANGING_RISK						2			//�����գ����ֵΪ3��
#define UWB_ERROR_MAGNETOMETER_CALIBRATION_NEEDED	4			//��������ҪУ׼
#define UWB_ERROR_SELF_TEST_FAILED					8			//�Լ�ʧ��


enum Signal_Intensity{ UWB_SIGNAL_POOR, UWB_SIGNAL_MEDIUM, UWB_SIGNAL_GOOD, UWB_SIGNAL_EXCELLENT};		//�źŵ������в�



void Get_UWB_Info( u32 uwb_id, u8 *canbuf_receive);

extern UWB_Data UWB_data;



#endif // UWB_H_INCLUDED
