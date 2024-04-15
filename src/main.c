#include "../include/baremetal.h"

#define DELAY_SECOND 2200000UL


void spin_delay (volatile u32 ticks) {
	while(ticks--);
}


size_t strlen(char const *data)
{
	size_t i = 0;
	for (; data[i]; ++i)
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

void USART_debug(u32 word)
{

	write("dbg! 0x", 7);

	char debug_message[8];

	for (size_t i = 0; i < sizeof(debug_message) / sizeof(debug_message[0]); ++i)
	{
		debug_message[i] = "0123456789ABCDEF"[(word >> (4 * (7 - i))) & 0xf];
	}

	writeln(debug_message, 8); 
}


char getchar(void)
{
	while (!(USART2->status & USART_STATUS__READ_REGISTER_NOT_EMPTY))
	{
	}
	return USART2->data;
}


void putchar(int c)
{
	while (!(USART2->status & USART_STATUS__TRANSMIT_REGISTER_EMPTY))
	{
	}
	USART2->data = c;
	while (!(USART2->status & USART_STATUS__TRANSMISSION_COMPLETE))
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
	RCC->AHB1_peripheral_enable |= RCC_AHB1_PERIPHERAL_ENABLE__GPIOA;

	u32 mode = GPIOA->mode;
	mode &= ~(0b11 * (GPIO_WPIN(3) | GPIO_WPIN(2)));
	mode |= GPIO_MODE__ALTERNATE * (GPIO_WPIN(3) | GPIO_WPIN(2));

	GPIOA->mode = mode;

	u64 alternate_function = GPIOA->alternate_function;
	alternate_function &= ~(0b1111 * (GPIO_QPIN(3) | GPIO_QPIN(2)));
	alternate_function |= 7        * (GPIO_QPIN(3) | GPIO_QPIN(2));
	GPIOA->alternate_function = alternate_function;
	RCC->APB1_peripheral_enable |= RCC_APB1_PERIPHERAL_ENABLE__USART2;

	const u32 uart_divider = 16000000 / baud_rate;
	USART2->baud_rate = 0
		| USART_BAUD_RATE__MANTISSA(uart_divider / 16UL)
		| USART_BAUD_RATE__FRACTION(uart_divider % 16UL)
		;

	USART2->control1 = 0
		| USART_CONTROL1__USART_ENABLE
		| USART_CONTROL1__TRANSMITTER_ENABLE
		| USART_CONTROL1__RECEIVER_ENABLE
		;

	USART2->control2 = 0
		| USART_CONTROL2__STOP_BITS_1
		;

	USART2->control3 = 0;

	while (!(USART2->status & USART_STATUS__TRANSMISSION_COMPLETE))
	{
	}
}

STATIC INLINE void GPIO_set_reset(struct GPIO_config *GPIO, u32 set_bits, u32 reset_bits)
{
	GPIO->bit_set_reset = GPIO_BIT_SET_RESET__SET(set_bits) | GPIO_BIT_SET_RESET__RESET(reset_bits);
}

int main() {

	RCC->AHB1_peripheral_enable |= RCC_AHB1_PERIPHERAL_ENABLE__GPIOD;
	GPIOD->mode = (GPIO_MODE__OUTPUT * (GPIO_WPIN(15) | GPIO_WPIN(14) | GPIO_WPIN(13) | GPIO_WPIN(12)));
	GPIOD->output_speed = (GPIO_OUTPUT_SPEED__LOW * (GPIO_WPIN(15) | GPIO_WPIN(14) | GPIO_WPIN(13) | GPIO_WPIN(12)));

	USART_enable(9600);

	char const *prompt = "insert command >> ";
	size_t size = strlen(prompt);

	while (true)
	{
		write(prompt, size);

		char data[32];
		readline(data, sizeof(data) / sizeof(data[0]));
		switch (data[0])
		{
		case '0': 
			GPIO_set_reset(GPIOD, GPIO_PIN(12), GPIO_PIN(13) | GPIO_PIN(14));
			break;
		case '1':
			GPIO_set_reset(GPIOD, GPIO_PIN(13), GPIO_PIN(12) | GPIO_PIN(14));
			break;
		case '2':
			GPIO_set_reset(GPIOD, GPIO_PIN(14), GPIO_PIN(12) | GPIO_PIN(13));
			break;
		default:
			GPIO_set_reset(GPIOD, 0, GPIO_PIN(12) | GPIO_PIN(13) | GPIO_PIN(14));
			break;

		}
	}
}
