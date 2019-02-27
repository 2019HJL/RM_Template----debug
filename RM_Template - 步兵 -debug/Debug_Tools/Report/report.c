#include "report.h"
#include "usart.h"

void usart2_send_char(u8 c);
void usart2_niming_report( u8 fun, u8 *data, u8 len);


static short datas[20];
static short data_index = 0;

//串口1发送1个字符 
//c:要发送的字符
void usart2_send_char(u8 c)
{

	while( USART_GetFlagStatus( USART2, USART_FLAG_TC) == RESET); 
    USART_SendData( USART2, c);   

} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart2_niming_report( u8 fun, u8 *data, u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)
		return;	//最多28字节数据 
	
	send_buf[len+3] = 0;	//校验数置零
	send_buf[0]		= 0X88;	//帧头
	send_buf[1]		= fun;	//功能字
	send_buf[2]		= len;	//数据长度
	
	for(i=0;i<len;i++)
		send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)
		send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)
		usart2_send_char(send_buf[i]);	//发送数据到串口1 
}


//发送要显示波形的数据
void Report_Data( short *data, u8 len)
{
	u8 tbuf[20];
	u8 t;
	
	if( len > 20)		//长度限制
		return ;
	
	for( t = 0; t < len * 2; ++t)
	{
		if( t % 2 == 0)
			tbuf[t] = ( data[t / 2] >> 8) & 0xff; 
		else tbuf[t] = data[ t / 2] & 0xff;
	}
	//printf("%d,data=%d\r\n",tbuf[2] << 8 | tbuf[3],data[1]);
	usart2_niming_report( 0xa1, tbuf, len * 2);//自定义帧,0XA1
}	




void Report_Add(short data)
{
	datas[ data_index++] = data;
	
}



void Report(void)
{
	Report_Data( datas, data_index);
	data_index = 0;
}
