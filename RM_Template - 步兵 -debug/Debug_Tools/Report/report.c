#include "report.h"
#include "usart.h"

void usart2_send_char(u8 c);
void usart2_niming_report( u8 fun, u8 *data, u8 len);


static short datas[20];
static short data_index = 0;

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart2_send_char(u8 c)
{

	while( USART_GetFlagStatus( USART2, USART_FLAG_TC) == RESET); 
    USART_SendData( USART2, c);   

} 
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart2_niming_report( u8 fun, u8 *data, u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)
		return;	//���28�ֽ����� 
	
	send_buf[len+3] = 0;	//У��������
	send_buf[0]		= 0X88;	//֡ͷ
	send_buf[1]		= fun;	//������
	send_buf[2]		= len;	//���ݳ���
	
	for(i=0;i<len;i++)
		send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)
		send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)
		usart2_send_char(send_buf[i]);	//�������ݵ�����1 
}


//����Ҫ��ʾ���ε�����
void Report_Data( short *data, u8 len)
{
	u8 tbuf[20];
	u8 t;
	
	if( len > 20)		//��������
		return ;
	
	for( t = 0; t < len * 2; ++t)
	{
		if( t % 2 == 0)
			tbuf[t] = ( data[t / 2] >> 8) & 0xff; 
		else tbuf[t] = data[ t / 2] & 0xff;
	}
	//printf("%d,data=%d\r\n",tbuf[2] << 8 | tbuf[3],data[1]);
	usart2_niming_report( 0xa1, tbuf, len * 2);//�Զ���֡,0XA1
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
