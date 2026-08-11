
#ifndef SYSTICK_DRIVER_H_
#define SYSTICK_DRIVER_H_

#include "stm32f4xx.h"

void SysTick_Init(void);
void SysTick_DelayMs(uint32_t ms);
void SysTick_Handler(void);
uint32_t SysTick_Millis(void);

#endif /* SYSTICK_DRIVER_H_ */
