#include "judge_19_data.h"
#include "string.h"
#include "crc.h"
#include "dma.h"
#include "usart.h"
#include "beep.h"

#include "shooter_heat.h"
#include "ammunition_feed.h"

//--------------------------------------------
//2019裁判系统数据接收及解析
//--------------------------------------------



#define FALSE 0
#define TRUE  1


//u8 judge_buf[JUDGE_DMA_RXBUFF_SIZE];

judge_get judge_info_get;					//裁判系统原始数据
Game_info game_info;						//裁判系统数据
u8 Seq_last;								//上一包序号
FrameHeader frame_header;					//帧头
//uint16_t command_id;
//u8 temp[60];

uint8_t judge_tx_buf[113];					//裁判系统发送缓冲区






void Judge_Init(void)
{
	judge_info_get.data_size = 0;
	
	judge_info_get.broken_frame_flag = RESET;
	judge_info_get.broken_frame_header_flag = RESET;
	judge_info_get.broken_offset = 0;


	judge_info_get.last_Seq = 0;
	judge_info_get.Seq_cnt  = 0;
	judge_info_get.lost_cnt = 0;
	
	DMA_USART6_Init();
}





//裁判系统数据处理（单缓冲）
void judge_data_get(void)
{
	int a,b;
	
	judge_info_get.data_size = JUDGE_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter( DMA2_Stream1);		//数据长度
	
	if( judge_info_get.data_size < 0)
	{
		printf("\r\nError in judge_data_get.\r\n\r\n");
		return ;
	}

	
//	memcpy( judge_info_get.last_judge_buf, judge_info_get.judge_buf, JUDGE_DMA_RXBUF_SIZE);		
	memcpy( judge_info_get.judge_buf, (void*)&judge_dma_rxbuf[0], judge_info_get.data_size);//读数据
	
	//判断上次是否断帧
	if( judge_info_get.broken_frame_flag == SET)
	{
		//是否帧头断帧
		if( judge_info_get.broken_frame_header_flag == SET)
		{
			//读帧头
			for( a = 0; a < sizeof( FrameHeader) - judge_info_get.broken_offset; ++a)
			{
				//超长度检测,帧头断帧
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_header_flag = SET;
					judge_info_get.broken_frame_flag		= SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];								
				
			}
			//帧头断帧读取完成
			if( a == sizeof( FrameHeader) - judge_info_get.broken_offset)
				judge_info_get.broken_frame_header_flag = RESET;
			
			judge_info_get.last_Seq = frame_header.Seq;
			memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));

			
			//读命令码ID
			//超长度检测,断帧
			if( a + sizeof( judge_info_get.CmdID)> judge_info_get.data_size)
			{
				judge_info_get.broken_frame_flag		= SET;
				
				judge_info_get.broken_offset = a + judge_info_get.broken_offset;
				
			}
			else
			{
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a];
				judge_info_get.judge[ a + 1 + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a + 1];
					
				memcpy( &judge_info_get.CmdID, &judge_info_get.judge_buf[ a + judge_info_get.broken_offset], sizeof( judge_info_get.CmdID));
				
				a += sizeof( judge_info_get.CmdID);				
			}
			
			
			//读数据
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
			{
				//超长度检测,断帧
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];				
			}
			
			//读CRC16
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset; ++a)
			{
				//超长度检测,断帧
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[a + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a];
				
			}
			
			
			//断帧读取完成
			if( a ==  sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset)
				judge_info_get.broken_frame_flag = RESET;	
			
			//CRC8和CRC16校验
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//数据录入
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);
			}
			
		}		
		else
		{
			for( a = 0; a < sizeof( FrameHeader)  + sizeof( judge_info_get.CmdID) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
			{
				//超长度检测,断帧
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];	
			}

			
			
			//读CRC16
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset; ++a)
			{
				//超长度检测,断帧
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[a + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a];
				
			}
			
			//断帧读取完成
			if( a ==  sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset)
				judge_info_get.broken_frame_flag = RESET;			
			
			//CRC8和CRC16校验
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//数据录入
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);
			}
	
		}
	}
	
	
	
	
	//正常读取数据
	for( a = 0; a < judge_info_get.data_size; ++a)
	{
		//判断帧头帧起始字节，每个字节检测，避免错接漏接
		if( judge_info_get.judge_buf[a] == 0xa5)
		{
			//读帧头
			for( b = 0; b < sizeof( FrameHeader); ++b)
			{
				//超长度检测,帧头断帧
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_header_flag = SET;
					judge_info_get.broken_frame_flag		= SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}
			//帧头CRC8校验
			if( Verify_CRC8_Check_Sum( (unsigned char*)&judge_info_get.judge[0], sizeof( FrameHeader)) && b == sizeof( FrameHeader))
			{
				judge_info_get.last_Seq = frame_header.Seq;
				memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));		
			}
			else 
				break;
			
			//读命令码ID
			//超长度检测,断帧
			if( a + b + sizeof( judge_info_get.CmdID) > judge_info_get.data_size)
			{
				judge_info_get.broken_frame_flag		= SET;
				
				judge_info_get.broken_offset = b;
				
				break;
			}
			else
			{
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				judge_info_get.judge[ b + 1] = judge_info_get.judge_buf[ a + b + 1];
					
				memcpy( &judge_info_get.CmdID, &judge_info_get.judge_buf[ a + b], sizeof( judge_info_get.CmdID));
				
				b += sizeof( judge_info_get.CmdID);				
			}

			
			
			//读数据
			for( ; b < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength; ++b)
			{
				//超长度检测,断帧
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}
				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}
			
			//读CRC16
			for( ; b < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN; ++b)
			{
				//超长度检测,断帧
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}
				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}			

			
			//CRC8和CRC16校验
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//数据录入
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);

			}
			else
				continue;		
			
		}
	}
	
}







//从缓冲区录入数据
u8 get_info_from_buf( u8 *temp, uint16_t command_id)
{
	switch( command_id)
	{
		//比赛状态数据
		case GAME_STATE:
			memcpy( &game_info.game_state, temp, sizeof(ext_game_state_t));
		break;
		//比赛结果数据
		case GAME_RESULT:
			memcpy( &game_info.result, temp, sizeof( ext_game_result_t));			
		break;
		//机器人存活数据
		case GAME_ROBOT_SURVIVORS:
			memcpy( &game_info.robot_survivors, temp, sizeof( ext_game_robot_survivors_t));
		break;
		//场地事件数据
		case EVENT_DATA:
			memcpy( &game_info.event_data, temp, sizeof( ext_event_data_t));
		break;
		//补给站动作标识
		case SUPPLY_PROJECTILE_ACTION:
			memcpy( &game_info.supply_projectile_action, temp, sizeof( ext_supply_projectile_action_t));
		break;
		//补给站预约子弹
		case SUPPLY_PROJECTILE_BOOKING:
			memcpy( &game_info.supply_projectile_booking, temp, sizeof( ext_supply_projectile_booking_t));
		break;
		//比赛机器人状态
		case GAME_ROBOT_STATE:
			memcpy( &game_info.game_robot_state, temp, sizeof( ext_game_robot_state_t));
		break;
		//实时功率热量数据
		case POWER_HEAT_DATA:
			memcpy( &game_info.power_heat_data, temp, sizeof( ext_power_heat_data_t));
			Shoot_Heat_Limit();	//控制热量	
		break;
		//机器人位置
		case GAME_ROBOT_POS:
			memcpy( &game_info.game_robot_pos, temp, sizeof( ext_game_robot_pos_t));		
		break;
		//机器人增益
		case BUFF_MASK:
			memcpy( &game_info.buff_mask, temp, sizeof( ext_buff_mask_t));
		break;
		//空中机器人能量状态
		case ROBOT_ENERGY:
			memcpy( &game_info.aerial_robot_energy, temp, sizeof( aerial_robot_energy_t));
		break;
		//伤害状态
		case ROBOT_HURT:
			memcpy( &game_info.robot_hurt, temp, sizeof( ext_robot_hurt_t));
		break;
		//实时射击信息
		case SHOOT_DATA:
			memcpy( &game_info.shoot_data, temp, sizeof( ext_shoot_data_t));
			//保存射速
			if( game_info.shoot_data.bullet_speed > 10 && game_info.shoot_data.bullet_type == SHELL_17MM)		
				Shoot_Data_Save(game_info.shoot_data.bullet_speed);
			
			//printf("freq:%5d,speed:%5f\r\n",game_info.shoot_data.bullet_freq,game_info.shoot_data.bullet_speed);
		break;
		
		//参赛队自定义数据
		case INTERACTIVE_DATA:
			memcpy( &game_info.custom_data, temp, sizeof( ext_student_interactive_header_data_t));
			
			//客户端自定义数据
			if( game_info.custom_data.data_cmd_id == 0xD180)
			{
				memcpy( &game_info.custom_data_client, temp + sizeof( ext_student_interactive_header_data_t), sizeof( client_custom_data_t));
			}
			//己方机器人间通行
			else if( game_info.custom_data.data_cmd_id >= 0x200 && game_info.custom_data.data_cmd_id <= 0x2FF)
			{
				memcpy( &game_info.custom_data_client, temp + sizeof( ext_student_interactive_header_data_t), sizeof( robot_interactive_data_t));				
			}		
		break;

	}
	
	
	
	//裁判系统数据包计数
	++judge_info_get.Seq_cnt;
	
	//裁判系统数据包丢包计数
	if( frame_header.Seq - judge_info_get.last_Seq == 1 ||  frame_header.Seq - judge_info_get.last_Seq == -256);
	else ++judge_info_get.lost_cnt;
	
	
			
//		if(judge_info_get.CmdID == 0x0001)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("maxHP=%d,reaminHP=%d\r\n", game_info.state.maxHP,game_info.state.remainHP);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}
		
//		if(judge_info_get.CmdID == 0x0008)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("yaw = %f\r\n", game_info.pos.yaw);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}
//		if(judge_info_get.CmdID == 0x0003)
//		{
//			printf("CmcdID=0x%x,length = %d,sof=0x%x,seq = %d\r\n\r\n", judge_info_get.CmdID,frame_header.DataLength, frame_header.SOF, frame_header.Seq);			
//			printf("speed=%f\r\n", game_info.shoot_data.bulletSpeed);
//		
//			printf("lost_seq=%d,seq_cnt=%d\r\n",judge_info_get.lost_cnt, judge_info_get.Seq_cnt);
//		}


//	if(judge_info_get.CmdID == 0x0003)
//	{
//		if( shoot_num > 0) --shoot_num;
//	}
	


	
	
	return 0;
}





//打包要发送的数据
void judge_data_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	static uint8_t seq = 0;
	FrameHeader header;
	
	memset(judge_tx_buf, 0, 113);

	header.SOF			= 0xA5;
	header.DataLength	= len;
	header.Seq			= seq++;

	//添加帧头
	memcpy(judge_tx_buf, (uint8_t*)&header, sizeof(FrameHeader) - CRC8_LEN);
	//添加CRC8校验
	Append_CRC8_Check_Sum(judge_tx_buf, sizeof(FrameHeader));
	
	//添加cmd_id
	memcpy(&judge_tx_buf[sizeof(FrameHeader)], (uint8_t*)&cmd_id, CMD_LEN);
	
	//添加数据
	memcpy(&judge_tx_buf[sizeof(FrameHeader) + CMD_LEN], p_data, len);
	//添加CRC16校验
	Append_CRC16_Check_Sum(judge_tx_buf, sizeof(FrameHeader) + CMD_LEN + len + CRC16_LEN);

}



//发送数据给裁判系统
void judge_data_send(uint16_t data_cmd_id, uint16_t receiver_id)
{
	uint8_t data[113];		//data to send
	uint8_t temp[40];		//在这里添加自定义数据
	uint8_t i=0;
	
	ext_student_interactive_header_data_t header_data;
	
	header_data.data_cmd_id = data_cmd_id;
	header_data.send_ID		= game_info.game_robot_state.robot_id;
	header_data.receiver_ID = receiver_id;
	
	//添加交互数据头
	memcpy( data, &header_data, sizeof(ext_student_interactive_header_data_t));
	
	//添加数据
	
	//temp[] = ...;
	
	//memcpy( &data[sizeof(ext_student_interactive_header_data_t)], temp, /*要发送的数据长度*/);
	
	//数据打包
	judge_data_pack( 0x301, data, 20);
	
	//数据发送
	for( i = 0; USART_GetFlagStatus( USART6, USART_FLAG_TC) == RESET; ++i)
		USART_SendData( USART6, (uint16_t)data[i]);
}
















