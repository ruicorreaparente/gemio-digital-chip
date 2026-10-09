# Changelog

## [Sprint 4] - 2026-10-02

### Adicionado
- Toolchain RISC-V bare-metal (riscv64-unknown-elf-gcc)
- Firmware em C (main.c) com acesso ao GPIO
- Linker script para RISC-V (linker.ld)
- Makefile do firmware
- Bridge QEMU <-> SystemC com multiplas conexoes
- Script Python de simulacao (bridge_sim.py)

### Validado
- Firmware compilado: firmware.elf (4828 bytes)
- Firmware executa no QEMU sem erros
- Bridge via socket Unix
- Multiplas conexoes sequenciais
- Escrita e leitura do GPIO via bridge
- Co-simulacao HW/SW funcional

## [Sprint 3] - 2026-10-02

### Validado
- GPIO com 5 testes passando

## [Sprint 2] - 2026-10-02

### Adicionado
- SystemC + Modelo GPIO com TLM 2.0

## [Sprint 1] - 2026-09-28

### Adicionado
- Dockerfile base + Hello World
## [Sprint 5 - M1] - 2026-10-08

### Marco M1: Device MMIO QEMU validado

- QEMU 8.2 compilado do fonte com device customizado
- Device gemio-gpio integrado em 0x10010000
- Firmware M1 compilado com startup.S (stack pointer)
- Teste final: WR DIR=0xff, WR DATA=0xaa, RD -> 0xaa, OK

### Correcoes
- -mcmodel=medany para resolver relocation truncada
- startup.S inicializando stack pointer (_stack_top)
- linker.ld com ENTRY(_start) e .text.start
