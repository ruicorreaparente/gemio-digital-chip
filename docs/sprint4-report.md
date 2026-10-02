# Relatorio Sprint 4 - Co-simulacao HW/SW Completa

**Data:** 02 de outubro de 2026
**Autor:** Rui Correa Parente
**Status:** CONCLUIDO

## 1. Objetivo

Implementar a co-simulacao entre firmware RISC-V (QEMU) e GPIO virtual (SystemC) via bridge com socket Unix.

## 2. Atividades Realizadas

1. Toolchain RISC-V via APT - OK
2. Firmware em C (main.c) - OK
3. Linker script (linker.ld) - OK
4. Firmware compilado - OK
5. Firmware executando no QEMU - OK
6. Bridge com multiplas conexoes - OK
7. Co-simulacao validada - OK

## 3. Arquitetura Final

Firmware (C) -> QEMU (RISC-V) -> Bridge (socket Unix) -> GPIO (SystemC)

## 4. Conclusao

O Sprint 4 foi concluido com 100 por cento de sucesso. A co-simulacao HW/SW esta funcional. MVP 100 por cento concluido.

---

Rui Correa Parente
Engenheiro RTL Certificado Synopsys
ruicorreaparente@gmail.com
