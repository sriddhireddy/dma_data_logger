#include "systick_driver.h"

/*typedef struct
{
    __IOM uint32_t CTRL; //CSR
    __IOM uint32_t LOAD; //RVR
    __IOM uint32_t VAL; //CVR
    __IM  uint32_t CALIB; //CALIB
} SysTick_Type;*/

#define SYS_CLK 				16000000U

//#define CTRL_COUNTFLAG			(1U<<16)
#define CTRL_ENABLE				(1U<<0)
#define CTRL_TICKINT			(1U<<1)
#define CTRL_CLKSOURCE			(1U<<2)

static volatile uint32_t tickCount = 0;

void SysTick_Init(void){

	SysTick->LOAD = (SYS_CLK / 1000) - 1;

	SysTick->VAL = 0;

	SysTick->CTRL = (CTRL_ENABLE | CTRL_TICKINT | CTRL_CLKSOURCE);
}

void SysTick_DelayMs(uint32_t ms){

//	for(uint32_t i = 0; i< ms; i++){
//
//		SysTick->VAL = 0;
//		while(!(SysTick->CTRL & CTRL_COUNTFLAG));
//	}

	uint32_t start = SysTick_Millis();
	while((SysTick_Millis()- start)<ms){}
}

void SysTick_Handler(void){
	tickCount++;
}

uint32_t SysTick_Millis(void){
	return tickCount;
}
