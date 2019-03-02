#ifndef JUDGE_DATA_H_INCLUDED
#define JUDGE_DATA_H_INCLUDED

#include "sys.h"
#include "dma.h"



enum Command_ID { ROBOT_STATE = 0x0001, HURTED, SHOOT_DATA, POWER_HEAT_DATA, RFID, GAME_RESULT, BUFF, GAMEROBOT_POS, CUSTOM_DATA = 0x0100};

enum Game_Progress { NOT_START, READY, SELF_CHECKING, COUNT_DOWN_5S, BATTLING, GAME_SETTLING};
enum ValidFlag { INVALID, VALID};			//position_t
enum Armor_Type { ARMOR_FRONT, ARMOR_LEFT, ARMOR_BEHIND, ARMOR_RIGHT, ARMOR_UP1, ARMOR_UP2};		//装甲ID
enum Hurt_Type { ARMOR_HURTED, MODULE_OFFLINE, SHELL_OVERSPEED, SHELL_OVERFREQ, SHOOTER_OVERHEAT, CHASSIS_OVERPOWER};		//血量变化类型
enum Bullet_Type { SHELL_17MM = 1, SHELL_42MM};
enum Card_Type { CARD_ATTACK, CARD_DEFEND, CARD_RED_HP, CARD_BLUE_HP, CARD_RED_AGENCY, CARD_BLUE_AGENCY};
enum Game_Result { GAME_TIE, WINNER_RED, WINNER_BLUE};
//enum BUFF_Type { BUFF_ATTACK, BUFF_DEFEND};
enum BUFF_Type { BLOOD_FILLING_POINT, BLOOD_FILLING_ENGINEERING, BLOOD_FILLING_CARD, RESOURCE_ISLAND_DEFENSE, OWN_LARGE_ENERGY_AGENCY, ENEMY_LARGE_ENERGY_AGENCY, OWN_SMALL_ENERGY_AGENCY, ENEMY_SMALL_ENERGY_AGENCY, ACCELERATED_COLLING, BUNKER_DEFENSE, COMLPETELY_DEFENSE, NO_SENTINEL_BASE_DEFENSE, SENTINEL_BASE_DEFENSE};

#define BUFF_GET(X) ( 1 << X)		//X为BUFF_Type的内容



#define JUDGE_DATA_FRAME_LENGTH 512

#define CRC16_LEN 2


//帧头
typedef __packed struct
{
	u8 SOF;					//数据帧起始字节，固定值为0xA5 
	u16 DataLength;			//数据帧内 Data 长度 
	u8 Seq;					//包序号 
	u8 CRC8;				//帧头 CRC8 校验
} FrameHeader;		





//比赛机器人信息(0x0001)
typedef __packed struct
{
	uint16_t stageRemainTime;
	uint8_t gameProgress;
	uint8_t robotLevel;
	uint16_t remainHP;
	uint16_t maxHP;
//	position_t position;
} extGameRobotState_t;



//伤害数据(0x0002)
typedef __packed struct
{
	uint8_t armorType :4;		//0~3bits:若血量变化类型为装甲伤害时，标识装甲ID
	uint8_t hurtType  :4;		//4~7bits:血量变化类型
} extRobotHurt_t;



//实时射击信息(0x0003)
typedef __packed struct
{
	uint8_t bulletType;			//弹丸类型
	uint8_t bulletFreq;			//弹丸射频
	float   bulletSpeed;		//弹丸射速
//	float   reserved;			//保留
} extShootData_t;


//实时功率热量数据(0x0004)
typedef __packed struct
{
	float chassisVolt;			//底盘输出电压
	float chassisCurrent;		//底盘输出电流
	float chassisPower;			//底盘输出功率
	float chassisPowerBuffer;	//底盘功率缓冲
	uint16_t shooterHeat0;		//17mm枪口热量
	uint16_t shooterHeat1;		//42mm枪口热量
} extPowerHeatData_t;



//场地交互数据(0x0005)
typedef __packed struct
{
	uint8_t cardType;			//卡类型
	uint8_t cardIdx;			//卡索引号，用于区分不同区域
} extRfidDetect_t;




//比赛胜负数据(0x0006)
typedef __packed struct
{
	uint8_t winner;
} extGameResult_t;


//Buff获取数据(0x0007)
typedef __packed struct
{
	uint16_t buffMusk;			//Buff类型，1表示有效
} extGetBuff_t;


//机器人位置朝向信息(0x0008)
typedef __packed struct 
{    
	float x;
	float y;
	float z;
	float yaw;
} extGameRobotPos_t;


//参赛队自定义数据(0x0100)
typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
} extShowData_t;





typedef __packed struct
{
	extGameRobotState_t 	state;				//比赛机器人信息(0x0001)
	extRobotHurt_t 			hurted;				//伤害数据(0x0002)
	extShootData_t 			shoot_data;			//实时射击信息(0x0003)
	extPowerHeatData_t		power_heat_data;	//实时功率热量数据(0x0004)
	extRfidDetect_t			rfid;				//场地交互数据(0x0005)
	extGameResult_t			winner;				//比赛胜负数据(0x0006)
	extGetBuff_t			buff;				//Buff获取数据(0x0007)
	extGameRobotPos_t		pos;				//机器人位置朝向信息(0x0008)

	
//	extShowData_t			custom_data;		//参赛队自定义数据

} Game_info;






//读取裁判系统数据
typedef struct
{
//	int last_buf_offset;
//	int buf_offset;
//	
//	int last_data_size;
//	int data_size;
//	
////	u8 last_judge_buf[ JUDGE_DMA_RXBUF_SIZE];		//上次获取的数据
//	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];			//此次获取的数据
//	u8 judge[ JUDGE_DATA_FRAME_LENGTH];				//一帧的数据
//	
//	u8 broken_frame_flag;			//断帧标志
//	u8 broken_frame_header_flag;	//帧头断帧标志
//	u8 continuous_frame_flag;		//连续帧标志
//	
//	u8 broken_offset;			//断帧位置，断帧标志设置时有效
//	
//	u8 last_Seq;		//上次包序号
//	uint32_t lost_cnt;	//丢包计数
	
	
	int data_size;
	
	uint16_t CmdID;
	
	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];			//此次获取的数据
	u8 judge[ JUDGE_DATA_FRAME_LENGTH];				//一帧的数据

	
	u8 broken_frame_flag;			//断帧标志
	u8 broken_frame_header_flag;	//帧头断帧标志
	u8 broken_offset;				//断帧位置，断帧标志设置时有效

	u8 last_Seq;		//上次包序号
	uint32_t Seq_cnt;	//总包数
	uint32_t lost_cnt;	//丢包计数
} judge_get;


u8 get_info_from_buf( u8 *temp, uint16_t command_id);
void judge_data_get(void);
void Judge_Init(void);





extern FrameHeader frame_header;
extern Game_info game_info;
//extern u8 judge_buf[ JUDGE_DMA_RXBUFF_SIZE];

extern judge_get judge_info_get;


#endif // JUDGE_DATA_H_INCLUDED
