#include "dma_driver.h"

#define DMA2EN			(1U<<22)
#define DMA_CR_EN		(1U<<0)
#define DMA_CR_CHSEL	(0U<<25) 	//default
#define DMA_CR_DIR		(0U<<6) 	//default
#define DMA_CR_MINC		(1U<<10)
#define DMA_CR_PINC		(0U<<9)		//default
#define DMA_CR_PSIZE	(1U<<11)
#define DMA_CR_MSIZE	(1U<<13)
#define DMA_CR_CIRC		(1U<<8)
#define DMA_CR_TCIE		(1U<<4)



void dma2_stream0_init(uint32_t src, uint32_t dst, uint32_t len)
{
    /* enable clk access to dma */
	RCC->AHB1ENR |= DMA2EN;

    /* disable dma2 stream0 */
	DMA2_Stream0->CR &= ~(DMA_CR_EN);

    /* wait while dma stream0 is disabled */
	while(DMA2_Stream0->CR & DMA_CR_EN){}

    /* clear all interrupt flags of Stream0 */
	DMA2->LIFCR = (1U<<0) | (15U<<2);

    /* set peripheral address */
	DMA2_Stream0->PAR = src;

    /* set memory address */
	DMA2_Stream0->M0AR = dst;

    /* set transfer length */
	DMA2_Stream0->NDTR = len;

    /* select stream0 channel0 */
	DMA2_Stream0->CR |= DMA_CR_CHSEL;

    /* configure peripheral-to-memory transfer direction */
	DMA2_Stream0->CR |= DMA_CR_DIR;

    /* enable memory increment */
	DMA2_Stream0->CR |= DMA_CR_MINC;

    /* disable peripheral increment */
	DMA2_Stream0->CR |= DMA_CR_PINC;

    /* configure peripheral data size as half-word */
	DMA2_Stream0->CR |= DMA_CR_PSIZE;

    /* configure memory data size as half-word */
	DMA2_Stream0->CR |= DMA_CR_MSIZE;

    /* enable circular mode */
	DMA2_Stream0->CR |= DMA_CR_CIRC;

    /* enable direct mode and disable FIFO */
	DMA2_Stream0->FCR = 0;

    /* enable dma transfer complete interrupt */
	//DMA2_Stream0->CR |= DMA_CR_TCIE;

    /* enable dma2 stream0 */
	DMA2_Stream0->CR |= DMA_CR_EN;

    /* enable dma2 stream0 interrupt in NVIC */
	//NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}
