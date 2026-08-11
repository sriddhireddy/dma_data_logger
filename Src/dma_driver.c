#include "dma_driver.h"

#define DMA2EN			(1U<<22)
#define DMA_CR_EN		(1U<<0)

//#define DMA_CR_CHSEL	(0U<<25) 	//default

#define DMA_CR_CHSEL_Pos    25U
#define DMA_CR_CHSEL_Msk    (7U << DMA_CR_CHSEL_Pos)

//#define DMA_CR_DIR		(0U<<6) 	//default
#define DMA_CR_DIR_Pos      6U
#define DMA_CR_DIR_Msk      (3U << DMA_CR_DIR_Pos)

#define DMA_CR_MINC		(1U<<10)
#define DMA_CR_PINC		(0U<<9)		//default
//#define DMA_CR_PSIZE	(1U<<11)
#define DMA_CR_PSIZE_Pos    11U
#define DMA_CR_PSIZE_Msk    (3U << DMA_CR_PSIZE_Pos)

//#define DMA_CR_MSIZE	(1U<<13)
#define DMA_CR_MSIZE_Pos    13U
#define DMA_CR_MSIZE_Msk    (3U << DMA_CR_MSIZE_Pos)

#define DMA_CR_CIRC		(1U<<8)
#define DMA_CR_TCIE		(1U<<4)
#define DMA_CR_HTIE		(1U<<3)


#define DMA_FCR_DMDIS_Pos		(2U)
#define DMA_FCR_DMDIS_Msk		(1U << DMA_FCR_DMDIS_Pos)
#define DMA_FCR_FTH_Pos			(0U)
#define DMA_FCR_FTH_Msk			(3U << DMA_FCR_FTH_Pos)
#define DMA_FCR_FS_Pos			(3U)
#define DMA_FCR_FS_Msk			(7U << DMA_FCR_FS_Pos)

#define DMA_LIFCR_CTCIF		(1U<<5)
#define DMA_LISR_TCIF		(1U<<5)

#define DMA_LIFCR_CHTIF    	(1U<<4)
#define DMA_LISR_HTIF      	(1U<<4)

#define DMA_LISR_TEIF    	(1U<<3)
#define DMA_LIFCR_CTEIF  	(1U<<3)

#define DMA_LISR_DMEIF   	(1U<<2)
#define DMA_LIFCR_CDMEIF 	(1U<<2)

#define DMA_LISR_FEIF    	(1U<<0)
#define DMA_LIFCR_CFEIF  	(1U<<0)


static volatile uint8_t dma_transfer_complete = 0;
static volatile uint8_t dma_half_transfer = 0;

static volatile uint8_t dma_transfer_error = 0;
static volatile uint8_t dma_direct_error = 0;
static volatile uint8_t dma_fifo_error = 0;

//NOTE: clear-set for multi bit and directly set for single bit

void DMA2_Stream0_Init(uint32_t src, uint32_t dst, uint32_t len)
{
    /* enable clk access to dma */
	RCC->AHB1ENR |= DMA2EN;

    /* disable dma2 stream0 */
	DMA2_Stream0_Stop();

    /* clear all interrupt flags of Stream0 */
	DMA2->LIFCR = (1U<<0) | (15U<<2);

    /* set peripheral address */
	DMA2_Stream0->PAR = src;

    /* set memory address */
	DMA2_Stream0->M0AR = dst;

    /* set transfer length */
	DMA2_Stream0->NDTR = len;

    /* select stream0 channel0 */
//	DMA2_Stream0->CR |= DMA_CR_CHSEL;
	DMA2_Stream0->CR &= ~DMA_CR_CHSEL_Msk; //CLEAR
	DMA2_Stream0->CR |= (0U << DMA_CR_CHSEL_Pos);; //SET

    /* configure peripheral-to-memory transfer direction */
//	DMA2_Stream0->CR |= DMA_CR_DIR;
	DMA2_Stream0->CR &= ~DMA_CR_DIR_Msk;
	DMA2_Stream0->CR |= (0U << DMA_CR_DIR_Pos);

    /* enable memory increment */
	DMA2_Stream0->CR |= DMA_CR_MINC;

    /* disable peripheral increment */
	DMA2_Stream0->CR |= DMA_CR_PINC;

    /* configure peripheral data size as half-word */
//	DMA2_Stream0->CR |= DMA_CR_PSIZE;
	DMA2_Stream0->CR &= ~DMA_CR_PSIZE_Msk;
	DMA2_Stream0->CR |= (1U << DMA_CR_PSIZE_Pos);

    /* configure memory data size as half-word */
//	DMA2_Stream0->CR |= DMA_CR_MSIZE;
	DMA2_Stream0->CR &= ~DMA_CR_MSIZE_Msk;
	DMA2_Stream0->CR |= (1U << DMA_CR_MSIZE_Pos);

    /* enable circular mode */
	DMA2_Stream0->CR |= DMA_CR_CIRC;

//    /* enable direct mode and disable FIFO */
//	DMA2_Stream0->FCR = 0;


	/* disable direct mode to enable FIFO mode */
	DMA2_Stream0->FCR &= ~(DMA_FCR_DMDIS_Msk);
	DMA2_Stream0->FCR |= (1U<<DMA_FCR_DMDIS_Pos);

	/* configure FIFO threshold as 1/2 full */
	DMA2_Stream0->FCR &= ~(DMA_FCR_FTH_Msk);
	DMA2_Stream0->FCR |= (1U<<DMA_FCR_FTH_Pos);


    /* enable dma transfer complete interrupt */
	DMA2_Stream0->CR |= DMA_CR_TCIE;

	/* enable dma half transfer interrupt */
	DMA2_Stream0->CR |= DMA_CR_HTIE;

    /* enable dma2 stream0 */
	//in main

    /* enable dma2 stream0 interrupt in NVIC */
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);




}

void DMA2_Stream0_IRQHandler(void)
{
    if(DMA2->LISR & DMA_LISR_HTIF)
    {
        /* clear half-transfer flag */
    	DMA2->LIFCR = DMA_LIFCR_CHTIF;

        /* indicate half-transfer */
    	dma_half_transfer = 1;
    }

    if(DMA2->LISR & DMA_LISR_TCIF)
    {
        /* clear transfer-complete flag */
		DMA2->LIFCR = DMA_LIFCR_CTCIF;

        /* indicate transfer complete */
		dma_transfer_complete = 1;
    }

    if (DMA2->LISR & DMA_LISR_TEIF)
    {
        DMA2->LIFCR = DMA_LIFCR_CTEIF;
        dma_transfer_error = 1;
    }

    if (DMA2->LISR & DMA_LISR_DMEIF)
    {
        DMA2->LIFCR = DMA_LIFCR_CDMEIF;
        dma_direct_error = 1;
    }

    if (DMA2->LISR & DMA_LISR_FEIF)
    {
        DMA2->LIFCR = DMA_LIFCR_CFEIF;
        dma_fifo_error = 1;
    }

}


uint8_t DMA_TransferComplete(void){
		if(dma_transfer_complete){
			dma_transfer_complete = 0;
			return 1;
		}
		return 0;
	}

uint8_t DMA_HalfTransferComplete(void){
		if(dma_half_transfer){
			dma_half_transfer = 0;
			return 1;
		}
		return 0;
	}

uint8_t DMA_TransferError(void){
		if(dma_transfer_error){
			dma_transfer_error = 0;
			return 1;
		}
		return 0;
	}

uint8_t DMA_DirectError(void){
		if(dma_direct_error){
			dma_direct_error = 0;
			return 1;
		}
		return 0;
	}

uint8_t DMA_FifoError(void){
		if(dma_fifo_error){
			dma_fifo_error = 0;
			return 1;
		}
		return 0;
	}

void DMA2_Stream0_Stop(void)
{
	/* disable dma2 stream0 */
	DMA2_Stream0->CR &= ~(DMA_CR_EN);

	/* wait while dma stream0 is disabled */
	while(DMA2_Stream0->CR & DMA_CR_EN){}
}

void DMA2_Stream0_Start(void)
{
    /* enable dma2 stream0 */
    DMA2_Stream0->CR |= DMA_CR_EN;
}


uint8_t DMA_FifoStatus(void){
	uint8_t status;

	status = (DMA2_Stream0->FCR & DMA_FCR_FS_Msk) >> DMA_FCR_FS_Pos;
	//value = (REGISTER & FIELD_MASK) >> FIELD_POSITION;

	return status;
}
