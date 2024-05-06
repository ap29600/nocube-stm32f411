#include "../include/baremetal.h"

#define __PROGRAM_START
#define STM32F411xE
#include "../vendor/stm32f4xx.h"

#define DELAY_SECOND 2200000UL

void spin_delay (volatile u32 ticks) {
	while(ticks--);
}

size_t strlen(char const *data)
{
	size_t i = 0;
	for (; data[i] != '\0'; ++i)
	{
	}
	return i;
}

void write(const char *data, size_t size);
void writeln(const char *data, size_t size)
{
	write(data, size);
	write("\r\n", 2);
}

char *u32_to_hex(char *buf, u32 num, size_t max_size)
{
	if (max_size > 9)
	{
		max_size = 9;
	}
	for (size_t i = 0; i < max_size; ++i)
	{
		buf[i] = "0123456789ABCDEF"[num >> 28];
		num <<= 4;
	}
	buf[max_size - 1] = '\0';
	return buf;
}


char getchar(void)
{
	while (!(USART2->SR & USART_SR_RXNE))
	{
	}
	return USART2->DR;
}


void putchar(int c)
{
	while (!(USART2->SR & USART_SR_TXE))
	{
	}
	USART2->DR = c;
	while (!(USART2->SR & USART_SR_TC))
	{
	}
}


size_t readline(char *restrict data, size_t max_size)
{
	if (max_size == 0)
	{
		return 0;
	}

	size_t i = 0;
	for (; i < max_size - 1; ++i)
	{
		char c = getchar();
		if (c == '\n')
		{
			++i;
			break;
		}
		data[i] = c;
	}

	data[i] = '\0';
	return i;
}

void write(char const *data, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		putchar(data[i]);
	}
}

static void USART_enable(u32 baud_rate)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	u32 mode = GPIOA->MODER;
	mode &= ~(GPIO_MODER_MODE3_Msk | GPIO_MODER_MODE2_Msk);
	mode |= (0b10 << GPIO_MODER_MODE3_Pos) | (0b10 << GPIO_MODER_MODE2_Pos);
	GPIOA->MODER = mode;

	u32 afr = GPIOA->AFR[0];
	afr &= ~(GPIO_AFRL_AFSEL3_Msk | GPIO_AFRL_AFSEL2_Msk);
	afr |= (7 << GPIO_AFRL_AFSEL3_Pos) | (7 << GPIO_AFRL_AFSEL2_Pos);
	GPIOA->AFR[0] = afr;

	const u32 uart_divider = 16000000UL / baud_rate;
	USART2->BRR = uart_divider;
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART2->CR2 = USART_CR2_STOP_1;
	USART2->CR3 = USART_CR3_DMAT;

	while (!(USART2->SR & USART_SR_TC))
	{
	}
}
// 
// STATIC INLINE void GPIO_set_reset(struct GPIO_config *GPIO, u32 set_bits, u32 reset_bits)
// {
// 	GPIO->bit_set_reset = GPIO_BIT_SET_RESET__SET(set_bits) | GPIO_BIT_SET_RESET__RESET(reset_bits);
// }

void dma1_stream6_transmission_complete_interrupt_handler(void)
{
	u32 status = DMA1->HISR;

	if (status & DMA_HISR_TEIF6) // transfer error
	{
		DMA1->HIFCR = DMA_HIFCR_CTEIF6; // clear transfer error interrupt
		GPIOD->BSRR |= GPIO_BSRR_BS12;
	}

	if (status & DMA_HISR_TCIF6) // transfer complete
	{
		DMA1->HIFCR = DMA_HIFCR_CTCIF6; // clear transfer complete interrupt
		GPIOD->BSRR |= GPIO_BSRR_BS13;
	}

	if (status & DMA_HISR_HTIF6) // half transfer
	{
		DMA1->HIFCR = DMA_HIFCR_CHTIF6; // clear half transfer interrupt
		GPIOD->BSRR = GPIO_BSRR_BS14;
	}

	if (status & DMA_HISR_DMEIF6) // direct mode error
	{
		DMA1->HIFCR = DMA_HIFCR_CDMEIF6; // clear direct mode error interrupt
		GPIOD->BSRR = GPIO_BSRR_BS15;
	}
}

#define GPIO_MODER_INPUT     0b00UL
#define GPIO_MODER_OUTPUT    0b01UL
#define GPIO_MODER_ANALOG    0b10UL
#define GPIO_MODER_ALTERNATE 0b11UL
#define GPIO_MODER_SET(__N__, __MODE__) ((u32)(__MODE__) << ((__N__) * 2))
int main() 
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER = GPIO_MODER_SET(12, GPIO_MODER_OUTPUT)
	             | GPIO_MODER_SET(13, GPIO_MODER_OUTPUT)
	             | GPIO_MODER_SET(14, GPIO_MODER_OUTPUT)
		     | GPIO_MODER_SET(15, GPIO_MODER_OUTPUT);

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	char *message = "Hello, world!\n";
	
#define DMA_SxCR_CHSEL_N(__CHANNEL__) ((u32)__CHANNEL__ << DMA_SxCR_CHSEL_Pos)
#define DMA_SxCR_DIR_MEMORY_TO_PERIPHERAL (0b01UL << DMA_SxCR_DIR_Pos)

	USART_enable(9600);
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

	DMA1_Stream6->PAR  = (uintptr_t)& USART2->DR;
	DMA1_Stream6->M0AR = (uintptr_t) message;
	DMA1_Stream6->NDTR = strlen(message);
	DMA1_Stream6->FCR = 0; // direct mode, fifo disabled
	DMA1_Stream6->CR = DMA_SxCR_CHSEL_N(4)
	                 | DMA_SxCR_MINC
		         | DMA_SxCR_DIR_MEMORY_TO_PERIPHERAL
		         | DMA_SxCR_TCIE
		         | DMA_SxCR_HTIE
		         | DMA_SxCR_TEIE
			 | DMA_SxCR_DMEIE
			 | DMA_SxCR_EN;

	while (true)
	{
		spin_delay(DELAY_SECOND / 10);
		GPIOD->ODR ^= GPIO_ODR_OD12;
	}

	return 0;
}
