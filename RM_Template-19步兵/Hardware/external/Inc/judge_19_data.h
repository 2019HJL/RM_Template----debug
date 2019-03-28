#ifndef JUDGE_19_DATA_H_INCLUDED
#define JUDGE_19_DATA_H_INCLUDED

#include "sys.h"
#include "dma.h"


//命令ID
enum Command_ID { GAME_STATE = 0x0001, GAME_RESULT, GAME_ROBOT_SURVIVORS, EVENT_DATA = 0x0101, SUPPLY_PROJECTILE_ACTION, SUPPLY_PROJECTILE_BOOKING, GAME_ROBOT_STATE = 0x201, POWER_HEAT_DATA, GAME_ROBOT_POS, BUFF_MASK, ROBOT_ENERGY, ROBOT_HURT, SHOOT_DATA, INTERACTIVE_DATA};
//机器人ID
enum Robot_ID { RED_HERO = 1, RED_ENGINEERING, RED_INFANTRY_1, RED_INFANTRY_2, RED_INFANTRY_3, RED_AIR, RED_SENTRY,
				BLUE_HERO = 11, BLUE_ENGINEERING, BLUE_INFANTRY_1, BLUE_INFANTRY_2, BLUE_INFANTRY_3, BLUE_AIR, BLUE_SENTRY};
//客户端ID
enum Client_ID { CLIENT_RED_HERO = 0x0101, CLIENT_RED_ENGINEERING, CLIENT_RED_INFANTRY_1, CLIENT_RED_INFANTRY_2, CLIENT_RED_INFANTRY_3, CLIENT_RED_AIR, CLIENT_RED_SENTRY,
				CLIENT_BLUE_HERO = 0x0111, CLIENT_BLUE_ENGINEERING, CLIENT_BLUE_INFANTRY_1, CLIENT_BLUE_INFANTRY_2, CLIENT_BLUE_INFANTRY_3, CLIENT_BLUE_AIR, CLIENT_BLUE_SENTRY};
//机器人等级
enum Robot_Level { ROBOT_LEVEL_1 = 1, ROBOT_LEVEL_2, ROBOT_LEVEL_3};
//比赛类型
enum Game_Type { RM_COMBAT, RM_INDIVIDUAL_EVENT, RM_ICRA};
//比赛阶段
enum Game_Progress { NOT_START, READY, SELF_CHECKING, COUNT_DOWN_5S, BATTLING, GAME_SETTLING};
//比赛结果
enum Game_Result { GAME_TIE, WINNER_RED, WINNER_BLUE};
//增益类型
enum BUFF_Type { BLOOD_SUPPYING, MUZZLE_COOLING, DEFENSE_UP, ATTACK_UP};
//伤害类型
enum Hurt_Type { ARMOR_HURTED, MODULE_OFFLINE,SHOOTER_OVERHEAT, CHASSIS_OVERPOWER};		//血量变化类型
//装甲类型
enum Armor_Type { ARMOR_FRONT, ARMOR_LEFT, ARMOR_BEHIND, ARMOR_RIGHT, ARMOR_UP};		//装甲ID(除哨兵)
//子弹类型
enum Bullet_Type { SHELL_17MM = 1, SHELL_42MM};


#define BUFF_GET(X) ( 1 << X)		//X为BUFF_Type的内容



#define JUDGE_DATA_FRAME_LENGTH 512

#define CRC16_LEN 	2
#define CRC8_LEN	1
#define CMD_LEN		2


//帧头
typedef __packed struct
{
	u8 SOF;					//数据帧起始字节，固定值为0xA5 
	u16 DataLength;			//数据帧内 Data 长度 
	u8 Seq;					//包序号 
	u8 CRC8;				//帧头 CRC8 校验
} FrameHeader;		





//比赛状态数据(0x0001)
typedef __packed struct 
{   
	uint8_t game_type : 4;   		//比赛类型
	uint8_t game_progress : 4;   	//当前比赛阶段
	uint16_t stage_remain_time; 	//当前阶段剩余时间
} ext_game_state_t; 



//比赛结果数据(0x0002)
typedef __packed struct
{
	uint8_t winner; 				//0平局，1红方胜利，2蓝方胜利
} ext_game_result_t; 



//机器人存活数据(0x0003)  发送频率：1Hz 
typedef __packed struct 
{
	uint16_t robot_legion; 
} ext_game_robot_survivors_t; 


//场地事件数据(0x0101)  发送频率：事件改变后发送 
typedef __packed struct
{
	uint32_t event_type; 
} ext_event_data_t; 




//补给站动作标识(0x0102)  发送频率：动作改变后发送 
typedef __packed struct
{
	uint8_t supply_projectile_id;			//补给站ID
	uint8_t supply_robot_id;   				//预约机器人ID
	uint8_t supply_projectile_step;  		//子弹口开闭状态
} ext_supply_projectile_action_t; 




//补给站预约子弹(0x0103)  发送频率：上限 10Hz。RM 对抗赛尚未开放 
typedef __packed struct
{
	uint8_t supply_projectile_id;			//预约补给站口ID
	uint8_t supply_num;  					//预约子弹数目
} ext_supply_projectile_booking_t;



//比赛机器人状态(0x0201)  发送频率：10Hz 
typedef __packed struct
{   
	uint8_t robot_id; 								//机器人ID
	uint8_t robot_level; 							//机器人等级
	uint16_t remain_HP;  							//机器人剩余血量
	uint16_t max_HP;  								//机器人满血量
	uint16_t shooter_heat0_cooling_rate;  			//机器人17mm子弹热量冷却速度 单位/s
	uint16_t shooter_heat0_cooling_limit; 			//机器人17mm子弹热量上限
	uint16_t shooter_heat1_cooling_rate; 			//机器人42mm子弹热量冷却速度 单位/s
	uint16_t shooter_heat1_cooling_limit;  			//机器人42mm子弹热量上限
	uint8_t mains_power_gimbal_output : 1;  		//gimbal口输出： 1 为有24V输出，0 为无24v输出；
	uint8_t mains_power_chassis_output : 1;  		//chassis口输出：1 为有24V输出，0 为无24v输出；
	uint8_t mains_power_shooter_output : 1; 		//shooter口输出：1 为有24V输出，0 为无24v输出；
} ext_game_robot_state_t;


//实时功率热量数据(0x0202)  发送频率：50Hz 
typedef __packed struct 
{    
	uint16_t chassis_volt;				//底盘输出电压 单位：毫伏
	uint16_t chassis_current;  			//底盘输出电流 单位：毫安
	float chassis_power;    			//底盘输出功率 单位：瓦
	uint16_t chassis_power_buffer;  	//底盘功率缓冲 单位：焦耳
	uint16_t shooter_heat0;    			//17mm枪口热量
	uint16_t shooter_heat1; 			//42mm枪口热量
} ext_power_heat_data_t; 


//机器人位置(0x0203)  发送频率：10Hz 
typedef __packed struct
{
	float x;  							//位置x坐标，单位：m
	float y;  							//位置y坐标，单位：m
	float z;  							//位置z坐标，单位：m
	float yaw; 							//枪口位置， 单位：度
} ext_game_robot_pos_t;




//机器人增益(0x0204)  发送频率：状态改变后发送 
typedef __packed struct 
{   
	uint8_t power_rune_buff; 
}ext_buff_mask_t; 



//空中机器人能量状态(0x0205)  发送频率：10Hz 
typedef __packed struct 
{   
	uint8_t energy_point;				//积蓄的能量点
	uint8_t attack_time; 				//可攻击时间，单位：s（50s递减至0）
} aerial_robot_energy_t; 



//伤害状态(0x0206)  发送频率：伤害发生后发送 
typedef __packed struct
{   
	uint8_t armor_id : 4; 				//当血量变化类型为装甲伤害，代表装甲ID;  其他血量变化类型，该变量数值为 0。
	uint8_t hurt_type : 4; 				//血量变化类型 
} ext_robot_hurt_t; 



//实时射击信息(0x0207)  发送频率：射击后发送 
typedef __packed struct
{ 
	uint8_t bullet_type; 				//子弹类型   
	uint8_t bullet_freq; 				//子弹射频，单位：Hz 
	float bullet_speed; 				//子弹射速，单位：m/s
} ext_shoot_data_t; 



//交互数据接收信息（头）：0x0301。发送频率：10Hz 
typedef __packed struct 
{   
	uint16_t data_cmd_id;				//数据段的内容 ID
	uint16_t send_ID;  					//发送者的 ID 
	uint16_t receiver_ID; 				//接收者的 ID 
	///。。。内容数据段 ，最大长度为 113 
} ext_student_interactive_header_data_t; 



//客户端自定义数据：cmd_id:0x0301。内容 ID:0xD180
typedef __packed struct
{ 
	float data1; 
	float data2; 
	float data3; 
	uint8_t masks; 
} client_custom_data_t; 


//学生机器人间通信 cmd_id 0x0301，内容 ID:0x0201~0x02FF,发送频率：上限 10Hz，最大长度113 
typedef __packed struct 
{
	uint8_t data[113]; 
} robot_interactive_data_t;



typedef __packed struct
{
	ext_game_state_t						game_state;							//比赛状态数据		(0x0001)
	ext_game_result_t						result;								//比赛结果数据		(0x0002)
	ext_game_robot_survivors_t				robot_survivors;					//机器人存活数据	(0x0003)  发送频率：1Hz
	ext_event_data_t						event_data;							//场地事件数据		(0x0101)  发送频率：事件改变后发送 
	ext_supply_projectile_action_t			supply_projectile_action;			//补给站动作标识	(0x0102)  发送频率：动作改变后发送 
	ext_supply_projectile_booking_t			supply_projectile_booking;			//补给站预约子弹	(0x0103)  发送频率：上限 10Hz。RM 对抗赛尚未开放 
	ext_game_robot_state_t					game_robot_state;					//比赛机器人状态	(0x0201)  发送频率：10Hz 
	ext_power_heat_data_t					power_heat_data;					//实时功率热量数据	(0x0202)  发送频率：50Hz 
	ext_game_robot_pos_t					game_robot_pos;						//机器人位置		(0x0203)  发送频率：10Hz
	ext_buff_mask_t							buff_mask;							//机器人增益		(0x0204)  发送频率：状态改变后发送 
	aerial_robot_energy_t					aerial_robot_energy;				//空中机器人能量状态(0x0205)  发送频率：10Hz
	ext_robot_hurt_t						robot_hurt;							//伤害状态			(0x0206)  发送频率：伤害发生后发送 
	ext_shoot_data_t						shoot_data;							//实时射击信息		(0x0207)  发送频率：射击后发送 


	
	ext_student_interactive_header_data_t	custom_data;						//参赛队自定义数据
	client_custom_data_t					custom_data_client;					//客户端自定义数据
	robot_interactive_data_t				custom_data_robot;					//己方机器人间通行
} Game_info;






//读取裁判系统数据
typedef struct
{
	int data_size;
	
	uint16_t CmdID;
	
	u8 judge_buf[ JUDGE_DMA_RXBUF_SIZE];		//此次获取的数据
	u8 judge[ JUDGE_DATA_FRAME_LENGTH];			//一帧的数据

	
	u8 broken_frame_flag;						//断帧标志
	u8 broken_frame_header_flag;				//帧头断帧标志
	u8 broken_offset;							//断帧位置，断帧标志设置时有效

	u8 last_Seq;								//上次包序号
	uint32_t Seq_cnt;							//总包数
	uint32_t lost_cnt;							//丢包计数
} judge_get;


u8 get_info_from_buf( u8 *temp, uint16_t command_id);					//从缓冲区录入数据
void judge_data_get(void);												//接收并解析裁判系统数据
void Judge_Init(void);													//裁判系统初始化

void judge_data_send(uint16_t data_cmd_id, uint16_t receiver_id);		//裁判系统数据发送




extern FrameHeader frame_header;
extern Game_info game_info;
//extern u8 judge_buf[ JUDGE_DMA_RXBUFF_SIZE];

extern judge_get judge_info_get;


#endif // JUDGE_DATA_H_INCLUDED
