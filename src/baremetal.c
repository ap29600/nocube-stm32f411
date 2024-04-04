#include "../include/baremetal.h"

void *memmove(void *destination, void const *source, size_t size) 
{
	byte *b_destination = destination;
	byte const *b_source = source;
	for (size_t i = 0; i < size; ++i)
	{
		b_destination[i] = b_source[i];
	}
	return destination;
}

void *memcpy(void *restrict destination, void const *source, size_t size) 
{
	return memmove(destination, source, size);
}

void *memset(void *destination, int b, size_t size) 
{
	byte *b_destination = destination;
	for (size_t i = 0; i < size; ++i) 
	{
		b_destination[i] = b;
	}
	return destination;
}

void reset_handler_procedure(void) 
{
	size_t const data_section_size = (uintptr_t)&__sram_data_section_end - (uintptr_t)&__sram_data_section_begin;
	memcpy(&__sram_data_section_begin, &__flash_data_section_begin, data_section_size);

	size_t const bss_section_size = (uintptr_t)&__bss_section_end - (uintptr_t)&__bss_section_begin;
	memset(&__bss_section_begin, 0, bss_section_size);

	main();
}

void default_handler_procedure(void) {
	while(1);
}

u32 interrupt_service_routine_vector[INTERRUPT_SERVICE_ROUTINE_VECTOR_LEN] __attribute__((section(".isr_vector"))) = {
	(u32)&__stack_end,
	(u32)&reset_handler_procedure,
	(u32)&default_handler_procedure,
	(u32)&default_handler_procedure,
	(u32)&default_handler_procedure,
	(u32)&default_handler_procedure,
	0,
	0,
	0,
	0,
	0,
	(u32)&default_handler_procedure,
	(u32)&default_handler_procedure,
	0,
	(u32)&default_handler_procedure,
	(u32)&default_handler_procedure,
}; 

