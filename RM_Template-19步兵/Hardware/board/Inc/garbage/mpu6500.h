#ifndef MPU6500_H_INCLUDED
#define MPU6500_H_INCLUDED

#include "sys.h"

#define MPU6500_NSS_Low() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define MPU6500_NSS_High() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)

//typedef struct
//{
//  int16_t ax;
//  int16_t ay;
//  int16_t az;
//  
//  int16_t temperature;
//  
//  int16_t gx;
//  int16_t gy;
//  int16_t gz;
//  
////  int16_t mx;
////  int16_t my;
////  int16_t mz;
//} IMUDataTypedef;


typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    int16_t temp;

    int16_t gx;
    int16_t gy;
    int16_t gz;
    
    int16_t ax_offset;
    int16_t ay_offset;
    int16_t az_offset;
  
    int16_t gx_offset;
    int16_t gy_offset;
    int16_t gz_offset;
    
} mpu_data_t;

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    float temp;
    float temp_ref;
  
    float wx;
    float wy;
    float wz;

    float vx;
    float vy;
    float vz;
  
    float rol;
    float pit;
    float yaw;
} imu_data_t;

//uint8_t mpu_device_init(void);
void    mpu_get_data(void);
void    mpu_offset_cal(void);


//extern uint8_t MPU_id;
//extern IMUDataTypedef imu_data;

//extern float _Yaw,_Pitch,_Roll;


uint8_t MPU6500_Init(void);
uint8_t MPU6500_Write_Reg(uint8_t const reg, uint8_t const data);
uint8_t MPU6500_Read_Reg(uint8_t const reg);
uint8_t MPU6500_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len);
//void IMU_Get_Data(void);

//void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

//void    mpu_offset_cal(void);


#endif // MPU6500_H_INCLUDED

