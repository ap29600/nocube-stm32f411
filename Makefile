CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump

FLAGS = -mcpu=cortex-m4 -nostdlib -mthumb -Wno-builtin-declaration-mismatch -s
OPT = -Os

BIN_DIR     = ./bin
SRC_DIR     = ./src
INCLUDE_DIR = ./include

SRC_FILES     = $(wildcard $(SRC_DIR)/*.c)
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.h)

.PHONY: default clean flash inspect

default: $(BIN_DIR)/bin.elf

clean:
	$(RM) $(BIN_DIR)/*

flash: $(BIN_DIR)/bin.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BIN_DIR)/bin.elf reset exit"

inspect: default
	$(OBJDUMP) -d $(BIN_DIR)/bin.elf -s -d --visualize-jumps=extended-color --disassembler-color=extended --headers | bat 

$(BIN_DIR)/bin.elf: $(SRC_FILES) $(INCLUDE_FILES) linker_script.ld
	$(CC) -o $@ $(SRC_FILES) -I $(INCLUDE_DIR) -T linker_script.ld $(FLAGS) $(OPT)
