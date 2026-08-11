
#ifndef DMA_DRIVER_H_
#define DMA_DRIVER_H_

#include "stm32f4xx.h"

void DMA2_Stream0_Init(uint32_t src, uint32_t dst0, uint32_t dst1, uint32_t len);
void DMA2_Stream0_IRQHandler(void);
uint8_t DMA_TransferComplete(void);
uint8_t DMA_HalfTransferComplete(void);

uint8_t DMA_TransferError(void);
uint8_t DMA_DirectError(void);
uint8_t DMA_FifoError(void);

void DMA2_Stream0_Stop(void);
void DMA2_Stream0_Start(void);

uint8_t DMA_FifoStatus(void);
uint8_t DMA_GetCompletedBuffer();
uint8_t DMA_GetCurrentBuffer();

#endif /* DMA_DRIVER_H_ */
