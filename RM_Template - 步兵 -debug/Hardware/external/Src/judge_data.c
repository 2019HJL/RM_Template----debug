#include "judge_data.h"
#include "string.h"
#include "crc.h"
#include "dma.h"
#include "usart.h"

#define FALSE 0
#define TRUE  1


//u8 judge_buf[JUDGE_DMA_RXBUFF_SIZE];

judge_get judge_info_get;
Game_info game_info;
u8 Seq_last;
FrameHeader frame_header;
//uint16_t command_id;
//u8 temp[60];


//int cnttttt=0;

extern volatile u8 shoot_num;


void Judge_Init(void)
{
//	judge_info_get.broken_frame_flag = RESET;
//	judge_info_get.broken_frame_header_flag = RESET;
//	judge_info_get.broken_offset = 0;
//	judge_info_get.buf_offset = 0;
//	judge_info_get.continuous_frame_flag = RESET;
//	judge_info_get.last_buf_offset = 0;
//	judge_info_get.last_data_size = 0;
//	judge_info_get.last_Seq = 0;
//	judge_info_get.lost_cnt = 0;
	
	
	judge_info_get.data_size = 0;
	
	judge_info_get.broken_frame_flag = RESET;
	judge_info_get.broken_frame_header_flag = RESET;
	judge_info_get.broken_offset = 0;


	judge_info_get.last_Seq = 0;
	judge_info_get.Seq_cnt  = 0;
	judge_info_get.lost_cnt = 0;
	
	DMA_USART6_Init();
}



//void judge_data_get(void)
//{
//	int a,b;
//	
//	judge_info_get.last_buf_offset = judge_info_get.buf_offset;					//数据位置
//	judge_info_get.buf_offset = JUDGE_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter( DMA1_Stream5);
//	
//	judge_info_get.last_data_size = judge_info_get.data_size;					//数据长度
//	judge_info_get.data_size =  judge_info_get.buf_offset - judge_info_get.last_buf_offset;
//	
////	memcpy( judge_info_get.last_judge_buf, judge_info_get.judge_buf, JUDGE_DMA_RXBUF_SIZE);		//读数据
//	memcpy( judge_info_get.judge_buf, (void*)&judge_dma_rxbuf[ judge_info_get.last_buf_offset], judge_info_get.data_size);//judge_info_get.buf_offset - judge_info_get.last_buf_offset);
//	
//	//判断上次是否断帧
//	if( judge_info_get.broken_frame_flag == SET)
//	{
//		//是否帧头断帧
//		if( judge_info_get.broken_frame_header_flag == SET)
//		{
//			//读帧头
//			for( a = 0; a < sizeof( FrameHeader) - judge_info_get.broken_offset; ++a)
//			{
//				//超长度检测,帧头断帧
//				if( a > judge_info_get.data_size)
//				{
//					judge_info_get.broken_frame_header_flag = SET;
//					judge_info_get.broken_frame_flag		= SET;
//					
//					judge_info_get.broken_offset = a;
//					
//					break;
//				}
//				
//				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];								
//				
//			}
//			//帧头断帧读取完成
//			if( a == sizeof( FrameHeader) - judge_info_get.broken_offset)
//				judge_info_get.broken_frame_header_flag = RESET;
//			
//			memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));

//			
//			
//			//读数据
//			for( ; a < sizeof( FrameHeader) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
//			{
//				//超长度检测,断帧
//				if( a > judge_info_get.data_size)
//				{
//					judge_info_get.broken_frame_flag = SET;
//					
//					judge_info_get.broken_offset = a;
//					
//					break;
//				}
//				
//				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];				
//			}
//			//断帧读取完成
//			if( a ==  sizeof( FrameHeader) + frame_header.DataLength - judge_info_get.broken_offset)
//				judge_info_get.broken_frame_flag = RESET;
//			
//			//CRC8和CRC16校验
//			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + frame_header.DataLength))
//			{
//				//数据录入
//				get_info_from_buf( judge_info_get.judge, frame_header.DataLength);
//			}
//			
//		}		
//		else
//		{
//			for( a = 0; a < sizeof( FrameHeader) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
//			{
//				//超长度检测,断帧
//				if( a > judge_info_get.data_size)
//				{
//					judge_info_get.broken_frame_flag = SET;
//					
//					judge_info_get.broken_offset = a;
//					
//					break;
//				}
//				
//				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];	
//			}
//			//断帧读取完成
//			if( a ==  sizeof( FrameHeader) + frame_header.DataLength - judge_info_get.broken_offset)
//				judge_info_get.broken_frame_flag = RESET;
//			
//			
//			
//			//CRC8和CRC16校验
//			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + frame_header.DataLength))
//			{
//				//数据录入
//				get_info_from_buf( judge_info_get.judge, frame_header.DataLength);
//			}
//	
//		}
//	}
//	
//	
//	
//	
//	//正常读取数据
//	for( a = 0; a < judge_info_get.data_size; ++a)
//	{
//		//判断帧头帧起始字节，每个字节检测，避免错接漏接
//		if( judge_info_get.judge_buf[a] == 0xa5)
//		{
//			//读帧头
//			for( b = 0; b < sizeof( FrameHeader); ++b)
//			{
//				//超长度检测,帧头断帧
//				if( a + b > judge_info_get.data_size)
//				{
//					judge_info_get.broken_frame_header_flag = SET;
//					judge_info_get.broken_frame_flag		= SET;
//					
//					judge_info_get.broken_offset = b;
//					
//					break;
//				}				
//				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
//				
//			}
//			memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));
//			
//			
//			
//			//读数据
//			for( ; b < sizeof( FrameHeader) + frame_header.DataLength; ++b)
//			{
//				//超长度检测,断帧
//				if( a + b > judge_info_get.data_size)
//				{
//					judge_info_get.broken_frame_flag = SET;
//					
//					judge_info_get.broken_offset = b;
//					
//					break;
//				}
//				
//				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
//				
//			}
//			
//			
//			
//			//CRC8和CRC16校验
//			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + frame_header.DataLength))
//			{
//				//数据录入
//				get_info_from_buf( judge_info_get.judge, frame_header.DataLength);//cnttttt++;

//			}
//			else
//				continue;		
//			
//		}
//	}
//	

//}

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
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);//cnttttt++;

			}
			else
				continue;		
			
		}
	}
	

}






//u8 buf_to_game_info()
//{

//}



u8 get_info_from_buf( u8 *temp, uint16_t command_id)
{
	switch( command_id)
	{
		case ROBOT_STATE:
			memcpy( &game_info.state, temp, sizeof(extGameRobotState_t));
		break;
		case HURTED:
			memcpy( &game_info.hurted, temp, sizeof( extRobotHurt_t));			
		break;
		case SHOOT_DATA:
			memcpy( &game_info.shoot_data, temp, sizeof( extShootData_t));	
		break;
		case POWER_HEAT_DATA:
			memcpy( &game_info.power_heat_data, temp, sizeof( extPowerHeatData_t));	
		break;
		case RFID:
			memcpy( &game_info.rfid, temp, sizeof( extRfidDetect_t));	
		break;
		case GAME_RESULT:
			memcpy( &game_info.winner, temp, sizeof( extGameResult_t));	
		break;
		case BUFF:
			memcpy( &game_info.buff, temp, sizeof( extGetBuff_t));	
		break;
		case GAMEROBOT_POS:
			memcpy( &game_info.pos, temp, sizeof( extGameRobotPos_t));	
		break;
		
//		case CUSTOM_DATA:
//			memcpy( &game_info.custom_data, temp, sizeof( extShowData_t));	
//		break;	
	}
	
	++judge_info_get.Seq_cnt;
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


	if(judge_info_get.CmdID == 0x0003)
	{
		if( shoot_num > 0) --shoot_num;
	}
	
	return 0;
}




