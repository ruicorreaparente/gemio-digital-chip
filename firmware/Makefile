RISCV_PREFIX := riscv64-unknown-elf-
CC := $(RISCV_PREFIX)gcc
BUILD_DIR := build

.PHONY: all build clean

all: build

build:
	@mkdir -p $(BUILD_DIR)
	$(CC) -march=rv32i -mabi=ilp32 -nostdlib -nostartfiles -T src/linker.ld -o $(BUILD_DIR)/firmware.elf src/main.c
	@echo 'Firmware compilado: $(BUILD_DIR)/firmware.elf'

clean:
	@rm -rf $(BUILD_DIR)
