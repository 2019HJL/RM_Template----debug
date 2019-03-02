#ifndef MPU6050_I2C_H_INCLUDED
#define MPU6050_I2C_H_INCLUDED


#include "sys.h"
#include "delay.h"

#include "mpu6050_i2c.h"
#include "gpio.h"
#include "usart.h"

///*----I2C1----SCL----PB6---*/
///*----I2C1----SDA----PB7---*/

//#define IIC_SCL_H()      GPIO_SetBits(GPIOB,GPIO_Pin_8)
//#define IIC_SCL_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_8)
//#define IIC_SDA_H()      GPIO_SetBits(GPIOB,GPIO_Pin_9)
//#define IIC_SDA_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_9)
//#define IIC_SDA_Read()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)


void IIC_Delay(unsigned int t);
void HEAT_Configuration(void);
void IIC_GPIO_Init(void);
void IIC_SDA_Out(void);
void IIC_SDA_In(void);
void IIC_Start(void);							  
void IIC_Stop(void);
void IIC_Ack(u8 re);					     
int IIC_WaitAck(void);
void IIC_WriteBit(u8 Temp);
u8 IIC_ReadBit(void);
//写数据，成功返回0，失败返回0xff
int IIC_WriteData(u8 dev_addr,u8 reg_addr,u8 data);
//读数据，成功返回0，失败返回0xff
int IIC_ReadData(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count);




#endif

