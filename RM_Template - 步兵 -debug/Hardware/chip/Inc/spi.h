#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "sys.h"


void SPI5_Init(void);
u8 SPI5_ReadWriteByte(u8 TxData);			//参数为发送值，返回值为接收值



#endif // SPI_H_INCLUDED
