#ifndef DMA0_H
#define DMA0_H
#include <stdint.h>

volatile static void *gfrom;
volatile static uint16_t glength;
void configure_dma0(void *from, void *to, uint8_t length);
void DMA0_IRQHandler();

#endif 


