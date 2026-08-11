#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "stm32f4xx.h"

void GPIO_Init(void);

void GPIO_SetPin(void);
void GPIO_ResetPin(void);


uint8_t GPIO_ReadButton(void);
uint8_t GPIO_ButtonPressed(void);

#endif /* GPIO_DRIVER_H_ */
