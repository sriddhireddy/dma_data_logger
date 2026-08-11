#include "gpio_driver.h"
#include "uart_driver.h"
#include "systick_driver.h"
#include "adc_driver.h"
#include "dma_driver.h"

volatile uint16_t adc_buffer[8];

int main(void)
{
    GPIO_Init();
    UART_Init();
    SysTick_Init();
    ADC_Init();

    DMA2_Stream0_Init((uint32_t)&ADC1->DR,(uint32_t)adc_buffer,8);
    DMA2_Stream0_Start();

    ADC1->CR2 |= ADC_CR2_SWSTART;



    while(1)
    {
    	if(DMA_HalfTransferComplete())
    	{
    	    UART_WriteString("First half: ");
    	    uint16_t sum1 = 0;

    	    for(int i = 0; i < 4; i++)
    	    {
    	        UART_WriteInt(adc_buffer[i]);
    	        UART_WriteString(" ");
    	    	sum1 += adc_buffer[i];
    	    }
    	    UART_WriteString("\r\n");

    	    UART_WriteString("average1:");
    	    UART_WriteInt(sum1/4);
    	    UART_WriteString("\r\n");
    	}

    	if(DMA_TransferComplete())
    	{
    	    UART_WriteString("Second half: ");
    	    uint16_t sum2 = 0;

    	    for(int i = 4; i < 8; i++)
    	    {
    	        UART_WriteInt(adc_buffer[i]);
    	        UART_WriteString(" ");
    	        sum2 += adc_buffer[i];
    	    }
    	    UART_WriteString("\r\n");

    	    UART_WriteString("average2:");
    	    UART_WriteInt(sum2/4);
    	    UART_WriteString("\r\n");
    	}
    	if(DMA_TransferError())
		{
			UART_WriteString("Transfer Error Occurred\r\n");
		}
    	if(DMA_DirectError())
		{
			UART_WriteString("Direct Error Occurred\r\n");
		}
    	if(DMA_FifoError())
		{
			UART_WriteString("Fifo Error Occurred\r\n");
		}


    	uint8_t fifo_status = DMA_FifoStatus();

    	UART_WriteString("FIFO Status: ");
    	UART_WriteInt(fifo_status);
    	UART_WriteString("\r\n");
    	SysTick_DelayMs(500);


    }
}

// ADC TEST
//        uint16_t value = ADC_Read();
//
//        UART_WriteString("ADC: ");
//        UART_WriteInt(value);
//        UART_WriteString("\r\n");
//
//        SysTick_DelayMs(500);

// DMA TEST (without interrupt part)
//        UART_WriteString("Samples:\r\n");
//
//        for(int i = 0; i < 8; i++)
//        {
//            UART_WriteInt(adc_buffer[i]);
//            UART_WriteString("\r\n");
//        }
//
//        UART_WriteString("\r\n");
//
//        SysTick_DelayMs(1000);

// DMA TEST (with interrupt part)
//        if(DMA_TransferComplete())
//        {
//            UART_WriteString("DMA COMPLETE\r\n");
//
//            for(int i = 0; i < 8; i++)
//            {
//                UART_WriteInt(adc_buffer[i]);
//                UART_WriteString("\r\n");
//            }
//
//            UART_WriteString("\r\n");
//        }

