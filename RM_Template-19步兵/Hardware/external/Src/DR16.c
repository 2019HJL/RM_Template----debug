#include "DR16.h"
#include "stdio.h"
#include "usart.h"
#include "dma.h"

volatile RC_Ctrl_t RC_CtrlData; 				//ң��������
volatile RC_Ctrl_t last_RC_CtrlData;			//��һ��ң��������

uint16_t RC_Action_Key_Press;					//���̰��¶���
uint16_t RC_Action_Key_Unpress;					//����̧����
uint8_t  RC_Action_Mouse;						//��궯��


static int mouse_l_last_time;		//��������������ʱ��
volatile u32 rc_cnt;			//ʧ�ر�������ʱ�������ӣ������ж����㣩

void RC_Action(void);								//����״̬�仯��ȡ


void RC_Init(void)
{
	USART1_Init();
	DMA_USART1_Init();
}


//���ݽ���
void RemoteDataProcess(uint8_t *pData) 
{     
	if(pData == NULL)    
	{       
		return;    
	}    
	
	last_RC_CtrlData = RC_CtrlData;
	
	RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF; 
	RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;  
	RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |      ((int16_t)pData[4] << 10)) & 0x07FF;  
	RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;    
	RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;  
	RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003); 
	 
	RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8); 
	RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);   
	RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);     
	 
	RC_CtrlData.mouse.press_l = pData[12]; 
	RC_CtrlData.mouse.press_r = pData[13];    
	RC_CtrlData.key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);   

	 //your control code ��.

	//printf("RC_CtrlData.rc.ch3 = %d\n", RC_CtrlData.rc.ch3);

	RC_Action();		//����״̬�仯��ȡ
	
	rc_cnt = 0;
	
//	//����
//	if( RC_Mouse_Action_Read( RC_ACTION_MOUSE_L_PRESS) == 1)
//	{
//		++shoot_num;
//	}
//	//����
//	if( RC_CtrlData.mouse.press_l == 1)
//		++mouse_l_last_time;
//	else mouse_l_last_time = 0;
//	
//	if( mouse_l_last_time >= 100)
//		dartle = SET;
//	else dartle = RESET;
	
} 


//����ӳ�䣨��bug��
float RC_CH_Mapping( uint16_t RC_CH_Data, float min, float max)
{
	return (float)(RC_CH_Data - RC_CH_VALUE_MIN) * ( max - min) / (float)( RC_CH_VALUE_MAX - RC_CH_VALUE_MIN) + min;
}


//��ȡ����״̬�仯
void RC_Action(void)
{
	u8 a;
	//����
	for( a = 0; a <= RC_KEY_E; ++a)
	{
		//���¶���
		if( (last_RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) == 0 && (RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) != 0)
			RC_Action_Key_Press |= (( uint16_t)0x01 << a);
		//̧����
		if( (last_RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) != 0 && (RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) == 0)
			RC_Action_Key_Unpress |= (( uint16_t)0x01 << a);
	}
	
	//������
	if( last_RC_CtrlData.mouse.press_l == 0 && RC_CtrlData.mouse.press_l == 1)			//���¶���
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_PRESS;
	else if( last_RC_CtrlData.mouse.press_l == 1 && RC_CtrlData.mouse.press_l == 0)		//̧����
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_UNPRESS;
	
	
	
	//����Ҽ�
	if( last_RC_CtrlData.mouse.press_l == 0 && RC_CtrlData.mouse.press_l == 1)			//���¶���
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_PRESS;
	else if( last_RC_CtrlData.mouse.press_l == 1 && RC_CtrlData.mouse.press_l == 0)		//̧����
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_UNPRESS;
}

//����״̬�仯��ȡ
u8 RC_Key_Action_Read( u8 rc_key, u8 key_action)
{
	//����
	if( (RC_Action_Key_Press & ( 0x01 << rc_key)) != 0 && key_action == RC_ACTION_KEY_PRESS)		//����
	{
		RC_Action_Key_Press &= ~( 0x01 << rc_key);			//�����־
		return 1;
	}
	//̧��
	if( (RC_Action_Key_Unpress & ( 0x01 << rc_key)) != 0 && key_action == RC_ACTION_KEY_UNPRESS)		//����
	{
		RC_Action_Key_Unpress &= ~( 0x01 << rc_key);			//�����־
		return 1;
	}
	
	return 0;
}

//���״̬�仯��ȡ������Ϊ1��δ��Ϊ0��
u8 RC_Mouse_Action_Read( u8 mouse_action)
{
	if( (RC_Action_Key_Press & ( mouse_action)) != 0 )		//����
	{
		RC_Action_Mouse &= ~( mouse_action);			//�����־
		return 1;
	}
	
	return 0;
}


//����״̬��ȡ������Ϊ1��δ��Ϊ0��
u8 RC_Key_Read( u8 rc_key)
{
	if( (RC_CtrlData.key.v & ( 0x01 << rc_key)) != 0)
		return 1;
	return 0;
}





