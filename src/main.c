#include "../include/baremetal.h"

#define DELAY_SECOND 2200000UL

void spin_delay (volatile u32 ticks) {
	while(ticks--);
}

int main() 
{
	RCC->AHB1ENR |= 0
		| RCC_AHB1ENR_GPIODEN
		| RCC_AHB1ENR_DMA2EN
		;

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	GPIOD->MODER = 0
		| GPIO_MODER_OUTPUT_N(12)
	        | GPIO_MODER_OUTPUT_N(13)
	        | GPIO_MODER_OUTPUT_N(14)
		| GPIO_MODER_OUTPUT_N(15)
		;

	TIM1->PSC = 64;
	TIM1->DIER = TIM_DIER_UDE;

	u32 gpio_instructions[] = {
		GPIO_BSRR_SET(12) | GPIO_BSRR_SET(13) | GPIO_BSRR_SET(14),
		GPIO_BSRR_RESET(12),
		GPIO_BSRR_SET(12) | GPIO_BSRR_RESET(13),
		GPIO_BSRR_RESET(12),
		GPIO_BSRR_SET(12) | GPIO_BSRR_SET(13) | GPIO_BSRR_RESET(14),
		GPIO_BSRR_RESET(12),
		GPIO_BSRR_SET(12) | GPIO_BSRR_RESET(13),
		GPIO_BSRR_RESET(12),
	};

	DMA_Stream_TypeDef *stream = DMA2_Stream5; 

	// write to BSSR
	stream->PAR  = (uintptr_t)&GPIOD->BSRR;
	stream->M0AR = (uintptr_t)&gpio_instructions[0];
	stream->NDTR = sizeof(gpio_instructions) / sizeof(gpio_instructions[0]);
	stream->FCR  = DMA_SxFCR_DMDIS;

	stream->CR = 0
		| DMA_SxCR_CHSEL_N(6)
		| DMA_SxCR_MSIZE_32BIT
		| DMA_SxCR_PSIZE_32BIT
		| DMA_SxCR_MINC
		| DMA_SxCR_CIRC
		| DMA_SxCR_DIR_MEMORY_TO_PERIPHERAL
		| DMA_SxCR_PL_PRIORITY_LOW
		| DMA_SxCR_EN
		;

	TIM1->CR1  |= TIM_CR1_CEN;

	return 0;
}
