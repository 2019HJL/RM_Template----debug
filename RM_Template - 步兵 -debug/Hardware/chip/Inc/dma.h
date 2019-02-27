#ifndef DMA_H_INCLUDED
#define DMA_H_INCLUDED

#include "sys.h"
#include "DR16.h"

#define JUDGE_DMA_RXBUF_SIZE 	512
#define VISION_DMA_RXBUF_SIZE	20

void DMA_USART1_Init(void);
void DMA_USART2_Init(void);
void DMA_USART6_Init(void);


extern volatile unsigned char dbus_buf[RC_FRAME_LENGTH];
extern volatile unsigned char judge_dma_rxbuf[JUDGE_DMA_RXBUF_SIZE];
extern volatile unsigned char vision_dma_rxbuf[VISION_DMA_RXBUF_SIZE];




#endif // DMA_H_INCLUDED
