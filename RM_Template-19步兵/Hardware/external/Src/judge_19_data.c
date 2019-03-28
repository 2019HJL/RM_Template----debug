#include "judge_19_data.h"
#include "string.h"
#include "crc.h"
#include "dma.h"
#include "usart.h"
#include "beep.h"

#include "shooter_heat.h"
#include "ammunition_feed.h"

//--------------------------------------------
//2019����ϵͳ���ݽ��ռ�����
//--------------------------------------------



#define FALSE 0
#define TRUE  1


//u8 judge_buf[JUDGE_DMA_RXBUFF_SIZE];

judge_get judge_info_get;					//����ϵͳԭʼ����
Game_info game_info;						//����ϵͳ����
u8 Seq_last;								//��һ�����
FrameHeader frame_header;					//֡ͷ
//uint16_t command_id;
//u8 temp[60];

uint8_t judge_tx_buf[113];					//����ϵͳ���ͻ�����






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





//����ϵͳ���ݴ��������壩
void judge_data_get(void)
{
	int a,b;
	
	judge_info_get.data_size = JUDGE_DMA_RXBUF_SIZE - DMA_GetCurrDataCounter( DMA2_Stream1);		//���ݳ���
	
	if( judge_info_get.data_size < 0)
	{
		printf("\r\nError in judge_data_get.\r\n\r\n");
		return ;
	}

	
//	memcpy( judge_info_get.last_judge_buf, judge_info_get.judge_buf, JUDGE_DMA_RXBUF_SIZE);		
	memcpy( judge_info_get.judge_buf, (void*)&judge_dma_rxbuf[0], judge_info_get.data_size);//������
	
	//�ж��ϴ��Ƿ��֡
	if( judge_info_get.broken_frame_flag == SET)
	{
		//�Ƿ�֡ͷ��֡
		if( judge_info_get.broken_frame_header_flag == SET)
		{
			//��֡ͷ
			for( a = 0; a < sizeof( FrameHeader) - judge_info_get.broken_offset; ++a)
			{
				//�����ȼ��,֡ͷ��֡
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_header_flag = SET;
					judge_info_get.broken_frame_flag		= SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];								
				
			}
			//֡ͷ��֡��ȡ���
			if( a == sizeof( FrameHeader) - judge_info_get.broken_offset)
				judge_info_get.broken_frame_header_flag = RESET;
			
			judge_info_get.last_Seq = frame_header.Seq;
			memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));

			
			//��������ID
			//�����ȼ��,��֡
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
			
			
			//������
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
			{
				//�����ȼ��,��֡
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];				
			}
			
			//��CRC16
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset; ++a)
			{
				//�����ȼ��,��֡
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[a + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a];
				
			}
			
			
			//��֡��ȡ���
			if( a ==  sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset)
				judge_info_get.broken_frame_flag = RESET;	
			
			//CRC8��CRC16У��
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//����¼��
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);
			}
			
		}		
		else
		{
			for( a = 0; a < sizeof( FrameHeader)  + sizeof( judge_info_get.CmdID) + frame_header.DataLength - judge_info_get.broken_offset; ++a)
			{
				//�����ȼ��,��֡
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[ a + judge_info_get.broken_offset] = judge_info_get.judge_buf[a];	
			}

			
			
			//��CRC16
			for( ; a < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset; ++a)
			{
				//�����ȼ��,��֡
				if( a > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = a + judge_info_get.broken_offset;
					
					break;
				}
				
				judge_info_get.judge[a + judge_info_get.broken_offset] = judge_info_get.judge_buf[ a];
				
			}
			
			//��֡��ȡ���
			if( a ==  sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN - judge_info_get.broken_offset)
				judge_info_get.broken_frame_flag = RESET;			
			
			//CRC8��CRC16У��
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//����¼��
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);
			}
	
		}
	}
	
	
	
	
	//������ȡ����
	for( a = 0; a < judge_info_get.data_size; ++a)
	{
		//�ж�֡ͷ֡��ʼ�ֽڣ�ÿ���ֽڼ�⣬������©��
		if( judge_info_get.judge_buf[a] == 0xa5)
		{
			//��֡ͷ
			for( b = 0; b < sizeof( FrameHeader); ++b)
			{
				//�����ȼ��,֡ͷ��֡
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_header_flag = SET;
					judge_info_get.broken_frame_flag		= SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}
			//֡ͷCRC8У��
			if( Verify_CRC8_Check_Sum( (unsigned char*)&judge_info_get.judge[0], sizeof( FrameHeader)) && b == sizeof( FrameHeader))
			{
				judge_info_get.last_Seq = frame_header.Seq;
				memcpy( &frame_header, judge_info_get.judge, sizeof(FrameHeader));		
			}
			else 
				break;
			
			//��������ID
			//�����ȼ��,��֡
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

			
			
			//������
			for( ; b < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength; ++b)
			{
				//�����ȼ��,��֡
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}
				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}
			
			//��CRC16
			for( ; b < sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN; ++b)
			{
				//�����ȼ��,��֡
				if( a + b > judge_info_get.data_size)
				{
					judge_info_get.broken_frame_flag = SET;
					
					judge_info_get.broken_offset = b;
					
					break;
				}
				
				judge_info_get.judge[b] = judge_info_get.judge_buf[ a + b];
				
			}			

			
			//CRC8��CRC16У��
			if( Verify_CRC8_Check_Sum( (unsigned char*)&frame_header, sizeof( FrameHeader)) && Verify_CRC16_Check_Sum( judge_info_get.judge, sizeof( FrameHeader) + sizeof( judge_info_get.CmdID) + frame_header.DataLength + CRC16_LEN))
			{
				//����¼��
				get_info_from_buf( judge_info_get.judge + sizeof( FrameHeader) + sizeof( judge_info_get.CmdID), judge_info_get.CmdID);

			}
			else
				continue;		
			
		}
	}
	
}







//�ӻ�����¼������
u8 get_info_from_buf( u8 *temp, uint16_t command_id)
{
	switch( command_id)
	{
		//����״̬����
		case GAME_STATE:
			memcpy( &game_info.game_state, temp, sizeof(ext_game_state_t));
		break;
		//�����������
		case GAME_RESULT:
			memcpy( &game_info.result, temp, sizeof( ext_game_result_t));			
		break;
		//�����˴������
		case GAME_ROBOT_SURVIVORS:
			memcpy( &game_info.robot_survivors, temp, sizeof( ext_game_robot_survivors_t));
		break;
		//�����¼�����
		case EVENT_DATA:
			memcpy( &game_info.event_data, temp, sizeof( ext_event_data_t));
		break;
		//����վ������ʶ
		case SUPPLY_PROJECTILE_ACTION:
			memcpy( &game_info.supply_projectile_action, temp, sizeof( ext_supply_projectile_action_t));
		break;
		//����վԤԼ�ӵ�
		case SUPPLY_PROJECTILE_BOOKING:
			memcpy( &game_info.supply_projectile_booking, temp, sizeof( ext_supply_projectile_booking_t));
		break;
		//����������״̬
		case GAME_ROBOT_STATE:
			memcpy( &game_info.game_robot_state, temp, sizeof( ext_game_robot_state_t));
		break;
		//ʵʱ������������
		case POWER_HEAT_DATA:
			memcpy( &game_info.power_heat_data, temp, sizeof( ext_power_heat_data_t));
			Shoot_Heat_Limit();	//��������	
		break;
		//������λ��
		case GAME_ROBOT_POS:
			memcpy( &game_info.game_robot_pos, temp, sizeof( ext_game_robot_pos_t));		
		break;
		//����������
		case BUFF_MASK:
			memcpy( &game_info.buff_mask, temp, sizeof( ext_buff_mask_t));
		break;
		//���л���������״̬
		case ROBOT_ENERGY:
			memcpy( &game_info.aerial_robot_energy, temp, sizeof( aerial_robot_energy_t));
		break;
		//�˺�״̬
		case ROBOT_HURT:
			memcpy( &game_info.robot_hurt, temp, sizeof( ext_robot_hurt_t));
		break;
		//ʵʱ�����Ϣ
		case SHOOT_DATA:
			memcpy( &game_info.shoot_data, temp, sizeof( ext_shoot_data_t));
			//��������
			if( game_info.shoot_data.bullet_speed > 10 && game_info.shoot_data.bullet_type == SHELL_17MM)		
				Shoot_Data_Save(game_info.shoot_data.bullet_speed);
			
			//printf("freq:%5d,speed:%5f\r\n",game_info.shoot_data.bullet_freq,game_info.shoot_data.bullet_speed);
		break;
		
		//�������Զ�������
		case INTERACTIVE_DATA:
			memcpy( &game_info.custom_data, temp, sizeof( ext_student_interactive_header_data_t));
			
			//�ͻ����Զ�������
			if( game_info.custom_data.data_cmd_id == 0xD180)
			{
				memcpy( &game_info.custom_data_client, temp + sizeof( ext_student_interactive_header_data_t), sizeof( client_custom_data_t));
			}
			//���������˼�ͨ��
			else if( game_info.custom_data.data_cmd_id >= 0x200 && game_info.custom_data.data_cmd_id <= 0x2FF)
			{
				memcpy( &game_info.custom_data_client, temp + sizeof( ext_student_interactive_header_data_t), sizeof( robot_interactive_data_t));				
			}		
		break;

	}
	
	
	
	//����ϵͳ���ݰ�����
	++judge_info_get.Seq_cnt;
	
	//����ϵͳ���ݰ���������
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





//���Ҫ���͵�����
void judge_data_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	static uint8_t seq = 0;
	FrameHeader header;
	
	memset(judge_tx_buf, 0, 113);

	header.SOF			= 0xA5;
	header.DataLength	= len;
	header.Seq			= seq++;

	//���֡ͷ
	memcpy(judge_tx_buf, (uint8_t*)&header, sizeof(FrameHeader) - CRC8_LEN);
	//���CRC8У��
	Append_CRC8_Check_Sum(judge_tx_buf, sizeof(FrameHeader));
	
	//���cmd_id
	memcpy(&judge_tx_buf[sizeof(FrameHeader)], (uint8_t*)&cmd_id, CMD_LEN);
	
	//�������
	memcpy(&judge_tx_buf[sizeof(FrameHeader) + CMD_LEN], p_data, len);
	//���CRC16У��
	Append_CRC16_Check_Sum(judge_tx_buf, sizeof(FrameHeader) + CMD_LEN + len + CRC16_LEN);

}



//�������ݸ�����ϵͳ
void judge_data_send(uint16_t data_cmd_id, uint16_t receiver_id)
{
	uint8_t data[113];		//data to send
	uint8_t temp[40];		//����������Զ�������
	uint8_t i=0;
	
	ext_student_interactive_header_data_t header_data;
	
	header_data.data_cmd_id = data_cmd_id;
	header_data.send_ID		= game_info.game_robot_state.robot_id;
	header_data.receiver_ID = receiver_id;
	
	//��ӽ�������ͷ
	memcpy( data, &header_data, sizeof(ext_student_interactive_header_data_t));
	
	//�������
	
	//temp[] = ...;
	
	//memcpy( &data[sizeof(ext_student_interactive_header_data_t)], temp, /*Ҫ���͵����ݳ���*/);
	
	//���ݴ��
	judge_data_pack( 0x301, data, 20);
	
	//���ݷ���
	for( i = 0; USART_GetFlagStatus( USART6, USART_FLAG_TC) == RESET; ++i)
		USART_SendData( USART6, (uint16_t)data[i]);
}
















