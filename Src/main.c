#include "gpio_driver.h"
#include "uart_driver.h"
#include "systick_driver.h"
#include "adc_driver.h"
#include "dma_driver.h"

volatile uint16_t adc_buffer0[8];
volatile uint16_t adc_buffer1[8];

#define DMA_CR_CT    (1U << 19)

int main(void)
{
    GPIO_Init();
    UART_Init();
    SysTick_Init();
    ADC_Init();

    DMA2_Stream0_Init((uint32_t)&ADC1->DR,(uint32_t)adc_buffer0,(uint32_t)adc_buffer1,8);
    DMA2_Stream0_Start();

    ADC1->CR2 |= ADC_CR2_SWSTART;

    while(1)
    {
    	if(DMA_HalfTransferComplete())
    	{
    		uint8_t current_target = (DMA2_Stream0->CR & DMA_CR_CT) ? 1 : 0;
    		uint16_t sum = 0;

    		if(current_target == 0)
			{
				UART_WriteString("Buffer 0 - First half: ");

				for(int i = 0; i < 4; i++)
				{
					UART_WriteInt(adc_buffer0[i]);
					UART_WriteString(" ");

					sum += adc_buffer0[i];
				}
			}
			else
			{
				UART_WriteString("Buffer 1 - First half: ");

				for(int i = 0; i < 4; i++)
				{
					UART_WriteInt(adc_buffer1[i]);
					UART_WriteString(" ");

					sum += adc_buffer1[i];
				}
			}
    		UART_WriteString("\r\n");

			UART_WriteString("Average: ");
			UART_WriteInt(sum / 4);
			UART_WriteString("\r\n");
    	}

    	if(DMA_TransferComplete())
    	{
    		/*
			 * At TC, DMA has already switched buffers.
			 *
			 * CT = 0 → DMA is now filling Buffer 0
			 *          therefore Buffer 1 just completed.
			 *
			 * CT = 1 → DMA is now filling Buffer 1
			 *          therefore Buffer 0 just completed.
			 */
    		uint8_t current_target = (DMA2_Stream0->CR & DMA_CR_CT) ? 1 : 0;
			uint16_t sum = 0;

			if(current_target == 0)
			{
				UART_WriteString("Buffer 0 - Second half: ");

				for(int i = 0; i < 4; i++)
				{
					UART_WriteInt(adc_buffer1[i]);
					UART_WriteString(" ");

					sum += adc_buffer1[i];
				}
			}
			else
			{
				UART_WriteString("Buffer 1 - Second half: ");

				for(int i = 0; i < 4; i++)
				{
					UART_WriteInt(adc_buffer0[i]);
					UART_WriteString(" ");

					sum += adc_buffer0[i];
				}
			}
			UART_WriteString("\r\n");

			UART_WriteString("Average: ");
			UART_WriteInt(sum / 4);
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

