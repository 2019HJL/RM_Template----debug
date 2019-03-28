#ifndef JUDGE_19_DATA_H_INCLUDED
#define JUDGE_19_DATA_H_INCLUDED

#include "sys.h"
#include "dma.h"


//����ID
enum Command_ID { GAME_STATE = 0x0001, GAME_RESULT, GAME_ROBOT_SURVIVORS, EVENT_DATA = 0x0101, SUPPLY_PROJECTILE_ACTION, SUPPLY_PROJECTILE_BOOKING, GAME_ROBOT_STATE = 0x201, POWER_HEAT_DATA, GAME_ROBOT_POS, BUFF_MASK, ROBOT_ENERGY, ROBOT_HURT, SHOOT_DATA, INTERACTIVE_DATA};
//������ID
enum Robot_ID { RED_HERO = 1, RED_ENGINEERING, RED_INFANTRY_1, RED_INFANTRY_2, RED_INFANTRY_3, RED_AIR, RED_SENTRY,
				BLUE_HERO = 11, BLUE_ENGINEERING, BLUE_INFANTRY_1, BLUE_INFANTRY_2, BLUE_INFANTRY_3, BLUE_AIR, BLUE_SENTRY};
//�ͻ���ID
enum Client_ID { CLIENT_RED_HERO = 0x0101, CLIENT_RED_ENGINEERING, CLIENT_RED_INFANTRY_1, CLIENT_RED_INFANTRY_2, CLIENT_RED_INFANTRY_3, CLIENT_RED_AIR, CLIENT_RED_SENTRY,
				CLIENT_BLUE_HERO = 0x0111, CLIENT_BLUE_ENGINEERING, CLIENT_BLUE_INFANTRY_1, CLIENT_BLUE_INFANTRY_2, CLIENT_BLUE_INFANTRY_3, CLIENT_BLUE_AIR, CLIENT_BLUE_SENTRY};
//�����˵ȼ�
enum Robot_Level { ROBOT_LEVEL_1 = 1, ROBOT_LEVEL_2, ROBOT_LEVEL_3};
//��������
enum Game_Type { RM_COMBAT, RM_INDIVIDUAL_EVENT, RM_ICRA};
//�����׶�
enum Game_Progress { NOT_START, READY, SELF_CHECKING, COUNT_DOWN_5S, BATTLING, GAME_SETTLING};
//�������
enum Game_Result { GAME_TIE, WINNER_RED, WINNER_BLUE};
//��������
enum BUFF_Type { BLOOD_SUPPYING, MUZZLE_COOLING, DEFENSE_UP, ATTACK_UP};
//�˺�����
enum Hurt_Type { ARMOR_HURTED, MODULE_OFFLINE,SHOOTER_OVERHEAT, CHASSIS_OVERPOWER};		//Ѫ���仯����
//װ������
enum Armor_Type { ARMOR_FRONT, ARMOR_LEFT, ARMOR_BEHIND, ARMOR_RIGHT, ARMOR_UP};		//װ��ID(���ڱ�)
//�ӵ�����
enum Bullet_Type { SHELL_17MM = 1, SHELL_42MM};


#define BUFF_GET(X) ( 1 << X)		//XΪBUFF_Type������



#define JUDGE_DATA_FRAME_LENGTH 512

#define CRC16_LEN 	2
#define CRC8_LEN	1
#define CMD_LEN		2


//֡ͷ
typedef __packed struct
{
	u8 SOF;					//����֡��ʼ�ֽڣ��̶�ֵΪ0xA5 
	u16 DataLength;			//����֡�� Data ���� 
	u8 Seq;					//����� 
	u8 CRC8;				//֡ͷ CRC8 У��
} FrameHeader;		





//����״̬����(0x0001)
typedef __packed struct 
{   
	uint8_t game_type : 4;   		//��������
	uint8_t game_progress : 4;   	//��ǰ�����׶�
	uint16_t stage_remain_time; 	//��ǰ�׶�ʣ��ʱ��
} ext_game_state_t; 



//�����������(0x0002)
typedef __packed struct
{
	uint8_t winner; 				//0ƽ�֣�1�췽ʤ����2����ʤ��
} ext_game_result_t; 



//�����˴������(0x0003)  ����Ƶ�ʣ�1Hz 
typedef __packed struct 
{
	uint16_t robot_legion; 
} ext_game_robot_survivors_t; 


//�����¼�����(0x0101)  ����Ƶ�ʣ��¼��ı���� 
typedef __packed struct
{
	uint32_t event_type; 
} ext_event_data_t; 




//����վ������ʶ(0x0102)  ����Ƶ�ʣ������ı���� 
typedef __packed struct
{
	uint8_t supply_projectile_id;			//����վID
	uint8_t supply_robot_id;   				//ԤԼ������ID
	uint8_t supply_projectile_step;  		//�ӵ��ڿ���״̬
} ext_supply_projectile_action_t; 




//����վԤԼ�ӵ�(0x0103)  ����Ƶ�ʣ����� 10Hz��RM �Կ�����δ���� 
typedef __packed struct
{
	uint8_t supply_projectile_id;			//ԤԼ����վ��ID
	uint8_t supply_num;  					//ԤԼ�ӵ���Ŀ
} ext_supply_projectile_booking_t;



//����������״̬(0x0201)  ����Ƶ�ʣ�10Hz 
typedef __packed struct
{   
	uint8_t robot_id; 								//������ID
	uint8_t robot_level; 							//�����˵ȼ�
	uint16_t remain_HP;  							//������ʣ��Ѫ��
	uint16_t max_HP;  								//��������Ѫ��
	uint16_t shooter_heat0_cooling_rate;  			//������17mm�ӵ�������ȴ�ٶ� ��λ/s
	uint16_t shooter_heat0_cooling_limit; 			//������17mm�ӵ���������
	uint16_t shooter_heat1_cooling_rate; 			//������42mm�ӵ�������ȴ�ٶ� ��λ/s
	uint16_t shooter_heat1_cooling_limit;  			//������42mm�ӵ���������
	uint8_t mains_power_gimbal_output : 1;  		//gimbal������� 1 Ϊ��24V�����0 Ϊ��24v�����
	uint8_t mains_power_chassis_output : 1;  		//chassis�������1 Ϊ��24V�����0 Ϊ��24v�����
	uint8_t mains_power_shooter_output : 1; 		//shooter�������1 Ϊ��24V�����0 Ϊ��24v�����
} ext_game_robot_state_t;


//ʵʱ������������(0x0202)  ����Ƶ�ʣ�50Hz 
typedef __packed struct 
{    
	uint16_t chassis_volt;				//���������ѹ ��λ������
	uint16_t chassis_current;  			//����������� ��λ������
	float chassis_power;    			//����������� ��λ����
	uint16_t chassis_power_buffer;  	//���̹��ʻ��� ��λ������
	uint16_t shooter_heat0;    			//17mmǹ������
	uint16_t shooter_heat1; 			//42mmǹ������
} ext_power_heat_data_t; 


//������λ��(0x0203)  ����Ƶ�ʣ�10Hz 
typedef __packed struct
{
	float x;  							//λ��x���꣬��λ��m
	float y;  							//λ��y���꣬��λ��m
	float z;  							//λ��z���꣬��λ��m
	float yaw; 							//ǹ��λ�ã� ��λ����
} ext_game_robot_pos_t;




//����������(0x0204)  ����Ƶ�ʣ�״̬�ı���� 
typedef __packed struct 
{   
	uint8_t power_rune_buff; 
}ext_buff_mask_t; 



//���л���������״̬(0x0205)  ����Ƶ�ʣ�10Hz 
typedef __packed struct 
{   
	uint8_t energy_point;				//�����������
	uint8_t attack_time; 				//�ɹ���ʱ�䣬��λ��s��50s�ݼ���0��
} aerial_robot_energy_t; 



//�˺�״̬(0x0206)  ����Ƶ�ʣ��˺��������� 
typedef __packed struct
{   
	uint8_t armor_id : 4; 				//��Ѫ���仯����Ϊװ���˺�������װ��ID;  ����Ѫ���仯���ͣ��ñ�����ֵΪ 0��
	uint8_t hurt_type : 4; 				//Ѫ���仯���� 
} ext_robot_hurt_t; 



//ʵʱ�����Ϣ(0x0207)  ����Ƶ�ʣ�������� 
typedef __packed struct
{ 
	uint8_t bullet_type; 				//�ӵ�����   
	uint8_t bullet_freq; 				//�ӵ���Ƶ����λ��Hz 
	float bullet_speed; 				//�ӵ����٣���λ��m/s
} ext_shoot_data_t; 



//�������ݽ�����Ϣ��ͷ����0x0301������Ƶ�ʣ�10Hz 
typedef __packed struct 
{   
	uint16_t data_cmd_id;				//���ݶε����� ID
	uint16_t send_ID;  					//�����ߵ� ID 
	uint16_t receiver_ID; 				//�����ߵ� ID 
	///�������������ݶ� ����󳤶�Ϊ 113 
} ext_student_interactive_header_data_t; 



//�ͻ����Զ������ݣ�cmd_id:0x0301������ ID:0xD180
typedef __packed struct
{ 
	float data1; 
	float data2; 
	float data3; 
	uint8_t masks; 
} client_custom_data_t; 


//ѧ�������˼�ͨ�� cmd_id 0x0301������ ID:0x0201~0x02FF,����Ƶ�ʣ����� 10Hz����󳤶�113 
typedef __packed struct 
{
	uint8_t data[113]; 
} robot_interactive_data_t;



typedef __packed struct
{
	ext_game_state_t						game_state;							//����״̬����		(0x0001)
	ext_game_result_t						result;								//�����������		(0x0002)
	ext_game_robot_survivors_t				robot_survivors;					//�����˴������	(0x0003)  ����Ƶ�ʣ�1Hz
	ext_event_data_t						event_data;							//�����¼�����		(0x0101)  ����Ƶ�ʣ��¼��ı���� 
	ext_supply_projectile_action_t			supply_projectile_action;			//����վ������ʶ	(0x0102)  ����Ƶ�ʣ������ı���� 
	ext_supply_projectile_booking_t			supply_projectile_booking;			//����վԤԼ�ӵ�	(0x0103)  ����Ƶ�ʣ����� 10Hz��RM �Կ�����δ���� 
	ext_game_robot_state_t					game_robot_state;					//����������״̬	(0x0201)  ����Ƶ�ʣ�10Hz 
	ext_power_heat_data_t					power_heat_data;					//ʵʱ������������	(0x0202)  ����Ƶ�ʣ�50Hz 
	ext_game_robot_pos_t					game_robot_pos;						//������λ��		(0x0203)  ����Ƶ�ʣ�10Hz
	ext_buff_mask_t							buff_mask;							//����������		(0x0204)  ����Ƶ�ʣ�״̬�ı���� 
	aerial_robot_energy_t					aerial_robot_energy;				//���л���������״̬(0x0205)  ����Ƶ�ʣ�10Hz
	ext_robot_hurt_t						robot_hurt;							//�˺�״̬			(0x0206)  ����Ƶ�ʣ��˺��������� 
	ext_shoot_data_t						shoot_data;							//ʵʱ�����Ϣ		(0x0207)  ����Ƶ�ʣ�������� 


	
	ext_student_interactive_header_data_t	custom_data;						//�������Զ�������
	client_custom_data_t					custom_data_client;					//�ͻ����Զ�������
	robot_interactive_data_t				custom_data_robot;					//���������˼�ͨ��
} Game_info;






//��ȡ����ϵͳ����
typedef struct
{
	int data_size;
	
	uint16_t CmdID;
	
	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];		//�˴λ�ȡ������
	u8 judge[ JUDGE_DATA_FRAME_LENGTH];			//һ֡������

	
	u8 broken_frame_flag;						//��֡��־
	u8 broken_frame_header_flag;				//֡ͷ��֡��־
	u8 broken_offset;							//��֡λ�ã���֡��־����ʱ��Ч

	u8 last_Seq;								//�ϴΰ����
	uint32_t Seq_cnt;							//�ܰ���
	uint32_t lost_cnt;							//��������
} judge_get;


u8 get_info_from_buf( u8 *temp, uint16_t command_id);					//�ӻ�����¼������
void judge_data_get(void);												//���ղ���������ϵͳ����
void Judge_Init(void);													//����ϵͳ��ʼ��

void judge_data_send(uint16_t data_cmd_id, uint16_t receiver_id);		//����ϵͳ���ݷ���




extern FrameHeader frame_header;
extern Game_info game_info;
//extern u8 judge_buf[ JUDGE_DMA_RXBUFF_SIZE];

extern judge_get judge_info_get;


#endif // JUDGE_DATA_H_INCLUDED
