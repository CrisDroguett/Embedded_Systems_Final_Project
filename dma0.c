// Cris Droguett
// Embedded Systems Final Project
#include "dma0.h"
#include <stdint.h>
#include <MKL25Z4.h>
volatile static void *gfrom;
volatile static uint16_t glength;
void configure_dma0(void *from, void *to, uint8_t length)
{
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	DMA0 -> DMA[0].DCR =
		   DMA_DCR_ERQ_MASK |
		   DMA_DCR_CS_MASK  |
		   DMA_DCR_SINC_MASK|
		   DMA_DCR_SSIZE(0) |
		   DMA_DCR_DSIZE(0) |
		   DMA_DCR_EINT_MASK;
	DMA0->DMA[0].SAR = (uint32_t) from;
	DMA0->DMA[0].DAR = (uint32_t) to;
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2*length);
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(49) | DMAMUX_CHCFG_ENBL_MASK;

	glength = 1;
	gfrom = from;
	NVIC_SetPriority(DMA0_IRQn,2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);
}

void DMA0_IRQHandler()
{
	DMA0->DMA[0].SAR = (uint32_t) gfrom;
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2*glength) | DMA_DSR_BCR_DONE_MASK;
}


