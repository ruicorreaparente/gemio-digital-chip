RISCV_PREFIX := riscv64-unknown-elf-
CC := $(RISCV_PREFIX)gcc
BUILD_DIR := build
QEMU ?= qemu-system-riscv64

.PHONY: all build m1 run-m1 clean

all: build

build:
	@mkdir -p $(BUILD_DIR)
	$(CC) -march=rv32i -mabi=ilp32 -nostdlib -nostartfiles -T src/linker.ld -o $(BUILD_DIR)/firmware.elf src/main.c
	@echo 'Firmware compilado: $(BUILD_DIR)/firmware.elf'

m1:
	@mkdir -p $(BUILD_DIR)
	$(CC) -march=rv64imac -mabi=lp64 -nostdlib -nostartfiles -T src/linker.ld \
		-o $(BUILD_DIR)/firmware_m1.elf src/main_m1.c
	@echo 'Firmware M1 compilado: $(BUILD_DIR)/firmware_m1.elf'

run-m1: m1
	$(QEMU) -M virt -nographic -bios none -kernel $(BUILD_DIR)/firmware_m1.elf

clean:
	@rm -rf $(BUILD_DIR)
