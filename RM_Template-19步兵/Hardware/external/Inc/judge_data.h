#ifndef JUDGE_DATA_H_INCLUDED
#define JUDGE_DATA_H_INCLUDED

#include "sys.h"
#include "dma.h"



enum Command_ID { ROBOT_STATE = 0x0001, HURTED, SHOOT_DATA, POWER_HEAT_DATA, RFID, GAME_RESULT, BUFF, GAMEROBOT_POS, CUSTOM_DATA = 0x0100};

enum Game_Progress { NOT_START, READY, SELF_CHECKING, COUNT_DOWN_5S, BATTLING, GAME_SETTLING};
enum ValidFlag { INVALID, VALID};			//position_t
enum Armor_Type { ARMOR_FRONT, ARMOR_LEFT, ARMOR_BEHIND, ARMOR_RIGHT, ARMOR_UP1, ARMOR_UP2};		//װ��ID
enum Hurt_Type { ARMOR_HURTED, MODULE_OFFLINE, SHELL_OVERSPEED, SHELL_OVERFREQ, SHOOTER_OVERHEAT, CHASSIS_OVERPOWER};		//Ѫ���仯����
enum Bullet_Type { SHELL_17MM = 1, SHELL_42MM};
enum Card_Type { CARD_ATTACK, CARD_DEFEND, CARD_RED_HP, CARD_BLUE_HP, CARD_RED_AGENCY, CARD_BLUE_AGENCY};
enum Game_Result { GAME_TIE, WINNER_RED, WINNER_BLUE};
//enum BUFF_Type { BUFF_ATTACK, BUFF_DEFEND};
enum BUFF_Type { BLOOD_FILLING_POINT, BLOOD_FILLING_ENGINEERING, BLOOD_FILLING_CARD, RESOURCE_ISLAND_DEFENSE, OWN_LARGE_ENERGY_AGENCY, ENEMY_LARGE_ENERGY_AGENCY, OWN_SMALL_ENERGY_AGENCY, ENEMY_SMALL_ENERGY_AGENCY, ACCELERATED_COLLING, BUNKER_DEFENSE, COMLPETELY_DEFENSE, NO_SENTINEL_BASE_DEFENSE, SENTINEL_BASE_DEFENSE};

#define BUFF_GET(X) ( 1 << X)		//XΪBUFF_Type������



#define JUDGE_DATA_FRAME_LENGTH 512

#define CRC16_LEN 2


//֡ͷ
typedef __packed struct
{
	u8 SOF;					//����֡��ʼ�ֽڣ��̶�ֵΪ0xA5 
	u16 DataLength;			//����֡�� Data ���� 
	u8 Seq;					//����� 
	u8 CRC8;				//֡ͷ CRC8 У��
} FrameHeader;		





//������������Ϣ(0x0001)
typedef __packed struct
{
	uint16_t stageRemainTime;
	uint8_t gameProgress;
	uint8_t robotLevel;
	uint16_t remainHP;
	uint16_t maxHP;
//	position_t position;
} extGameRobotState_t;



//�˺�����(0x0002)
typedef __packed struct
{
	uint8_t armorType :4;		//0~3bits:��Ѫ���仯����Ϊװ���˺�ʱ����ʶװ��ID
	uint8_t hurtType  :4;		//4~7bits:Ѫ���仯����
} extRobotHurt_t;



//ʵʱ�����Ϣ(0x0003)
typedef __packed struct
{
	uint8_t bulletType;			//��������
	uint8_t bulletFreq;			//������Ƶ
	float   bulletSpeed;		//��������
//	float   reserved;			//����
} extShootData_t;


//ʵʱ������������(0x0004)
typedef __packed struct
{
	float chassisVolt;			//���������ѹ
	float chassisCurrent;		//�����������
	float chassisPower;			//�����������
	float chassisPowerBuffer;	//���̹��ʻ���
	uint16_t shooterHeat0;		//17mmǹ������
	uint16_t shooterHeat1;		//42mmǹ������
} extPowerHeatData_t;



//���ؽ�������(0x0005)
typedef __packed struct
{
	uint8_t cardType;			//������
	uint8_t cardIdx;			//�������ţ��������ֲ�ͬ����
} extRfidDetect_t;




//����ʤ������(0x0006)
typedef __packed struct
{
	uint8_t winner;
} extGameResult_t;


//Buff��ȡ����(0x0007)
typedef __packed struct
{
	uint16_t buffMusk;			//Buff���ͣ�1��ʾ��Ч
} extGetBuff_t;


//������λ�ó�����Ϣ(0x0008)
typedef __packed struct 
{    
	float x;
	float y;
	float z;
	float yaw;
} extGameRobotPos_t;


//�������Զ�������(0x0100)
typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
} extShowData_t;





typedef __packed struct
{
	extGameRobotState_t 	state;				//������������Ϣ(0x0001)
	extRobotHurt_t 			hurted;				//�˺�����(0x0002)
	extShootData_t 			shoot_data;			//ʵʱ�����Ϣ(0x0003)
	extPowerHeatData_t		power_heat_data;	//ʵʱ������������(0x0004)
	extRfidDetect_t			rfid;				//���ؽ�������(0x0005)
	extGameResult_t			winner;				//����ʤ������(0x0006)
	extGetBuff_t			buff;				//Buff��ȡ����(0x0007)
	extGameRobotPos_t		pos;				//������λ�ó�����Ϣ(0x0008)

	
//	extShowData_t			custom_data;		//�������Զ�������

} Game_info;






//��ȡ����ϵͳ����
typedef struct
{
//	int last_buf_offset;
//	int buf_offset;
//	
//	int last_data_size;
//	int data_size;
//	
////	u8 last_judge_buf[ JUDGE_DMA_RXBUF_SIZE];		//�ϴλ�ȡ������
//	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];			//�˴λ�ȡ������
//	u8 judge[ JUDGE_DATA_FRAME_LENGTH];				//һ֡������
//	
//	u8 broken_frame_flag;			//��֡��־
//	u8 broken_frame_header_flag;	//֡ͷ��֡��־
//	u8 continuous_frame_flag;		//����֡��־
//	
//	u8 broken_offset;			//��֡λ�ã���֡��־����ʱ��Ч
//	
//	u8 last_Seq;		//�ϴΰ����
//	uint32_t lost_cnt;	//��������
	
	
	int data_size;
	
	uint16_t CmdID;
	
	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];			//�˴λ�ȡ������
	u8 judge[ JUDGE_DATA_FRAME_LENGTH];				//һ֡������

	
	u8 broken_frame_flag;			//��֡��־
	u8 broken_frame_header_flag;	//֡ͷ��֡��־
	u8 broken_offset;				//��֡λ�ã���֡��־����ʱ��Ч

	u8 last_Seq;		//�ϴΰ����
	uint32_t Seq_cnt;	//�ܰ���
	uint32_t lost_cnt;	//��������
} judge_get;


u8 get_info_from_buf( u8 *temp, uint16_t command_id);
void judge_data_get(void);
void Judge_Init(void);





extern FrameHeader frame_header;
extern Game_info game_info;
//extern u8 judge_buf[ JUDGE_DMA_RXBUFF_SIZE];

extern judge_get judge_info_get;


#endif // JUDGE_DATA_H_INCLUDED
