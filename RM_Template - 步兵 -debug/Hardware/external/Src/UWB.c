#include "UWB.h"

UWB_Data UWB_data;


void Get_UWB_Info( u32 uwb_id, u8 *canbuf_receive)
{
	u8 t;
	if( uwb_id != 0x259)
		return ;
	
	UWB_data.x = canbuf_receive[0] << 8 | canbuf_receive[1];
	UWB_data.y = canbuf_receive[2] << 8 | canbuf_receive[3];
	UWB_data.yaw = canbuf_receive[4] << 8 | canbuf_receive[5];
	for( t = 0; t < 6; ++t)
		UWB_data.distance[t] = canbuf_receive[ 6 + t * 2] << 8 | canbuf_receive[ 6 + t * 2 + 1];
	UWB_data.error_type = ( canbuf_receive[18] << 8 | canbuf_receive[19]) & 0x3fff;
	UWB_data.signal_intensity = ( ( canbuf_receive[18] << 8 | canbuf_receive[19]) & 0xc000) >> 14;

}



