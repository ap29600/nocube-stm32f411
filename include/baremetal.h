#ifndef BAREMETAL_H
#define BAREMETAL_H

#include <inttypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "cmsis_ext.h"

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

#endif // BAREMETAL_H
