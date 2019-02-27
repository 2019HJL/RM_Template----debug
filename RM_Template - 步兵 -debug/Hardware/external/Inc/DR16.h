#ifndef DR16_H_INCLUDED
#define DR16_H_INCLUDED


#include "sys.h"


#define RC_ACTION_KEY_PRESS 	1
#define RC_ACTION_KEY_UNPRESS	2

#define RC_ACTION_MOUSE_L_PRESS			(0x01<<0)
#define RC_ACTION_MOUSE_L_UNPRESS		(0x01<<1)
#define RC_ACTION_MOUSE_R_PRESS			(0x01<<2)
#define RC_ACTION_MOUSE_R_UNPRESS		(0x01<<3)

enum RC_Key { RC_KEY_W, RC_KEY_S, RC_KEY_A, RC_KEY_D, RC_KEY_SHIFT, RC_KEY_CTRL, RC_KEY_Q, RC_KEY_E};



/* ----------------------- RC Channel Definition---------------------------- */ 
#define RC_CH_VALUE_MIN              ((uint16_t)364 ) 
#define RC_CH_VALUE_OFFSET           ((uint16_t)1024) 
#define RC_CH_VALUE_MAX              ((uint16_t)1684) 
 
/* ----------------------- RC Switch Definition----------------------------- */ 
#define RC_SW_UP                     ((uint16_t)1) 
#define RC_SW_MID                    ((uint16_t)3) 
#define RC_SW_DOWN                   ((uint16_t)2) 
 
/* ----------------------- PC Key Definition-------------------------------- */ 
#define KEY_PRESSED_OFFSET_W         ((uint16_t)0x01<<0) 
#define KEY_PRESSED_OFFSET_S         ((uint16_t)0x01<<1) 
#define KEY_PRESSED_OFFSET_A         ((uint16_t)0x01<<2) 
#define KEY_PRESSED_OFFSET_D         ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT     ((uint16_t)0x01<<4)		//shift,ctrl,q,e与手册不同 
#define KEY_PRESSED_OFFSET_CTRL      ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q         ((uint16_t)0x01<<6) 
#define KEY_PRESSED_OFFSET_E         ((uint16_t)0x01<<7) 
 
 
#define  RC_FRAME_LENGTH                            18u 



 
/* ----------------------- Data Struct ------------------------------------- */ 
typedef struct  
{     
	struct     
	{         
		uint16_t ch0;   
		uint16_t ch1;  
		uint16_t ch2;  
		uint16_t ch3;   
		uint8_t  s1;    
		uint8_t  s2;    
	} rc; 
 
    struct    
	{       
		int16_t x;   
		int16_t y;     
		int16_t z;      
		uint8_t press_l;    
		uint8_t press_r;  
	} mouse; 
 
    struct  
	{      
		uint16_t v;    
	} key; 
   
} RC_Ctl_t; 


/* ----------------------- Internal Data ----------------------------------- */ 

extern RC_Ctl_t RC_CtrlData; 

extern volatile u32 rc_cnt;		//失控保护

void RC_Init(void);									//接收初始化
void RemoteDataProcess(uint8_t *pData); 			//数据解调
float RC_CH_Mapping( uint16_t RC_CH_Data, float min, float max);		//遥控器数据映射
//void RC_Action(void);								//按键状态变化获取
u8 RC_Key_Action_Read( u8 rc_key, u8 key_action);						//按键状态变化读取
u8 RC_Key_Read( u8 rc_key);							//按键状态读取
u8 RC_Mouse_Action_Read( u8 mouse_action);			//鼠标状态变化获取



#endif // DR16_H_INCLUDED
