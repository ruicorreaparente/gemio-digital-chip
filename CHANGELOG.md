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