#include "DR16.h"
#include "stdio.h"
#include "usart.h"
#include "dma.h"

volatile RC_Ctrl_t RC_CtrlData; 				//遥控器数据
volatile RC_Ctrl_t last_RC_CtrlData;			//上一次遥控器数据

uint16_t RC_Action_Key_Press;					//键盘按下动作
uint16_t RC_Action_Key_Unpress;					//键盘抬起动作
uint8_t  RC_Action_Mouse;						//鼠标动作


static int mouse_l_last_time;		//鼠标左键持续按下时间
volatile u32 rc_cnt;			//失控保护（定时器中增加，接收中断清零）

void RC_Action(void);								//按键状态变化获取


void RC_Init(void)
{
	USART1_Init();
	DMA_USART1_Init();
}


//数据解析
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

	 //your control code ….

	//printf("RC_CtrlData.rc.ch3 = %d\n", RC_CtrlData.rc.ch3);

	RC_Action();		//按键状态变化获取
	
	rc_cnt = 0;
	
//	//点射
//	if( RC_Mouse_Action_Read( RC_ACTION_MOUSE_L_PRESS) == 1)
//	{
//		++shoot_num;
//	}
//	//连射
//	if( RC_CtrlData.mouse.press_l == 1)
//		++mouse_l_last_time;
//	else mouse_l_last_time = 0;
//	
//	if( mouse_l_last_time >= 100)
//		dartle = SET;
//	else dartle = RESET;
	
} 


//数据映射（有bug）
float RC_CH_Mapping( uint16_t RC_CH_Data, float min, float max)
{
	return (float)(RC_CH_Data - RC_CH_VALUE_MIN) * ( max - min) / (float)( RC_CH_VALUE_MAX - RC_CH_VALUE_MIN) + min;
}


//获取按键状态变化
void RC_Action(void)
{
	u8 a;
	//键盘
	for( a = 0; a <= RC_KEY_E; ++a)
	{
		//按下动作
		if( (last_RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) == 0 && (RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) != 0)
			RC_Action_Key_Press |= (( uint16_t)0x01 << a);
		//抬起动作
		if( (last_RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) != 0 && (RC_CtrlData.key.v & ( ( uint16_t)0x01 << a)) == 0)
			RC_Action_Key_Unpress |= (( uint16_t)0x01 << a);
	}
	
	//鼠标左键
	if( last_RC_CtrlData.mouse.press_l == 0 && RC_CtrlData.mouse.press_l == 1)			//按下动作
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_PRESS;
	else if( last_RC_CtrlData.mouse.press_l == 1 && RC_CtrlData.mouse.press_l == 0)		//抬起动作
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_UNPRESS;
	
	
	
	//鼠标右键
	if( last_RC_CtrlData.mouse.press_l == 0 && RC_CtrlData.mouse.press_l == 1)			//按下动作
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_PRESS;
	else if( last_RC_CtrlData.mouse.press_l == 1 && RC_CtrlData.mouse.press_l == 0)		//抬起动作
		RC_Action_Mouse |= RC_ACTION_MOUSE_L_UNPRESS;
}

//按键状态变化读取
u8 RC_Key_Action_Read( u8 rc_key, u8 key_action)
{
	//按下
	if( (RC_Action_Key_Press & ( 0x01 << rc_key)) != 0 && key_action == RC_ACTION_KEY_PRESS)		//读到
	{
		RC_Action_Key_Press &= ~( 0x01 << rc_key);			//清除标志
		return 1;
	}
	//抬起
	if( (RC_Action_Key_Unpress & ( 0x01 << rc_key)) != 0 && key_action == RC_ACTION_KEY_UNPRESS)		//读到
	{
		RC_Action_Key_Unpress &= ~( 0x01 << rc_key);			//清除标志
		return 1;
	}
	
	return 0;
}

//鼠标状态变化获取（按下为1，未按为0）
u8 RC_Mouse_Action_Read( u8 mouse_action)
{
	if( (RC_Action_Key_Press & ( mouse_action)) != 0 )		//读到
	{
		RC_Action_Mouse &= ~( mouse_action);			//清除标志
		return 1;
	}
	
	return 0;
}


//按键状态读取（按下为1，未按为0）
u8 RC_Key_Read( u8 rc_key)
{
	if( (RC_CtrlData.key.v & ( 0x01 << rc_key)) != 0)
		return 1;
	return 0;
}





