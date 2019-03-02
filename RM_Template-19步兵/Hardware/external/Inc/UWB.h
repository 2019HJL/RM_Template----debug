#ifndef UWB_H_INCLUDED
#define UWB_H_INCLUDED

#include "sys.h"
//#include "stdint.h"

typedef struct
{
	int16_t 	x;						//定位坐标x（厘米）
	int16_t 	y;						//定位坐标y（厘米）
	uint16_t 	yaw;					//方向角度yaw（角度），角度基准：地磁场南极方向，角度方向：逆时针，输出范围：0-36000（对应角度0-360度）
	int16_t 	distance[6];			//标签模块到各个基站模块的距离
	uint16_t 	error_type;				//错误类型
	uint16_t 	signal_intensity;		//信号强度
	
} UWB_Data;


#define UWB_ERROR_RANGING_FAILURE					1			//测距失败，测距值为0-2个
#define UWB_ERROR_RANGING_RISK						2			//测距风险，测距值为3个
#define UWB_ERROR_MAGNETOMETER_CALIBRATION_NEEDED	4			//磁力计需要校准
#define UWB_ERROR_SELF_TEST_FAILED					8			//自检失败


enum Signal_Intensity{ UWB_SIGNAL_POOR, UWB_SIGNAL_MEDIUM, UWB_SIGNAL_GOOD, UWB_SIGNAL_EXCELLENT};		//信号的优良中差



void Get_UWB_Info( u32 uwb_id, u8 *canbuf_receive);

extern UWB_Data UWB_data;



#endif // UWB_H_INCLUDED
