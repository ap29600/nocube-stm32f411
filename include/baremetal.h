#ifndef BAREMETAL_H
#define BAREMETAL_H

#define STATIC __attribute__((unused)) static
#define INLINE __attribute__((always_inline)) inline

typedef unsigned char bool;

typedef unsigned long size_t;
typedef unsigned long uintptr_t;

typedef unsigned long u32;
typedef          long i32;

typedef unsigned int u16;
typedef          int i16;

typedef unsigned char u8;
typedef          char i8;

typedef unsigned char byte;

#define true  ((bool)1)
#define false ((bool)0)
#define NULL ((void*)0)

#define INTERRUPT_SERVICE_ROUTINE_VECTOR_LEN 256

void reset_handler_procedure(void);
int main (void);

// ================= section addresses, to be provided by the linker script

/// Incomplete struct used to describe the type of linker-provided memory region bounds.
/// it would be undefined behaviour to make these a complete type and then write to them 
/// outside of the type's in-memory size.
///
/// `TODO`: some lawyering required to make sure this is not UB anyway.
struct memory_section;

extern struct memory_section __stack_end;

extern struct memory_section __flash_data_section_begin;
extern struct memory_section __sram_data_section_begin;
extern struct memory_section __sram_data_section_end;

extern struct memory_section __text_section_begin;
extern struct memory_section __text_section_end;

extern struct memory_section __bss_section_begin;
extern struct memory_section __bss_section_end;
// =================

extern u32 interrupt_service_routine_vector[INTERRUPT_SERVICE_ROUTINE_VECTOR_LEN];

STATIC struct GPIO_config *const GPIO_config_A = (struct GPIO_config*)0x40020000;
STATIC struct GPIO_config *const GPIO_config_B = (struct GPIO_config*)0x40020400;
STATIC struct GPIO_config *const GPIO_config_C = (struct GPIO_config*)0x40020800;
STATIC struct GPIO_config *const GPIO_config_D = (struct GPIO_config*)0x40020c00;
STATIC struct GPIO_config *const GPIO_config_E = (struct GPIO_config*)0x40021000;
STATIC struct GPIO_config *const GPIO_config_H = (struct GPIO_config*)0x40021c00;
struct __attribute__((packed)) GPIO_config {
	volatile u32 mode;
	volatile u32 output_type;
	volatile u32 output_speed;
	volatile u32 pull_type;
	volatile u32 input_data;
	volatile u32 output_data;
	volatile u32 bit_set_reset;
	volatile u32 port_configuration_lock;
	volatile u32 alternate_function_low;
	volatile u32 alternate_function_high;
};

enum GPIO_PIN : u32 {
	GPIO_PIN_00 = 1 <<  0,
	GPIO_PIN_01 = 1 <<  1,
	GPIO_PIN_02 = 1 <<  2,
	GPIO_PIN_03 = 1 <<  3,
	GPIO_PIN_04 = 1 <<  4,
	GPIO_PIN_05 = 1 <<  5,
	GPIO_PIN_06 = 1 <<  6,
	GPIO_PIN_07 = 1 <<  7,
	GPIO_PIN_08 = 1 <<  8,
	GPIO_PIN_09 = 1 <<  9,
	GPIO_PIN_10 = 1 << 10,
	GPIO_PIN_11 = 1 << 11,
	GPIO_PIN_12 = 1 << 12,
	GPIO_PIN_13 = 1 << 13,
	GPIO_PIN_14 = 1 << 14,
	GPIO_PIN_15 = 1 << 15,
};

/*
 * defines the lsb of bit pairs corresponding to GPIO pins.
 * can be used to broadcast configuration numbers to several pins through multiplication:
 *
 * `CONFIG_NUMBER_BIT_PAIR * (GPIO_WPIN_XX | GPIO_WPIN_YY)`
 *
 */
enum GPIO_WPIN : u32 {
	GPIO_WPIN_00 = 1 <<  0,
	GPIO_WPIN_01 = 1 <<  2,
	GPIO_WPIN_02 = 1 <<  4,
	GPIO_WPIN_03 = 1 <<  6,
	GPIO_WPIN_04 = 1 <<  8,
	GPIO_WPIN_05 = 1 << 10,
	GPIO_WPIN_06 = 1 << 12,
	GPIO_WPIN_07 = 1 << 14,
	GPIO_WPIN_08 = 1 << 16,
	GPIO_WPIN_09 = 1 << 18,
	GPIO_WPIN_10 = 1 << 20,
	GPIO_WPIN_11 = 1 << 22,
	GPIO_WPIN_12 = 1 << 24,
	GPIO_WPIN_13 = 1 << 26,
	GPIO_WPIN_14 = 1 << 28,
	GPIO_WPIN_15 = 1 << 30,
};

enum GPIO_MODE : u32 
{
	GPIO_MODE__INPUT     = 0b00UL,
	GPIO_MODE__OUTPUT    = 0b01UL,
	GPIO_MODE__ALTERNATE = 0b10UL,
	GPIO_MODE__ANALOG    = 0b11UL,
};

enum GPIO_OUTPUT_TYPE : u32
{
	GPIO_OUTPUT_TYPE__PUSH_PULL  = 0b0UL,
	GPIO_OUTPUT_TYPE__OPEN_DRAIN = 0b1UL,
};

enum GPIO_OUTPUT_SPEED : u32
{
	GPIO_OUTPUT_SPEED__LOW     = 0b00UL,
	GPIO_OUTPUT_SPEED__MID     = 0b01UL,
	GPIO_OUTPUT_SPEED__HIGH    = 0b10UL,
	GPIO_OUTPUT_SPEED__HIGHEST = 0b11UL,
};

enum GPIO_PULL_TYPE : u32 
{
	GPIO_PULL_TYPE__NONE = 0b00UL,
	GPIO_PULL_TYPE__UP   = 0b01UL,
	GPIO_PULL_TYPE__DOWN = 0b10UL,
};

enum GPIO_BIT_SET_RESET : u32 
{
	GPIO_BIT_SET_RESET__SET_SHIFT	= 16UL,
#define GPIO_BIT_SET_RESET__SET(mask)	((mask & 0xFFFFUL) << GPIO_BIT_SET_RESET__SET_SHIFT)
	GPIO_BIT_SET_RESET__RESET_SHIFT = 0UL,
#define GPIO_BIT_SET_RESET__RESET(mask)	((mask & 0xFFFFUL) << GPIO_BIT_SET_RESET__RESET_SHIFT)
};



STATIC struct RCC_config *const RCC_config = (struct RCC_config*)0x40023800;
struct __attribute__((packed)) RCC_config 
{
	volatile u32 clock_control;	 	/*!< RCC clock control register,				Address offset: 0x00 */
	volatile u32 PLL_config;	     	/*!< RCC PLL configuration register,				Address offset: 0x04 */
	volatile u32 clock_config;	   	/*!< RCC clock configuration register,				Address offset: 0x08 */
	volatile u32 interrupt;	      		/*!< RCC clock interrupt register,				Address offset: 0x0C */
	volatile u32 AHB1_peripheral_reset;  	/*!< RCC AHB1 peripheral reset register,			Address offset: 0x10 */
	volatile u32 AHB2_peripheral_reset;  	/*!< RCC AHB2 peripheral reset register,			Address offset: 0x14 */
	volatile u32 AHB3_peripheral_reset;  	/*!< RCC AHB3 peripheral reset register,			Address offset: 0x18 */
		 u32 RESERVED0;	      		/*!< Reserved, 0x1C								     */
	volatile u32 APB1_peripheral_reset;  	/*!< RCC APB1 peripheral reset register,			Address offset: 0x20 */
	volatile u32 APB2_peripheral_reset;  	/*!< RCC APB2 peripheral reset register,			Address offset: 0x24 */
		 u32 RESERVED1[2];	   	/*!< Reserved, 0x28-0x2C							     */
	volatile u32 AHB1_peripheral_enable; 	/*!< RCC AHB1 peripheral clock register,			Address offset: 0x30 */
	volatile u32 AHB2_peripheral_enable; 	/*!< RCC AHB2 peripheral clock register,			Address offset: 0x34 */
	volatile u32 AHB3_peripheral_enable; 	/*!< RCC AHB3 peripheral clock register,			Address offset: 0x38 */
		 u32 RESERVED2;	      		/*!< Reserved, 0x3C								     */
	volatile u32 APB1_peripheral_enable;	/*!< RCC APB1 peripheral clock enable register,			Address offset: 0x40 */
	volatile u32 APB2_peripheral_enable;	/*!< RCC APB2 peripheral clock enable register,			Address offset: 0x44 */
		 u32 RESERVED3[2];  		/*!< Reserved, 0x48-0x4C							     */
	volatile u32 AHB1_low_power_enable;	/*!< RCC AHB1 peripheral clock enable in low power mode register,	Address offset: 0x50 */
	volatile u32 AHB2_low_power_enable;	/*!< RCC AHB2 peripheral clock enable in low power mode register,	Address offset: 0x54 */
	volatile u32 AHB3_low_power_enable;	/*!< RCC AHB3 peripheral clock enable in low power mode register,	Address offset: 0x58 */
		 u32 RESERVED4;     		/*!< Reserved, 0x5C								     */
	volatile u32 APB1_low_power_enable;	/*!< RCC APB1 peripheral clock enable in low power mode register,	Address offset: 0x60 */
	volatile u32 APB2_low_power_enable;	/*!< RCC APB2 peripheral clock enable in low power mode register,	Address offset: 0x64 */
		 u32 RESERVED5[2];  		/*!< Reserved, 0x68-0x6C							     */
	volatile u32 backup_domain;		/*!< RCC Backup domain control register,			Address offset: 0x70 */
	volatile u32 clock_control_status;	/*!< RCC clock control & status register,			Address offset: 0x74 */
		 u32 RESERVED6[2];		/*!< Reserved, 0x78-0x7C							     */
	volatile u32 spread_spectrum_clock_generation;	/*!< RCC spread spectrum clock generation register,	Address offset: 0x80 */
	volatile u32 PLLI2S_config;		/*!< RCC PLLI2S configuration register,			  	Address offset: 0x84 */
		 u32 RESERVED7[1];		/*!< Reserved, 0x88								     */
	volatile u32 dedicated_clocks_config;	/*!< RCC Dedicated Clocks configuration register,		Address offset: 0x8C */
};

enum RCC_CONTROL : u32
{
	RCC_CONTROL__PLLI2S_READY	= 1UL << 27UL,
	RCC_CONTROL__PLLI2S_ENABLE	= 1UL << 26UL,

	RCC_CONTROL__PLL_READY 		= 1UL << 25UL,
	RCC_CONTROL__PLL_ENABLE 	= 1UL << 24UL,

	RCC_CONTROL__CSS_ENABLE		= 1UL << 19UL,

	RCC_CONTROL__HSE_BYPASS		= 1UL << 18UL,
	RCC_CONTROL__HSE_READY		= 1UL << 17UL,
	RCC_CONTROL__HSE_ENABLE		= 1UL << 16UL,

	RCC_CONTROL__HSI_CALIBRATION_SHIFT	= 8,
#define RCC_CONTROL__HSE_CALIBRATION(n)		(((n) & 0xFF) << RCC_CONTROL__HSI_CALIBRATION_SHIFT)
#define RCC_CONTROL__HSE_CALIBRATION_GET(mask)	(((mask) >> RCC_CONTROL__HSI_CALIBRATION_SHIFT) & 0xFF)

	RCC_CONTROL__HSI_TRIM_SHIFT		= 4,
#define RCC_CONTROL__HSE_TRIM(n)	(((n) & 0x1F) << RCC_CONTROL__HSI_TRIM_SHIFT)
#define RCC_CONTROL__HSE_TRIM_GET(mask)	(((mask) >> RCC_CONTROL__HSI_TRIM_SHIFT) & 0x1F)

	RCC_CONTROL__HSI_READY		= 1ULL << 1UL,
	RCC_CONTROL__HSI_ENABLE		= 1ULL << 0UL,

};

enum RCC_AHB1_PERIPHERAL_ENABLE : u32
{
	RCC_AHB1_PERIPHERAL_ENABLE__DMA2	= 1UL << 22UL,
	RCC_AHB1_PERIPHERAL_ENABLE__DMA1	= 1UL << 21UL,
	RCC_AHB1_PERIPHERAL_ENABLE__CRC		= 1UL << 12UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOH	= 1UL <<  7UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOE	= 1UL <<  4UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOD	= 1UL <<  3UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOC	= 1UL <<  2UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOB	= 1UL <<  1UL,
	RCC_AHB1_PERIPHERAL_ENABLE__GPIOA	= 1UL <<  0UL,
};


STATIC struct PWR_config *const PWR_config = (struct PWR_config*)0x40007000;
struct __attribute__((packed)) PWR_config 
{
	volatile u32 control;		/*!< PWR power control register,        Address offset: 0x00 */
	volatile u32 control_status;	/*!< PWR power control/status register, Address offset: 0x04 */
};


STATIC struct FLASH_config *const FLASH_config = (struct FLASH_config*)0x40023c00;
struct __attribute__((packed)) FLASH_config
{
	volatile u32 access_control;	/*!< FLASH access control register,   Address offset: 0x00 */
	volatile u32 key;		/*!< FLASH key register,              Address offset: 0x04 */
	volatile u32 option_key;	/*!< FLASH option key register,       Address offset: 0x08 */
	volatile u32 status;		/*!< FLASH status register,           Address offset: 0x0C */
	volatile u32 control;		/*!< FLASH control register,          Address offset: 0x10 */
	volatile u32 option_control;	/*!< FLASH option control register ,  Address offset: 0x14 */
	volatile u32 option_control_1;	/*!< FLASH option control register 1, Address offset: 0x18 */
};


enum FLASH_ACCESS_CONTROL : u32
{
	FLASH_ACCESS_CONTROL__DATA_CACHE	= 1UL << 10UL,
	FLASH_ACCESS_CONTROL__INSTRUCTION_CACHE = 1UL <<  9UL,
	FLASH_ACCESS_CONTROL__PREFETCH		= 1UL <<  8UL,
	FLASH_ACCESS_CONTROL__LATENCY_SHIFT	= 0UL,
#define FLASH_ACCESS_CONTROL__LATENCY(n)	(((n) & 0xFUL) << FLASH_ACCESS_CONTROL__LATENCY_SHIFT)
#define FLASH_ACCESS_CONTROL__LATENCY_GET(mask)	(((mask) >> FLASH_ACCESS_CONTROL__LATENCY_SHIFT) & 0xFUL)
};


STATIC struct USART_config *const USART_config_1 = (struct USART_config*)0x40011000;
STATIC struct USART_config *const USART_config_2 = (struct USART_config*)0x40004400;
STATIC struct USART_config *const USART_config_6 = (struct USART_config*)0x40011400;
struct __attribute__((packed)) USART_config
{
	volatile u16 status;               u16 const reserved_status;
	volatile u16 data;                 u16 const reserved_data;
	volatile u16 baud_rate;            u16 const reserved_baud_rate;
	volatile u16 control1;             u16 const reserved_control1;
	volatile u16 control2;             u16 const reserved_control2;
	volatile u16 control3;             u16 const reserved_control3;
	volatile u16 guard_time_prescaler; u16 const reserved_guard_time_prescaler;
};

enum USART_STATUS : u32
{
	USART_STATUS__CTS_LINE_DETECTED       = 1UL << 9UL,
	USART_STATUS__LIN_BREAK_DETECTED      = 1UL << 8UL,
	USART_STATUS__TRANSMIT_REGISTER_EMPTY = 1UL << 7UL,
	USART_STATUS__TRANSMISSION_COMPLETE   = 1UL << 6UL,
	USART_STATUS__READ_REGISTER_NOT_EMPTY = 1UL << 5UL,
	USART_STATUS__IDLE_LINE_DETECTED      = 1UL << 4UL,
	USART_STATUS__OVERRUN_ERROR_DETECTED  = 1UL << 3UL,
	USART_STATUS__NOISE_DETECTED          = 1UL << 2UL,
	USART_STATUS__FRAMING_ERROR_DETECTED  = 1UL << 1UL,
	USART_STATUS__PARITY_ERROR_DETECTED   = 1UL << 0UL,
};

enum USART_BAUD_RATE : u32
{
	USART_BAUD_RATE__MANTISSA_SHIFT     = 4,
	USART_BAUD_RATE__MANTISSA_BITS      = 12,
#define USART_BAUD_RATE__MANTISSA(n)        (((n) & ((1UL << USART_BAUD_RATE__MANTISSA_BITS) - 1)) << USART_BAUD_RATE__MANTISSA_SHIFT)
#define USART_BAUD_RATE__MANTISSA_GET(mask) (((mask) >> USART_BAUD_RATE__MANTISSA_SHIFT) & ((1UL << USART_BAUD_RATE__MANTISSA_BITS) - 1))

	USART_BAUD_RATE__FRACTION_SHIFT     = 0,
	USART_BAUD_RATE__FRACTION_BITS      = 4,
#define USART_BAUD_RATE__FRACTION(n)        (((n) & ((1UL << USART_BAUD_RATE__FRACTION_BITS) - 1)) << USART_BAUD_RATE__FRACTION_SHIFT)
#define USART_BAUD_RATE__FRACTION_GET(mask) (((mask) >> USART_BAUD_RATE__FRACTION_SHIFT) & ((1UL << USART_BAUD_RATE__FRACTION_BITS) - 1))
};

enum USART_CONTROL1 : u32
{
	USART_CONTROL1__8X_OVERSAMPLE                     = 1UL << 15UL,
	USART_CONTROL1__USART_ENABLE                      = 1UL << 13UL,
	USART_CONTROL1__9_BIT_WORD                        = 1UL << 12UL,
	USART_CONTROL1__WAKEUP_ADDRESS                    = 1UL << 11UL,
	USART_CONTROL1__PARITY_ENABLE                     = 1UL << 10UL,
	USART_CONTROL1__PARITY_ODD                        = 1UL <<  9UL,
	USART_CONTROL1__INTERRUPT_PARITY                  = 1UL <<  8UL,
	USART_CONTROL1__INTERRUPT_TRANSMIT_REGISTER_EMPTY = 1UL <<  7UL,
	USART_CONTROL1__INTERRUPT_TRANSMSSION_COMPLETE    = 1UL <<  6UL,
	USART_CONTROL1__INTERRUPT_READ_REGISTER_NOT_EMPTY = 1UL <<  5UL,
	USART_CONTROL1__INTERRUPT_IDLE_LINE_DETECTED      = 1UL <<  4UL,
	USART_CONTROL1__TRANSMITTER_ENABLE                = 1UL <<  3UL,
	USART_CONTROL1__RECEIVER_ENABLE                   = 1UL <<  2UL,
	USART_CONTROL1__RECEIVER_WAKEUP                   = 1UL <<  1UL,
	USART_CONTROL1__SEND_BREAK                        = 1UL <<  1UL,
};

enum USART_CONTROL2 : u32
{
	USART_CONTROL2__LIN_MODE_ENABLE              = 1UL << 14UL,
	USART_CONTROL2__STOP_BITS_1                  = 0UL << 12UL,
	USART_CONTROL2__STOP_BITS_05	             = 1UL << 12UL,
	USART_CONTROL2__STOP_BITS_2                  = 2UL << 12UL,
	USART_CONTROL2__STOP_BITS_15                 = 3UL << 12UL,
	USART_CONTROL2__CLOCK_ENABLE                 = 1UL << 11UL,
	USART_CONTROL2__CLOCK_POLARITY_HIGH          = 1UL << 10UL,
	USART_CONTROL2__CLOCK_PHASE                  = 1UL <<  9UL,
	USART_CONTROL2__LAST_BIT_CLOCK_PULSE         = 1UL <<  8UL,
	USART_CONTROL2__INTERRUPT_LIN_BREAK_DETECTED = 1UL <<  6UL,
	USART_CONTROL2__LIN_BREAK_DETECTION_11_BIT   = 1UL <<  5UL,
	USART_CONTROL2__USART_NODE_ADDRESS_SHIFT     = 0,
	USART_CONTROL2__USART_NODE_ADDRESS_BITS      = 4,
#define USART_CONTROL2__USART_NODE_ADDRESS(n)        = (((n) & ((1UL << USART_CONTROL2__USART_NODE_ADDRESS_BITS) - 1)) << USART_CONTROL2__USART_NODE_ADDRESS_SHIFT)
#define USART_CONTROL2__USART_NODE_ADDRESS_GET(mask) = (((mask) >> USART_CONTROL2__USART_NODE_ADDRESS_SHIFT) & ((1UL << USART_CONTROL2__USART_NODE_ADDRESS_BITS) - 1))
};

#endif // BAREMETAL_H
