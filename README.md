# Gemeo Digital de Chip

Plataforma de co-simulacao HW/SW com SystemC e QEMU para validacao pre-silicon.

## Sobre o Projeto

O Gemeo Digital de Chip e uma plataforma open-source que permite testar firmware antes do tape-out, reduzindo custos e acelerando o time-to-market.

### O Problema

Times de hardware (RTL) e software (firmware) trabalham em silos isolados. O software so pode ser testado apos o retorno do chip fisico (6 a 12 meses).

### A Solucao

Um ambiente de co-simulacao onde o firmware roda sobre um modelo de hardware, permitindo testes desde o primeiro dia do projeto.

## Arquitetura

Firmware (C) - QEMU (RISC-V) - Bridge (socket Unix) - GPIO (SystemC)

## Tecnologias

| Tecnologia | Versao | Uso |
|------------|--------|-----|
| Docker | 29.8.1 | Ambiente reproduzivel |
| SystemC | 2.3.3 | Modelagem de hardware |
| QEMU | 8.2.0 | Emulacao RISC-V |
| RISC-V GCC | 10.2.0 | Toolchain bare-metal |
| CMake | 3.16+ | Build system |

## Resultados Validados

### Testes do GPIO - Sprint 3

5 testes passaram: Reset, Direcao, Escrita DATA=0xAA, Leitura DATA=0xAA, Leitura entrada=0x55.

### Co-simulacao - Sprint 4

Bridge via socket Unix processou comandos de escrita e leitura do GPIO com sucesso.

## Roadmap

| Sprint | Descricao | Status |
|--------|-----------|--------|
| Sprint 1 | Docker + Toolchain | Concluido |
| Sprint 2 | SystemC + GPIO | Concluido |
| Sprint 3 | GPIO Validado | Concluido |
| Sprint 4 | Co-simulacao HW/SW | Concluido |
| Sprint 5 | QEMU real com device model | Planejado |
| Sprint 6 | Interface Web | Planejado |

## Licenca

Este projeto esta sob a licenca MIT. Veja o arquivo LICENSE.

## Autor

Rui Correa Parente

- Engenheiro RTL Certificado Synopsys
- Aluno Bolsista do Curso Especialista em Microeletronica - UFRGS
- Email: ruicorreaparente@gmail.com
- LinkedIn: https://www.linkedin.com/in/rcpanalistadedados/
- Website: https://ciexpert.softex.br

## Desenvolvimento com IA Agentica

Este projeto utiliza o [DIO Agent](https://github.com/digitalinnovationone/dio-agent) como mentor de aprendizado em tecnologia, integrado ao Cursor IDE.

