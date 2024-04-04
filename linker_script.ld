ENTRY(reset_handler_procedure)

MEMORY
{
  SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 128K
  FLASH (rx): ORIGIN = 0x08000000, LENGTH = 512K
}

__stack_end = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    __text_section_begin = .;
		
    *(.text)
    *(.text.*)

    . = ALIGN(4);
    __text_section_end = .;
    _etext = .;

    *(.comment)
    *(.comment.*)

  } >FLASH

  .comment :
  {
    . = ALIGN(4);

    *(.comment)
    *(.comment.*)

    . = ALIGN(4);
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);

    *(.rodata)
    *(.rodata.*)

    . = ALIGN(4);
  } >FLASH

  __flash_data_section_begin = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    __sram_data_section_begin = .;
    _sdata = .;
		
    *(.data)

    . = ALIGN(4);
    __sram_data_section_end = .;
    _edata = .;
  } >SRAM AT> FLASH

  .bss :
  {
    . = ALIGN(4);
    __bss_section_begin = .;
    _sbss = .;
		
    *(.bss)
    *(.bss.*)
		
    . = ALIGN(4);
    __bss_section_end = .;
    _ebss = .;
  } >SRAM
}
