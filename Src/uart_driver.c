#include "uart_driver.h"


//use datasheet to find block diagram to figure which clock to enable.
//use referece manual to set that bits
#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_CLK     16000000U
#define BAUDRATE    115200U

#define CR1_UE			(1U<<13)
#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)

#define SR_TXE			(1U<<7)
#define SR_RXNE			(1U<<5)

void UART_Init(void){

	RCC->AHB1ENR |= GPIOAEN;
	RCC->APB1ENR |= UART2EN;

	GPIOA->MODER &=~(15U<<4); //0000 to bits 7:4
	GPIOA->MODER |= (10U<<4); //1010 to bits 7:4

	GPIOA->AFR[0] &= ~(0xFFU << 8);   // Clear bits 15:8
	GPIOA->AFR[0] |=  (0x77U << 8);   // AF7 for PA2 and PA3

	USART2->BRR = SYS_CLK/BAUDRATE; //BRR = clk freq/baudrate = 16M /115200 = 138.88 = 139

	USART2->CR1 |= (CR1_TE | CR1_RE | CR1_UE);

}

void UART_WriteChar(char c){

	while(!(USART2->SR & SR_TXE)){} //polling

	USART2->DR = c;
}

void UART_WriteString(const char *str){ //const implies the func will NOT modify the string

	while(*str){ //same as *str != '\0' as \0 equivalent to 0
		UART_WriteChar(*str);
		str++;
	}
}

void UART_WriteInt(uint32_t value){

	char digits[10];
	int count = 0;

	if(value==0){
		UART_WriteChar('0');
		return;
	}

	while(value>0){
		digits[count]= value % 10;
		count++;
		// OR: 	digits[count++]= value % 10;
		value = value / 10;
	}

	for(int i = count-1 ; i >= 0; i--){
		UART_WriteChar(digits[i] + '0');
	}

}






