#include "mpu6500.h"
#include "mpu6500_reg.h"
#include "usart.h"
#include "spi.h"
#include "delay.h"

#include "math.h"

uint8_t MPU_id = 0;

IMUDataTypedef imu_data = {0,0,0,0,0,0,0};
IMUDataTypedef imu_data_offest = {0,0,0,0,0,0,0};


u8 MPU6500_Write_Reg( u8 reg, u8 value)
{
	u8 status;
	
	//SPI5_NSS = 0;
	GPIO_ResetBits(GPIOF, GPIO_Pin_6);
	status = SPI5_ReadWriteByte( reg & 0x7f);
	SPI5_ReadWriteByte(value);
	GPIO_SetBits( GPIOF, GPIO_Pin_6);
	//SPI5_NSS = 1;
	
	return status;
}


u8 MPU6500_Read_Reg( u8 reg)
{
	u8 value;
	
	//SPI5_NSS = 0;
	GPIO_ResetBits(GPIOF, GPIO_Pin_6);
	SPI5_ReadWriteByte( reg | 0x80);
	//delay_ms(5);
	value = SPI5_ReadWriteByte(0x00);
	//SPI5_NSS = 1;
	GPIO_SetBits( GPIOF, GPIO_Pin_6);
	
	return value;
}


//Read registers from MPU6500,address begin with regAddr
uint8_t MPU6500_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
	u8 t;
	for( t = 0; t < len; ++t)
	{
		pData[t] = MPU6500_Read_Reg( regAddr + t);
	}
	return 0;
}



void MPU6500_Init()
{
	uint8_t index = 0;
	uint8_t MPU6500_Init_Data[8][2] = 
	{
		{MPU6500_PWR_MGMT_1,    0x80},      // Reset Device
		{MPU6500_PWR_MGMT_1,    0x03},      // Clock Source - Gyro-Z
		{MPU6500_PWR_MGMT_2,    0x00},      // Enable Acc & Gyro
		{MPU6500_CONFIG,        0x02},      // LPF 98Hz
		{MPU6500_GYRO_CONFIG,   0x18},      // +-2000dps
		{MPU6500_ACCEL_CONFIG,  0x10},      // +-8G
		{MPU6500_ACCEL_CONFIG_2,0x02},      // enable LowPassFilter  Set Acc LPF
		{MPU6500_USER_CTRL,     0x20},      // Enable AUX
	};
  
	delay_ms(100);
//  printf("MPU6500_WHO_AM_I=0x%x\r\n", MPU6500_Read_Reg(MPU6500_WHO_AM_I));  //read id of device,check if MPU6500 or not
    MPU_id = MPU6500_Read_Reg(MPU6500_WHO_AM_I);  //read id of device,check if MPU6500 or not
	
	
	for(index = 0; index < 8; index++)
	{
		MPU6500_Write_Reg(MPU6500_Init_Data[index][0], MPU6500_Init_Data[index][1]);
	  
		delay_ms(1);
//		printf("index:%d,value=%x,set=%x\r\n",index, MPU6500_Read_Reg(MPU6500_Init_Data[index][0]), MPU6500_Init_Data[index][1]);
	}

}



//Set the accelerated velocity resolution
uint8_t MPU6500_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU6500_Write_Reg(MPU6500_ACCEL_CONFIG, fsr<<3);
}

//Set the angular velocity resolution
uint8_t MPU6500_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU6500_Write_Reg(MPU6500_GYRO_CONFIG, fsr<<3);
}


//Get 6 axis data from MPU6500
void IMU_Get_Data()
{
  uint8_t mpu_buff[14];
  MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);
  
  imu_data.ax = mpu_buff[0]<<8 |mpu_buff[1];
  imu_data.ay = mpu_buff[2]<<8 |mpu_buff[3];
  imu_data.az = mpu_buff[4]<<8 |mpu_buff[5];
  
  imu_data.temperature = mpu_buff[6]<<8 |mpu_buff[7];
  
  imu_data.gx = mpu_buff[8]<<8 |mpu_buff[9] - imu_data_offest.gx;
  imu_data.gy = mpu_buff[10]<<8 |mpu_buff[11] - imu_data_offest.gy;
  imu_data.gz = mpu_buff[12]<<8 |mpu_buff[13] - imu_data_offest.gz;
  
  
}



const float Kp =  2.0f;	// 比例增益支配收敛率accelerometer/magnetometer 
const float Ki = 0.005f;	// 积分增益执政速率陀螺仪的衔接gyroscopeases 
const float halfT = 0.5f;	// 采样周期的一半 

//--------------------------------------------------------------------------------------------------- 

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// 四元数的元素，代表估计 方向 
float exInt = 0, eyInt = 0, ezInt = 0;	// 按比例缩小积分误差

float _Yaw,_Pitch,_Roll;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
{ 
	float norm; 
	float vx, vy, vz;
	float ex, ey, ez;           
	norm = sqrt(ax*ax + ay*ay + az*az);       
	ax = ax / norm; 
	ay = ay / norm; 
	az = az / norm; 
	//estimated direction of gravity 
	vx = 2.0f * ( q1 * q3 - q0 * q2);
	vy = 2.0f * ( q0 * q1 + q2 * q3); 
	vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
	
	
	ex = (ay * vz - az * vy);// + (my*wz - mz*wy); 
	ey = (az * vx - ax * vz);// + (mz*wx - mx*wz); 
	ez = (ax * vy - ay * vx);// + (mx*wy - my*wx); 

	
	//integral error scaled integral gain 
	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki; 
	ezInt = ezInt + ez * Ki; 
	

	// adjusted gyroscope measurements 
	gx = gx + Kp*ex + exInt; 
	gy = gy + Kp*ey + eyInt; 
	gz = gz + Kp*ez + ezInt;
	//integrate quaternion rate and normalise 
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT; 
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT; 
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT; 
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT; 
	
	// 正常化四元 
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3); 
	q0 = q0 / norm; 
	q1 = q1 / norm; 
	q2 = q2 / norm; 
	q3 = q3 / norm; 
	
	_Yaw = atan2(2.0f * q1 * q2 + 2.0f * q0 * q3, -2.0f * q2*q2 - 2.0f * q3* q3 + 1.0f)* 57.3f; // yaw 
	_Pitch = asin(-2.0f * q1 * q3 + 2.0f * q0* q2)* 57.3f; // pitch 
	_Roll = atan2(2.0f * q2 * q3 + 2.0f * q0 * q1, -2.0f * q1 * q1 - 2.0f * q2* q2 + 1.0f)* 57.3f; // roll
	
	
}

