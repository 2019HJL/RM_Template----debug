#include "mpu6050_i2c.h"
#include "mpu6050_driver.h"
#include "system_reset.h"

#include <math.h>

volatile MPU6050_RAW_DATA    MPU6050_Raw_Data;    //原始数据
volatile MPU6050_REAL_DATA   MPU6050_Real_Data;
uint8_t mpu_buf[20]={0};       //save the data of acc gyro & mag using iic
int16_t MPU6050_FIFO[6][11] = {0};//[0]-[9]为最近10次数据 [10]为10次数据的平均值

int MPU6050_Init(void);


//MPU6050 初始化，成功返回0  失败返回 0xff
int MPU6050_Init(void)
{
    unsigned char temp_data = 0x00;

    IIC_GPIO_Init();  //初始化IIC接口
//    HEAT_Configuration();
    
    if(IIC_ReadData(MPU6050_DEVICE_ADDRESS,WHO_AM_I,&temp_data,1)==0) //确定IIC总线上挂接的是否是MPU6050
    {
        if(temp_data != MPU6050_ID)
        {
            printf("error 1A\r\n");
            return 0xff; //校验失败，返回0xff
        }
    }
    else
    {
        printf("error 1B\r\n");
        return 0xff; //读取失败 返回0xff
    }
    
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,PWR_MGMT_1,0x80) == 0xff)    //复位
    {
        printf("error 1C1\r\n");
        return 0xff;
    }
	delay_ms(10);
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,PWR_MGMT_1,0x00) == 0xff)    //唤醒
    {
        printf("error 1C2\r\n");
        return 0xff;
    }
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,PWR_MGMT_1,0x01) == 0xff)    //设置CLKSEL,PLL X 轴为参考
    {
        printf("error 1C3\r\n");
        return 0xff;
    }
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,MPU6050_RA_PWR_MGMT_2,0x00) == 0xff)    //加速度陀螺仪都工作
    {
        printf("error 1C4\r\n");
        return 0xff;
    }

    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,CONFIG,0x03) == 0xff)         //Digital Low-Pass Filter:DLPF_CFG is 3, Fs is 1khz 
    {                                                                     //acc bandwidth 44Hz,gyro 42Hz
        printf("error 1E\r\n");
        return 0xff;
    }
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,GYRO_CONFIG,0x10) == 0xff)    //FS_SEL 3 : gyroscope full scale range is +-1000degs/s 
    {
        printf("error 1F\r\n");
        return 0xff;
    }
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,ACCEL_CONFIG,0x00) == 0xff)   //AFS_SEL 1: accelerometer full scale range is +-2g
    {
        printf("error 1G\r\n");
        return 0xff;
    }

    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,INT_ENABLE,0x00) == 0xff)      //disable data ready interrupt
    {
        printf("error 1I\r\n");
        return 0xff;
    }
		
	//设置mpu6050 IIC masters mode  disabled 不让mpu6050控制aux IIC接口
	if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,MPU6050_RA_USER_CTRL,0x00) == 0xff)      //disable data ready interrupt
    {
        printf("error 1I\r\n");
        return 0xff;
    }
		
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,MPU6050_RA_FIFO_EN,0x00) == 0xff)    //关闭FIFO
    {
        printf("error 1H\r\n");
        return 0xff;
    }		
    if(IIC_WriteData(MPU6050_DEVICE_ADDRESS,MPU6050_RA_INT_PIN_CFG,0x80) == 0xff)    //INT引脚低电平有效
    {
        return 0xff;
    }	
		
    delay_ms(500);
    return 0;
}



void MPU6050_Initialize(void)
{
    while(MPU6050_Init() == 0xff) 
    {                       
		delay_ms(200);  
		System_Reset();
    }
}

//MPU6050  数据读取，成功返回0  失败返回 0xff



int MPU6050_ReadData(uint8_t Slave_Addr, uint8_t Reg_Addr, uint8_t * Data, uint8_t Num)
{    
	//IIC_ReadData(MPU6050_DEVICE_ADDRESS,MPU6050_DATA_START,buf,14)
    if(IIC_ReadData(Slave_Addr,Reg_Addr,Data,Num) == 0xff)
    {
        printf("error 1J\r\n");
        return 0xff;
    }
   
    return 0;
}

/**********************************************************************************/
/*将MPU6050_ax,MPU6050_ay, MPU6050_az,MPU6050_gx, MPU6050_gy, MPU6050_gz处理后存储*/
/**********************************************************************************/
void MPU6050_DataSave(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz) //[0]-[9]为最近10次数据 [10]为10次数据的平均值
{
	uint8_t i = 0;
	int32_t sum=0;
	
	for(i=1;i<10;i++)
	{
		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	
	MPU6050_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
	MPU6050_FIFO[1][9]=ay;
	MPU6050_FIFO[2][9]=az;
	MPU6050_FIFO[3][9]=gx;
	MPU6050_FIFO[4][9]=gy;
	MPU6050_FIFO[5][9]=gz;
	
	for(i=0;i<10;i++)//求当前数组的合，再取平均值
	{	
		 sum+=MPU6050_FIFO[0][i];
	}
	MPU6050_FIFO[0][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[1][i];
	}
	MPU6050_FIFO[1][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[2][i];
	}
	MPU6050_FIFO[2][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[3][i];
	}
	MPU6050_FIFO[3][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[4][i];
	}
	MPU6050_FIFO[4][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++){
		 sum+=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[5][10]=sum/10;
	
}


int16_t MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz,MPU6050_Lastgx,MPU6050_Lastgy,MPU6050_Lastgz;
/**************************实现函数********************************************
*函数原型:		void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
*功　　能:	    读取 MPU6050的当前zz测量值
*******************************************************************************/
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) 
{
	MPU6050_ReadData(MPU6050_DEVICE_ADDRESS,MPU6050_DATA_START,mpu_buf,14);  //
//		HMC58X3_ReadData(&(mpu_buf[14]));  //14-19为陀螺仪数据
	MPU6050_Lastax=(((int16_t)mpu_buf[0]) << 8) | mpu_buf[1];
	MPU6050_Lastay=(((int16_t)mpu_buf[2]) << 8) | mpu_buf[3];
	MPU6050_Lastaz=(((int16_t)mpu_buf[4]) << 8) | mpu_buf[5];
	//跳过温度ADC
	MPU6050_Lastgx=(((int16_t)mpu_buf[8]) << 8) | mpu_buf[9];
	MPU6050_Lastgy=(((int16_t)mpu_buf[10]) << 8) | mpu_buf[11];
	MPU6050_Lastgz=(((int16_t)mpu_buf[12]) << 8) | mpu_buf[13];
		
	MPU6050_DataSave(MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz,MPU6050_Lastgx,MPU6050_Lastgy,MPU6050_Lastgz);  		
	*ax = MPU6050_FIFO[0][10];
	*ay = MPU6050_FIFO[1][10];
	*az = MPU6050_FIFO[2][10];
	*gx = MPU6050_FIFO[3][10] ;//- GyroSavedCaliData.GyroXOffset;
	*gy = MPU6050_FIFO[4][10] ;//- GyroSavedCaliData.GyroYOffset;
	*gz = MPU6050_FIFO[5][10] ;//- GyroSavedCaliData.GyroZOffset;

}

void MPU6050_getlastMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
	*ax = MPU6050_FIFO[0][10];
	*ay = MPU6050_FIFO[1][10];
	*az = MPU6050_FIFO[2][10];
	*gx = MPU6050_FIFO[3][10];//-GyroSavedCaliData.GyroXOffset;
	*gy = MPU6050_FIFO[4][10];//-GyroSavedCaliData.GyroYOffset;
	*gz = MPU6050_FIFO[5][10];//-GyroSavedCaliData.GyroZOffset;
}


void IMU_getValues(volatile float * values) 
{  
	int16_t accgyroval[6];
	int i;
	//读取加速度和陀螺仪的当前ADC
	MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);
	MPU6050_Raw_Data.Accel_X = accgyroval[0];
	MPU6050_Raw_Data.Accel_Y = accgyroval[1];
	MPU6050_Raw_Data.Accel_Z = accgyroval[2];
	MPU6050_Raw_Data.Gyro_X  = accgyroval[3];
	MPU6050_Raw_Data.Gyro_Y  = accgyroval[4];
	MPU6050_Raw_Data.Gyro_Z  = accgyroval[5];
	
	
    for(i = 0; i<6; i++) 
	{
		if(i < 3) 
		{
			values[i] = (float) accgyroval[i];
		}
		else 
		{
			values[i] = ((float) accgyroval[i]) / 32.8f; //转成度每秒
			//这里已经将量程改成了 1000度每秒  32.8 对应 1度每秒
		}
    }
}



volatile int Yaw,Pitch,Roll;  							//偏航角，俯仰角，翻滚角





