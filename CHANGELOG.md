# Changelog

## [Sprint 3] - 2026-10-02

### Adicionado
- SystemC 2.3.3 instalado via APT (libsystemc-dev)
- QEMU RISC-V instalado via APT (qemu-system-misc)
- CMakeLists.txt ajustado para encontrar SystemC via APT
- GPIO em SystemC com interface TLM 2.0
- Testbench com 5 testes automatizados
- Makefile do SystemC

### Validado
- Compilacao sem erros
- 5 testes passando:
  * [PASS] Reset aplicado
  * [PASS] Direcao configurada (0xFF)
  * [PASS] Escrita DATA = 0xAA
  * [PASS] Leitura DATA = 0xAA
  * [PASS] Leitura entrada = 0x55
- Saida final: [SUCESSO] GPIO validado!

### Correcoes
- Removido socket TLM do GPIO (causava erro E109)
- reset() nao escreve mais em sinais (erro E115)
- main.cpp chama b_transport() diretamente

### Proximo Sprint
- Sprint 4: Firmware bare-metal + co-simulacao completa

## [Sprint 2] - 2026-10-02

### Adicionado
- SystemC no Dockerfile
- Modelo GPIO em SystemC com TLM 2.0

## [Sprint 1] - 2026-09-28

### Adicionado
- Dockerfile base com Ubuntu 22.04 + GCC + Make + CMake
- Exemplo Hello World em C++
- Pipeline Docker validado