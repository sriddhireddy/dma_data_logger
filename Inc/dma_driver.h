
#ifndef DMA_DRIVER_H_
#define DMA_DRIVER_H_

#include "stm32f4xx.h"

void dma2_stream0_init(uint32_t src, uint32_t dst, uint32_t len);
void DMA2_Stream0_IRQHandler(void);
uint8_t DMA_TransferComplete(void);

#endif /* DMA_DRIVER_H_ */
