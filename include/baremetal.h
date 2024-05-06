#ifndef BAREMETAL_H
#define BAREMETAL_H

#define STATIC __attribute__((unused)) static
#define INLINE __attribute__((always_inline)) inline

#include <inttypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint64_t u64;
typedef int64_t i64;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint8_t u8;
typedef int8_t i8;

typedef unsigned char byte;

#define INTERRUPT_SERVICE_ROUTINE_VECTOR_LEN 256

void reset_handler_procedure(void);
void default_handler_procedure(void);
void dma1_stream6_transmission_complete_interrupt_handler(void);
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

// ===============[ Memory Map ]=================
//
// [memory map](file://spec.pdf#page=38)

// STATIC struct GPIO_config *const GPIOA = (struct GPIO_config*)0x40020000;
// STATIC struct GPIO_config *const GPIOB = (struct GPIO_config*)0x40020400;
// STATIC struct GPIO_config *const GPIOC = (struct GPIO_config*)0x40020800;
// STATIC struct GPIO_config *const GPIOD = (struct GPIO_config*)0x40020c00;
// STATIC struct GPIO_config *const GPIOE = (struct GPIO_config*)0x40021000;
// STATIC struct GPIO_config *const GPIOH = (struct GPIO_config*)0x40021c00;
// STATIC struct RCC_config *const RCC = (struct RCC_config*)0x40023800;
// STATIC struct PWR_config *const PWR = (struct PWR_config*)0x40007000;
// STATIC struct FLASH_config *const FLASH = (struct FLASH_config*)0x40023c00;
// STATIC struct USART_config *const USART1 = (struct USART_config*)0x40011000;
// STATIC struct USART_config *const USART2 = (struct USART_config*)0x40004400;
// STATIC struct USART_config *const USART6 = (struct USART_config*)0x40011400;



// =================[ GPIO ]===================
//
// [register functions](file://spec.pdf#page=157)
//
// [register offsets](file://spec.pdf#page=163)

// struct __attribute__((packed)) GPIO_config
// {
// 	volatile u32 mode;
// 	volatile u32 output_type;
// 	volatile u32 output_speed;
// 	volatile u32 pull_type;
// 	volatile u32 input_data;
// 	volatile u32 output_data;
// 	volatile u32 bit_set_reset;
// 	volatile u32 port_configuration_lock;
// 	volatile u64 alternate_function;
// };
// 
// // Bit set generation macros.
// //
// // The WPIN and QPIN macros are useful for broadcasting packed enumeration
// // values to multiple pins through multiplication:
// //
// // ```
// // TWO_BIT_ENUM * (GPIO_WPIN(2) | GPIO_WPIN(3))
// // FOUR_BIT_ENUM * (GPIO_QPIN(2) | GPIO_QPIN(3))
// // ```
// //
// // invocations of `GPIO_QPIN` produce a 64 bit value.
// //
#define GPIO_PIN(n)	((u32)1 << (n))
#define GPIO_WPIN(n)	((u32)1 << (2 * (n)))
#define GPIO_QPIN(n)	((u64)1 << (4 * (n)))
// 
// // Set modes for individual pins in a port.
// // Two bit enum, broadcast with GPIO_WPIN.
// //
// // When set to `GPIO_MODE__ALTERNATE`, the corresponding bits in alternate_function
// enum GPIO_MODE : u32 
// {
// 	GPIO_MODE__INPUT     = 0b00UL,
// 	GPIO_MODE__OUTPUT    = 0b01UL,
// 	GPIO_MODE__ALTERNATE = 0b10UL,
// 	GPIO_MODE__ANALOG    = 0b11UL,
// };
// 
// // Set output type for individual pins in a port.
// // One bit enum, broadcast with GPIO_PIN.
// enum GPIO_OUTPUT_TYPE : u32
// {
// 	GPIO_OUTPUT_TYPE__PUSH_PULL  = 0b0UL,
// 	GPIO_OUTPUT_TYPE__OPEN_DRAIN = 0b1UL,
// };
// 
// // Set speed for individual pins in a port.
// // Two bit enum, broadcast with `GPIO_WPIN`.
// enum GPIO_OUTPUT_SPEED : u32
// {
// 	GPIO_OUTPUT_SPEED__LOW     = 0b00UL,
// 	GPIO_OUTPUT_SPEED__MID     = 0b01UL,
// 	GPIO_OUTPUT_SPEED__HIGH    = 0b10UL,
// 	GPIO_OUTPUT_SPEED__HIGHEST = 0b11UL,
// };
// 
// // Set pull up/down for individual pins in a port.
// // Two bit enum, broadcast with GPIO_WPIN.
// enum GPIO_PULL_TYPE : u32 
// {
// 	GPIO_PULL_TYPE__NONE = 0b00UL,
// 	GPIO_PULL_TYPE__UP   = 0b01UL,
// 	GPIO_PULL_TYPE__DOWN = 0b10UL,
// };
// 
// // Set or reset individual pins in a port, without affecting the others.
// // use masks built with `GPIO_PIN`.
// enum GPIO_BIT_SET_RESET : u32 
// {
// 	GPIO_BIT_SET_RESET__RESET_SHIFT = 16UL,
// #define GPIO_BIT_SET_RESET__RESET(mask)	(((mask) & 0xFFFFUL) << GPIO_BIT_SET_RESET__RESET_SHIFT)
// 	GPIO_BIT_SET_RESET__SET_SHIFT	= 0UL,
// #define GPIO_BIT_SET_RESET__SET(mask)	(((mask) & 0xFFFFUL) << GPIO_BIT_SET_RESET__SET_SHIFT)
// };
// 
// // ===============[ RCC ]=================
// //
// // [register offsets](file://spec.pdf#page=136)
// //
// // [register functions](file://spec.pdf#page=106)
// 
// struct __attribute__((packed)) RCC_config 
// {
// 	volatile u32 clock_control;
// 	volatile u32 PLL_config;
// 	volatile u32 clock_config;
// 	volatile u32 interrupt;
// 	volatile u32 AHB1_peripheral_reset;
// 	volatile u32 AHB2_peripheral_reset;
// 	volatile u32 AHB3_peripheral_reset;
// 		 u32 RESERVED0;
// 	volatile u32 APB1_peripheral_reset;
// 	volatile u32 APB2_peripheral_reset;
// 		 u32 RESERVED1[2];
// 	volatile u32 AHB1_peripheral_enable;
// 	volatile u32 AHB2_peripheral_enable;
// 	volatile u32 AHB3_peripheral_enable;
// 		 u32 RESERVED2;
// 	volatile u32 APB1_peripheral_enable;
// 	volatile u32 APB2_peripheral_enable;
// 		 u32 RESERVED3[2];
// 	volatile u32 AHB1_low_power_enable;
// 	volatile u32 AHB2_low_power_enable;
// 	volatile u32 AHB3_low_power_enable;
// 		 u32 RESERVED4;
// 	volatile u32 APB1_low_power_enable;
// 	volatile u32 APB2_low_power_enable;
// 		 u32 RESERVED5[2];
// 	volatile u32 backup_domain;
// 	volatile u32 clock_control_status;
// 		 u32 RESERVED6[2];
// 	volatile u32 spread_spectrum_clock_generation;
// 	volatile u32 PLLI2S_config;
// 		 u32 RESERVED7[1];
// 	volatile u32 dedicated_clocks_config;
// };
// 
// enum RCC_CONTROL : u32
// {
// 	RCC_CONTROL__PLLI2S_READY		= 1UL << 27UL,
// 	RCC_CONTROL__PLLI2S_ENABLE		= 1UL << 26UL,
// 	RCC_CONTROL__PLL_READY 			= 1UL << 25UL,
// 	RCC_CONTROL__PLL_ENABLE 		= 1UL << 24UL,
// 	RCC_CONTROL__CSS_ENABLE			= 1UL << 19UL,
// 	RCC_CONTROL__HSE_BYPASS			= 1UL << 18UL,
// 	RCC_CONTROL__HSE_READY			= 1UL << 17UL,
// 	RCC_CONTROL__HSE_ENABLE			= 1UL << 16UL,
// 	RCC_CONTROL__HSI_CALIBRATION_SHIFT	= 8,
// #define RCC_CONTROL__HSE_CALIBRATION(n)		(((n) & 0xFF) << RCC_CONTROL__HSI_CALIBRATION_SHIFT)
// #define RCC_CONTROL__HSE_CALIBRATION_GET(mask)	(((mask) >> RCC_CONTROL__HSI_CALIBRATION_SHIFT) & 0xFF)
// 	RCC_CONTROL__HSI_TRIM_SHIFT		= 4,
// #define RCC_CONTROL__HSE_TRIM(n)		(((n) & 0x1F) << RCC_CONTROL__HSI_TRIM_SHIFT)
// #define RCC_CONTROL__HSE_TRIM_GET(mask)		(((mask) >> RCC_CONTROL__HSI_TRIM_SHIFT) & 0x1F)
// 	RCC_CONTROL__HSI_READY			= 1ULL << 1UL,
// 	RCC_CONTROL__HSI_ENABLE			= 1ULL << 0UL,
// };
// 
// enum RCC_AHB1_PERIPHERAL_ENABLE : u32
// {
// 	RCC_AHB1_PERIPHERAL_ENABLE__DMA2	= 1UL << 22UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__DMA1	= 1UL << 21UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__CRC		= 1UL << 12UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOH	= 1UL <<  7UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOE	= 1UL <<  4UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOD	= 1UL <<  3UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOC	= 1UL <<  2UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOB	= 1UL <<  1UL,
// 	RCC_AHB1_PERIPHERAL_ENABLE__GPIOA	= 1UL <<  0UL,
// };
// 
// enum RCC_APB1_PERIPHERAL_ENABLE : u32
// {
// 	RCC_APB1_PERIPHERAL_ENABLE__PWR	 		= 1UL << 28UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__I2C3		= 1UL << 23UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__I2C2		= 1UL << 22UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__I2C1		= 1UL << 21UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__USART2		= 1UL << 17UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__SPI3		= 1UL << 15UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__SPI2		= 1UL << 14UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__WINDOW_WATCHDOG	= 1UL << 11UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__TIM5		= 1UL << 3UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__TIM4		= 1UL << 2UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__TIM3		= 1UL << 1UL,
// 	RCC_APB1_PERIPHERAL_ENABLE__TIM2		= 1UL << 0UL,
// };
// 
// struct __attribute__((packed)) PWR_config 
// {
// 	volatile u32 control;
// 	volatile u32 control_status;
// };
// 
// // =================[ FLASH ]===================
// //
// // [register offsets](file://spec.pdf#page=66)
// //
// // [register functions](file://spec.pdf#page=59)
// 
// struct __attribute__((packed)) FLASH_config
// {
// 	volatile u32 access_control;
// 	volatile u32 key;
// 	volatile u32 option_key;
// 	volatile u32 status;
// 	volatile u32 control;
// 	volatile u32 option_control;
// 	volatile u32 option_control_1;
// };
// 
// enum FLASH_ACCESS_CONTROL : u32
// {
// 	FLASH_ACCESS_CONTROL__DATA_CACHE	= 1UL << 10UL,
// 	FLASH_ACCESS_CONTROL__INSTRUCTION_CACHE = 1UL <<  9UL,
// 	FLASH_ACCESS_CONTROL__PREFETCH		= 1UL <<  8UL,
// 	FLASH_ACCESS_CONTROL__LATENCY_SHIFT	= 0UL,
// #define FLASH_ACCESS_CONTROL__LATENCY(n)	(((n) & 0xFUL) << FLASH_ACCESS_CONTROL__LATENCY_SHIFT)
// #define FLASH_ACCESS_CONTROL__LATENCY_GET(mask)	(((mask) >> FLASH_ACCESS_CONTROL__LATENCY_SHIFT) & 0xFUL)
// };
// 
// // TODO: document flag values for the other registers
// 
// // =================[ USART ]===================
// //
// // [register offsets](file://spec.pdf#page=557)
// // 
// // [register functions](file://spec.pdf#page=547)
// 
// struct __attribute__((packed)) USART_config
// {
// 	volatile u32 status;
// 	volatile u32 data;
// 	volatile u32 baud_rate;
// 	volatile u32 control1;
// 	volatile u32 control2;
// 	volatile u32 control3;
// 	volatile u32 guard_time_prescaler;
// };
// 
// // [status](file://spec.pdf#page=547)
// enum USART_STATUS : u32
// {
// 	USART_STATUS__CTS_LINE_DETECTED       = 1UL << 9UL,
// 	USART_STATUS__LIN_BREAK_DETECTED      = 1UL << 8UL,
// 	USART_STATUS__TRANSMIT_REGISTER_EMPTY = 1UL << 7UL,
// 	USART_STATUS__TRANSMISSION_COMPLETE   = 1UL << 6UL,
// 	USART_STATUS__READ_REGISTER_NOT_EMPTY = 1UL << 5UL,
// 	USART_STATUS__IDLE_LINE_DETECTED      = 1UL << 4UL,
// 	USART_STATUS__OVERRUN_ERROR_DETECTED  = 1UL << 3UL,
// 	USART_STATUS__NOISE_DETECTED          = 1UL << 2UL,
// 	USART_STATUS__FRAMING_ERROR_DETECTED  = 1UL << 1UL,
// 	USART_STATUS__PARITY_ERROR_DETECTED   = 1UL << 0UL,
// };
// 
// // [baud rate](file://spec.pdf#page=550)
// enum USART_BAUD_RATE : u32
// {
// 	USART_BAUD_RATE__MANTISSA_SHIFT     = 4,
// 	USART_BAUD_RATE__MANTISSA_BITS      = 12,
// #define USART_BAUD_RATE__MANTISSA(n)        (((n) & ((1UL << USART_BAUD_RATE__MANTISSA_BITS) - 1)) << USART_BAUD_RATE__MANTISSA_SHIFT)
// #define USART_BAUD_RATE__MANTISSA_GET(mask) (((mask) >> USART_BAUD_RATE__MANTISSA_SHIFT) & ((1UL << USART_BAUD_RATE__MANTISSA_BITS) - 1))
// 
// 	USART_BAUD_RATE__FRACTION_SHIFT     = 0,
// 	USART_BAUD_RATE__FRACTION_BITS      = 4,
// #define USART_BAUD_RATE__FRACTION(n)        (((n) & ((1UL << USART_BAUD_RATE__FRACTION_BITS) - 1)) << USART_BAUD_RATE__FRACTION_SHIFT)
// #define USART_BAUD_RATE__FRACTION_GET(mask) (((mask) >> USART_BAUD_RATE__FRACTION_SHIFT) & ((1UL << USART_BAUD_RATE__FRACTION_BITS) - 1))
// };
// 
// // [control 1](file://spec.pdf#page=550)
// enum USART_CONTROL1 : u32
// {
// 	USART_CONTROL1__8X_OVERSAMPLE                     = 1UL << 15UL,
// 	USART_CONTROL1__USART_ENABLE                      = 1UL << 13UL,
// 	USART_CONTROL1__9_BIT_WORD                        = 1UL << 12UL,
// 	USART_CONTROL1__WAKEUP_ADDRESS                    = 1UL << 11UL,
// 	USART_CONTROL1__PARITY_ENABLE                     = 1UL << 10UL,
// 	USART_CONTROL1__PARITY_ODD                        = 1UL <<  9UL,
// 	USART_CONTROL1__INTERRUPT_PARITY                  = 1UL <<  8UL,
// 	USART_CONTROL1__INTERRUPT_TRANSMIT_REGISTER_EMPTY = 1UL <<  7UL,
// 	USART_CONTROL1__INTERRUPT_TRANSMSSION_COMPLETE    = 1UL <<  6UL,
// 	USART_CONTROL1__INTERRUPT_READ_REGISTER_NOT_EMPTY = 1UL <<  5UL,
// 	USART_CONTROL1__INTERRUPT_IDLE_LINE_DETECTED      = 1UL <<  4UL,
// 	USART_CONTROL1__TRANSMITTER_ENABLE                = 1UL <<  3UL,
// 	USART_CONTROL1__RECEIVER_ENABLE                   = 1UL <<  2UL,
// 	USART_CONTROL1__RECEIVER_WAKEUP                   = 1UL <<  1UL,
// 	USART_CONTROL1__SEND_BREAK                        = 1UL <<  0UL,
// };
// 
// // [control 2](file://spec.pdf#page=553)
// enum USART_CONTROL2 : u32
// {
// 	USART_CONTROL2__LIN_MODE_ENABLE              = 1UL << 14UL,
// 	USART_CONTROL2__STOP_BITS_1                  = 0UL << 12UL,
// 	USART_CONTROL2__STOP_BITS_05	             = 1UL << 12UL,
// 	USART_CONTROL2__STOP_BITS_2                  = 2UL << 12UL,
// 	USART_CONTROL2__STOP_BITS_15                 = 3UL << 12UL,
// 	USART_CONTROL2__CLOCK_ENABLE                 = 1UL << 11UL,
// 	USART_CONTROL2__CLOCK_POLARITY_HIGH          = 1UL << 10UL,
// 	USART_CONTROL2__CLOCK_PHASE                  = 1UL <<  9UL,
// 	USART_CONTROL2__LAST_BIT_CLOCK_PULSE         = 1UL <<  8UL,
// 	USART_CONTROL2__INTERRUPT_LIN_BREAK_DETECTED = 1UL <<  6UL,
// 	USART_CONTROL2__LIN_BREAK_DETECTION_11_BIT   = 1UL <<  5UL,
// 	USART_CONTROL2__USART_NODE_ADDRESS_SHIFT     = 0UL,
// 	USART_CONTROL2__USART_NODE_ADDRESS_BITS      = 4UL,
// #define USART_CONTROL2__USART_NODE_ADDRESS(n)        = (((n) & ((1UL << USART_CONTROL2__USART_NODE_ADDRESS_BITS) - 1)) << USART_CONTROL2__USART_NODE_ADDRESS_SHIFT)
// #define USART_CONTROL2__USART_NODE_ADDRESS_GET(mask) = (((mask) >> USART_CONTROL2__USART_NODE_ADDRESS_SHIFT) & ((1UL << USART_CONTROL2__USART_NODE_ADDRESS_BITS) - 1))
// };
// 
// // [control 3](file://spec.pdf#page=554)
// enum USART_CONTROL3 : u32
// {
// 	USART_CONTROL3__ONE_SAMPLE_BIT_MODE              = 1UL << 11UL,
// 	USART_CONTROL3__INTERRUPT_CTS_LINE_DETECTED      = 1UL << 10UL,
// 	USART_CONTROL3__CTS_ENABLE                       = 1UL << 9UL,
// 	USART_CONTROL3__RTS_ENABLE                       = 1UL << 8UL,
// 	USART_CONTROL3__DMA_TRANSMIT_ENABLE              = 1UL << 7UL,
// 	USART_CONTROL3__DMA_RECEIVE_ENABLE               = 1UL << 6UL,
// 	USART_CONTROL3__SMARTCARD_MODE                   = 1UL << 5UL,
// 	USART_CONTROL3__SMARTCARD_NACK_MODE              = 1UL << 4UL,
// 	USART_CONTROL3__HALF_DUPLEX_MODE                 = 1UL << 3UL,
// 	USART_CONTROL3__IRDA_LOW_POWER                   = 1UL << 2UL,
// 	USART_CONTROL3__IRDA_ENABLE                      = 1UL << 1UL,
// 	USART_CONTROL3__INTERRUPT_FRAMING_ERROR_DETECTED = 1UL << 0UL,
// };
// 
// // [guard time/prescaler](file://spec.pdf#page=556)
// enum USART_GUARD_TIME_PRESCALER : u32
// {
// 	USART_GUARD_TIME_PRESCALER__GUARD_TIME_SHIFT     = 8UL,
// 	USART_GUARD_TIME_PRESCALER__GUARD_TIME_BITS      = 8UL,
// #define USART_GUARD_TIME_PRESCALER__GUARD_TIME(n)        (((n) & ((USART_GUARD_TIME_PRESCALER__GUARD_TIME_BITS) - 1)) << USART_GUARD_TIME_PRESCALER__GUARD_TIME_SHIFT)
// #define USART_GUARD_TIME_PRESCALER__GUARD_TIME_GET(mask) (((mask) >> USART_GUARD_TIME_PRESCALER__GUARD_TIME_SHIFT) & ((USART_GUARD_TIME_PRESCALER__GUARD_TIME_BITS) - 1))
// 
// 	USART_GUARD_TIME_PRESCALER__PRESCALER_SHIFT      = 0UL,
// 	USART_GUARD_TIME_PRESCALER__PRESCALER_BITS       = 8UL,
// #define USART_GUARD_TIME_PRESCALER__PRESCALER(n)         (((n) & ((USART_GUARD_TIME_PRESCALER__PRESCALER_BITS) - 1)) << USART_GUARD_TIME_PRESCALER__PRESCALER_SHIFT)
// #define USART_GUARD_TIME_PRESCALER__PRESCALER_GET(mask)  (((mask) >> USART_GUARD_TIME_PRESCALER__PRESCALER_SHIFT) & ((USART_GUARD_TIME_PRESCALER__PRESCALER_BITS) - 1))
// 
// };

#endif // BAREMETAL_H
