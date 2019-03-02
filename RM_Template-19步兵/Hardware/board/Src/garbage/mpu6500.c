#include "mpu6500.h"
#include "mpu6500_reg.h"
#include "ist8310_reg.h"
#include "usart.h"
#include "spi.h"
#include "delay.h"
#include "imu.h"

#include <string.h>
#include "math.h"


#define ENABLE_IST   GPIO_SetBits(GPIOE, GPIO_Pin_2)


uint8_t MPU_id = 0;

//IMUDataTypedef imu_data = {0,0,0,0,0,0,0};
//IMUDataTypedef imu_data_offest = {0,0,0,0,0,0,0};
static uint8_t mpu_buff[14];

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



static void ist_reg_write_by_mpu(uint8_t addr, uint8_t data)
{
  //turn off slave 1 at first
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
  delay_ms(2);
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, addr);
  delay_ms(2);
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, data);
  delay_ms(2);
  //turn on slave 1 with one byte transmitting
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
  delay_ms(10);
}

static uint8_t ist_reg_read_by_mpu(uint8_t addr)
{
  uint8_t retval;
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_REG, addr);
  delay_ms(10);
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x80);
  delay_ms(10);
  retval = MPU6500_Read_Reg(MPU6500_I2C_SLV4_DI);
  //turn off slave4 after read
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
  delay_ms(10);
  return retval;
}

static void mpu_mst_i2c_auto_read_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num)
{
  //configure the device address of the IST8310
  //use slave1,auto transmit single measure mode.
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, device_address);
  delay_ms(2);
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, IST8310_R_CONFA);
  delay_ms(2);
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, IST8310_ODR_MODE);
  delay_ms(2);

  //use slave0,auto read data
  MPU6500_Write_Reg(MPU6500_I2C_SLV0_ADDR, 0x80 | device_address);
  delay_ms(2);
  MPU6500_Write_Reg(MPU6500_I2C_SLV0_REG, reg_base_addr);
  delay_ms(2);

  //every eight mpu6500 internal samples one i2c master read
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x03);
  delay_ms(2);
  //enable slave 0 and 1 access delay
  MPU6500_Write_Reg(MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
  delay_ms(2);
  //enable slave 1 auto transmit
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
  delay_ms(6); //Wait 6ms (minimum waiting time for 16 times internal average setup)
  //enable slave 0 with data_num bytes reading
  MPU6500_Write_Reg(MPU6500_I2C_SLV0_CTRL, 0x80 | data_num);
  delay_ms(2);
}

uint8_t ist8310_init(void)
{
  //Enable I2C master mode, Reset I2C Slave module
  MPU6500_Write_Reg(MPU6500_USER_CTRL, 0x30); 
  delay_ms(10);
  //I2C master clock 400kHz
  MPU6500_Write_Reg(MPU6500_I2C_MST_CTRL, 0x0d);
  delay_ms(10);

  //turn on slave 1 for ist write and slave 4 for ist read
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, IST8310_ADDRESS); //write ist
  delay_ms(10);
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS); //read ist
  delay_ms(10);

  //reset ist8310
  ist_reg_write_by_mpu(IST8310_R_CONFB, 0x01);
  delay_ms(10);

  if (IST8310_DEVICE_ID_A != ist_reg_read_by_mpu(IST8310_WHO_AM_I))
      return 1;

  ist_reg_write_by_mpu(IST8310_R_CONFB, 0x01);
  delay_ms(10);

  //config as ready mode to access reg
  ist_reg_write_by_mpu(IST8310_R_CONFA, 0x00); 
  if (ist_reg_read_by_mpu(IST8310_R_CONFA) != 0x00)
      return 2;
  delay_ms(10);

  //normal state, no int
  ist_reg_write_by_mpu(IST8310_R_CONFB, 0x00); 
  if (ist_reg_read_by_mpu(IST8310_R_CONFB) != 0x00)
      return 3;
  delay_ms(10);

  //config  low noise mode, x,y,z axis 16 time 1 avg,
  ist_reg_write_by_mpu(IST8310_AVGCNTL, 0x24); //100100
  if (ist_reg_read_by_mpu(IST8310_AVGCNTL) != 0x24)
      return 4;
  delay_ms(10);

  //Set/Reset pulse duration setup, normal mode
  ist_reg_write_by_mpu(IST8310_PDCNTL, 0xc0);
  if (ist_reg_read_by_mpu(IST8310_PDCNTL) != 0xc0)
      return 5;
  delay_ms(10);

  //turn off slave1 & slave 4
  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
  delay_ms(10);
  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
  delay_ms(10);

  //configure and turn on slave 0
  mpu_mst_i2c_auto_read_config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
  delay_ms(100);
  return 0;
}

void ist8310_get_data(uint8_t* buff)
{
  MPU6500_Read_Regs(MPU6500_EXT_SENS_DATA_00, buff, 6);
}


uint8_t MPU6500_Init()
{

	
	uint8_t index = 0;
	uint8_t MPU6500_Init_Data[8][2] = 
	{
		//{MPU6500_PWR_MGMT_1,    0x80},      // Reset Device
		{MPU6500_PWR_MGMT_1,    0x03},      // Clock Source - Gyro-Z
		{MPU6500_PWR_MGMT_2,    0x00},      // Enable Acc & Gyro
		{MPU6500_CONFIG,        0x04},      // LPF 98Hz
		{MPU6500_GYRO_CONFIG,   0x18},      // +-2000dps
		{MPU6500_ACCEL_CONFIG,  0x10},      // +-8G
		{MPU6500_ACCEL_CONFIG_2,0x04},      // enable LowPassFilter  Set Acc LPF
		{MPU6500_USER_CTRL,     0x20},      // Enable AUX
	};
	//ENABLE_IST;  
//	delay_ms(100);
//  printf("MPU6500_WHO_AM_I=0x%x\r\n", MPU6500_Read_Reg(MPU6500_WHO_AM_I));  //read id of device,check if MPU6500 or not
      //read id of device,check if MPU6500 or not


	MPU6500_Write_Reg(MPU6500_PWR_MGMT_1, 0x80);
	delay_ms(100);
	MPU6500_Write_Reg(MPU6500_SIGNAL_PATH_RESET, 0x07);
	delay_ms(1000);
	MPU6500_Write_Reg(MPU6500_PWR_MGMT_1, 0x0);
	delay_ms(100);		
	if(MPU6500_Read_Reg(MPU6500_WHO_AM_I) != MPU6500_WHO_AM_I)
	{
		printf("mpu_id=0x%x\r\n",MPU6500_Read_Reg(MPU6500_WHO_AM_I));	
		//return 1;
	}
	
	for(index = 0; index < 7; index++)
	{
		MPU6500_Write_Reg(MPU6500_Init_Data[index][0], MPU6500_Init_Data[index][1]);
	  
		delay_ms(1);
		printf("index:%d,value=%x,set=%x\r\n",index, MPU6500_Read_Reg(MPU6500_Init_Data[index][0]), MPU6500_Init_Data[index][1]);
	}
	
//	ist8310_init();
	mpu_offset_cal();
	
	return 0;
}

void mpu_offset_cal(void)
{
	int i;
	for (i = 0; i < 300; i++)
	{
		MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);

		mpu_data.ax_offset += mpu_buff[0] << 8 | mpu_buff[1];
		mpu_data.ay_offset += mpu_buff[2] << 8 | mpu_buff[3];
		mpu_data.az_offset += mpu_buff[4] << 8 | mpu_buff[5];

		mpu_data.gx_offset += mpu_buff[8] << 8 | mpu_buff[9];
		mpu_data.gy_offset += mpu_buff[10] << 8 | mpu_buff[11];
		mpu_data.gz_offset += mpu_buff[12] << 8 | mpu_buff[13];

		delay_ms(5);
	}
	mpu_data.ax_offset=mpu_data.ax_offset / 300;
	mpu_data.ay_offset=mpu_data.ay_offset / 300;
	mpu_data.az_offset=mpu_data.az_offset / 300;
	mpu_data.gx_offset=mpu_data.gx_offset / 300;
	mpu_data.gy_offset=mpu_data.gx_offset / 300;
	mpu_data.gz_offset=mpu_data.gz_offset / 300;
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
void mpu_get_data()
{
  MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);

  mpu_data.ax   = mpu_buff[0] << 8 | mpu_buff[1];
  mpu_data.ay   = mpu_buff[2] << 8 | mpu_buff[3];
  mpu_data.az   = mpu_buff[4] << 8 | mpu_buff[5];
  mpu_data.temp = mpu_buff[6] << 8 | mpu_buff[7];

  mpu_data.gx = ((mpu_buff[8] << 8 | mpu_buff[9])   - mpu_data.gx_offset);
  mpu_data.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - mpu_data.gy_offset);
  mpu_data.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - mpu_data.gz_offset);

  //ist8310_get_data((uint8_t*)&mpu_data.mx);

  memcpy(&imu.ax, &mpu_data.ax, 6 * sizeof(int16_t));
  imu.temp = 21 + mpu_data.temp / 333.87f;
  imu.wx   = mpu_data.gx / 16.384f / 57.3f; //2000dps -> rad/s
  imu.wy   = mpu_data.gy / 16.384f / 57.3f; //2000dps -> rad/s
  imu.wz   = mpu_data.gz / 16.384f / 57.3f; //2000dps -> rad/s
  
  
}



//const float Kp =  2.0f;	// 比例增益支配收敛率accelerometer/magnetometer 
//const float Ki = 0.005f;	// 积分增益执政速率陀螺仪的衔接gyroscopeases 
//const float halfT = 0.5f;	// 采样周期的一半 

////--------------------------------------------------------------------------------------------------- 

//float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// 四元数的元素，代表估计 方向 
//float exInt = 0, eyInt = 0, ezInt = 0;	// 按比例缩小积分误差

//float _Yaw,_Pitch,_Roll;

//void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
//{ 
//	float norm; 
//	float vx, vy, vz;
//	float ex, ey, ez;           
//	norm = sqrt(ax*ax + ay*ay + az*az);       
//	ax = ax / norm; 
//	ay = ay / norm; 
//	az = az / norm; 
//	//estimated direction of gravity 
//	vx = 2.0f * ( q1 * q3 - q0 * q2);
//	vy = 2.0f * ( q0 * q1 + q2 * q3); 
//	vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
//	
//	
//	ex = (ay * vz - az * vy);// + (my*wz - mz*wy); 
//	ey = (az * vx - ax * vz);// + (mz*wx - mx*wz); 
//	ez = (ax * vy - ay * vx);// + (mx*wy - my*wx); 

//	
//	//integral error scaled integral gain 
//	exInt = exInt + ex * Ki;
//	eyInt = eyInt + ey * Ki; 
//	ezInt = ezInt + ez * Ki; 
//	

//	// adjusted gyroscope measurements 
//	gx = gx + Kp*ex + exInt; 
//	gy = gy + Kp*ey + eyInt; 
//	gz = gz + Kp*ez + ezInt;
//	//integrate quaternion rate and normalise 
//	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT; 
//	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT; 
//	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT; 
//	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT; 
//	
//	// 正常化四元 
//	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3); 
//	q0 = q0 / norm; 
//	q1 = q1 / norm; 
//	q2 = q2 / norm; 
//	q3 = q3 / norm; 
//	
//	_Yaw = atan2(2.0f * q1 * q2 + 2.0f * q0 * q3, -2.0f * q2*q2 - 2.0f * q3* q3 + 1.0f)* 57.3f; // yaw 
//	_Pitch = asin(-2.0f * q1 * q3 + 2.0f * q0* q2)* 57.3f; // pitch 
//	_Roll = atan2(2.0f * q2 * q3 + 2.0f * q0 * q1, -2.0f * q1 * q1 - 2.0f * q2* q2 + 1.0f)* 57.3f; // roll
//	
//	
//}

