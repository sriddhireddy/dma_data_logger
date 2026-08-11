#include "adc_driver.h"

#define ADC1EN			(1U<<8)
#define GPIOAEN			(1U<<0)

//alr defined in CMSIS
//#define ADC_CR2_ADON	(1U<<0)

#define ADC_SQ1_CH0 	(0U) //as its a value not a bit. for ch1-> (1U) for ch5: (5U)

//alr defines these macros in CMSIS

//#define ADC_CR2_SWSTART		(1U<<30)
//#define ADC_SR_EOC		(1U<<1)

//#define ADC_CR2_DMA		(1U<<8)
//#define ADC_CR2_DDS		(1U<<9)
//#define ADC_CR2_CONT	(1U<<1)


void ADC_Init(void){

	RCC->APB2ENR |= ADC1EN;
	RCC->AHB1ENR |= GPIOAEN;

	//pa0 to analog mode
	GPIOA->MODER &=~(3U<<0); //clear bits 1:0
	GPIOA->MODER |= (3U<<0); //11 to bits 1:0

	GPIOA->PUPDR &=~(3U<<0); //no pull-up, pull-down

	//turn on adc
	ADC1->CR2 |= ADC_CR2_ADON;

	//choose channel 0 [only 1 conversion thus SQR1:- L [3:0] = 0000 (default)]
	ADC1->SQR3 = ADC_SQ1_CH0;

	/* enable DMA mode */
	ADC1->CR2 |= ADC_CR2_DMA;

	/* enable DMA requests to continue */
	ADC1->CR2 |= ADC_CR2_DDS;

	/* enable continuous conversion */
	ADC1->CR2 |= ADC_CR2_CONT;

}

uint16_t ADC_Read(void){

	//Start adc conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;

	//waits for eoc (check if conversion completed)
	while(!(ADC1->SR & ADC_SR_EOC)){}

	//return the value found
	return (ADC1->DR);

}



