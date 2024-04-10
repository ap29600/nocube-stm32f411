#include "../include/baremetal.h"

#define DELAY_SECOND 2200000UL


void spin_delay (volatile u32 ticks) {
	while(ticks--);
}


size_t strlen(char const *data)
{
	size_t i = 0;
	for (; data[i] ; ++i);
	return i;
}


size_t readline(char *data, size_t max_size)
{
	if (max_size == 0)
	{
		return 0;
	}

	size_t i = 0;
	for (; i < max_size - 1; ++i)
	{
		while (!(USART2->status & USART_STATUS__READ_REGISTER_NOT_EMPTY));
		data[i] = USART2->data;
		if (data[i] == '\n')
		{
			++i;
			break;
		}
	}

	data[i] = '\0';
	return i;
}


void write(char const *data, size_t size)
{
	for (u32 i = 0; i < size; ++i)
	{
		while (!(USART2->status & USART_STATUS__TRANSMIT_REGISTER_EMPTY));
		USART2->data = data[i];
		while (!(USART2->status & USART_STATUS__TRANSMISSION_COMPLETE));
	}
}


int main() {
	/// ============== CONFIGURE CLOCK
	RCC->AHB1_peripheral_enable |= 0
		| RCC_AHB1_PERIPHERAL_ENABLE__GPIOA
		| RCC_AHB1_PERIPHERAL_ENABLE__GPIOD
		;

	RCC->APB1_peripheral_enable |= 0
		| RCC_APB1_PERIPHERAL_ENABLE__USART2
		;

	/// =============== CONFIGURE USART
	
	static const u32 baud_rate    = 9600UL;
	static const u32 core_clock   = 16UL * 1000UL * 1000UL;
	static const u32 uart_divider = core_clock / baud_rate;
	USART2->baud_rate = 0
		| USART_BAUD_RATE__MANTISSA(uart_divider / 16UL)
		| USART_BAUD_RATE__FRACTION(uart_divider % 16UL)
		;
	
	USART2->control1 |= 0
		| USART_CONTROL1__USART_ENABLE
		| USART_CONTROL1__TRANSMITTER_ENABLE
		| USART_CONTROL1__RECEIVER_ENABLE
		;

	GPIOA->mode |= 0
		| (GPIO_MODE__ALTERNATE * (GPIO_WPIN(3) | GPIO_WPIN(2)))
		;

	GPIOA->alternate_function = 0
		| (7 * (GPIO_QPIN(3) | GPIO_QPIN(2)))
		;

	/// =============== CONFIGURE GPIOD
	GPIOD->mode = 0
		| (GPIO_MODE__OUTPUT * (GPIO_WPIN(15) | GPIO_WPIN(14) | GPIO_WPIN(13) | GPIO_WPIN(12)))
		;

	GPIOD->output_speed = 0
		| (GPIO_OUTPUT_SPEED__LOW * (GPIO_WPIN(15) | GPIO_WPIN(14) | GPIO_WPIN(13) | GPIO_WPIN(12)))
		;

	char const *prompt = "insert command >> ";
	size_t size = strlen(prompt);

	while (true)
	{
		write(prompt, size);

		char line[32];
		readline(line, sizeof(line));

		switch (line[0])
		{
		case '0': 
			GPIOD->bit_set_reset = 0
				| GPIO_BIT_SET_RESET__SET(GPIO_PIN(12))
				| GPIO_BIT_SET_RESET__RESET(GPIO_PIN(13) | GPIO_PIN(14))
				;
			break;
		case '1':
			GPIOD->bit_set_reset = 0
				| GPIO_BIT_SET_RESET__SET(GPIO_PIN(13))
				| GPIO_BIT_SET_RESET__RESET(GPIO_PIN(12) | GPIO_PIN(14))
				;
			break;
		case '2':
			GPIOD->bit_set_reset = 0
				| GPIO_BIT_SET_RESET__SET(GPIO_PIN(14))
				| GPIO_BIT_SET_RESET__RESET(GPIO_PIN(12) | GPIO_PIN(13))
				;
			break;
		default:
			GPIOD->bit_set_reset = 0
				| GPIO_BIT_SET_RESET__RESET(GPIO_PIN(12) | GPIO_PIN(13) | GPIO_PIN(14))
				;
			break;

		}
	}
}
