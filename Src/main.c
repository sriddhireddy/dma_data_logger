#include "gpio_driver.h"
#include "uart_driver.h"
#include "systick_driver.h"
#include "adc_driver.h"

int main(void)
{
    GPIO_Init();
    UART_Init();
    SysTick_Init();
    ADC_Init();

    while(1)
    {
        uint16_t value = ADC_Read();

        UART_WriteString("ADC: ");
        UART_WriteInt(value);
        UART_WriteString("\r\n");

        SysTick_DelayMs(500);
    }
}
