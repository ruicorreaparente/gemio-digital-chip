# Gêmeo Digital de Chip

## Integração Hardware/Software com SystemC e QEMU

**Autor:** Rui Correa Parente (Eng. RTL Certificado Synopsys)

---

## 📋 Status do Projeto

| Sprint | Descrição | Status |
|--------|-----------|--------|
| **Sprint 1** | Docker + Toolchain mínima | ✅ **CONCLUÍDO** |
| Sprint 2 | SystemC + Modelo GPIO | ⏳ Próximo |
| Sprint 3 | Bridge TLM 2.0 + QEMU | ⏳ Pendente |
| Sprint 4 | Firmware bare-metal + Co-simulação | ⏳ Pendente |

---

## 🎯 Sobre o Projeto

Este projeto implementa um ambiente de co-simulação para teste de integração hardware-software, utilizando:

- **SystemC/TLM 2.0** para modelagem de hardware em alto nível
- **QEMU** para emulação do processador RISC-V
- **Bridge de comunicação** via sockets Unix
- **Firmware bare-metal** em C

O objetivo é permitir que testes de firmware comecem **antes do tape-out**, reduzindo custos e acelerando o desenvolvimento.

---

## 🏗️ Estrutura do Projeto

\\\
gemio-digital-chip/
├── docker/                 # Dockerfile e scripts de container
│   └── Dockerfile
├── examples/               # Exemplos de código
│   └── hello/
│       ├── hello.cpp
│       └── Makefile
├── scripts/                # Scripts de automação
├── docs/                   # Documentação
├── .dockerignore
└── README.md
\\\

---

## 🚀 Como Executar

### Pré-requisitos

- Docker Desktop (v29+)
- WSL2 (Windows Subsystem for Linux)
- Git

### Passo 1: Construir a imagem Docker

\\\powershell
cd D:\dev\gemio-digital-chip
docker build -t gemio-digital-chip:sprint1 -f docker/Dockerfile .
\\\

### Passo 2: Executar o container

\\\powershell
docker run --rm -it -v \D:\dev\gemio-digital-chip:/workspace gemio-digital-chip:sprint1
\\\

### Passo 3: Executar o exemplo

Dentro do container:

\\\ash
cd examples/hello
make run
\\\

---

## ✅ Sprint 1 - Concluído

### O que foi validado

- [x] Docker Desktop instalado (v29.8.0)
- [x] WSL2 + Ubuntu configurados
- [x] Estrutura de pastas criada
- [x] Dockerfile mínimo funcional
- [x] Imagem Docker construída com sucesso
- [x] Container executando
- [x] GCC compilando dentro do container
- [x] Make automatizando o build
- [x] Pipeline validado end-to-end

### Saída esperada do Sprint 1

\\\
==========================================
  Gemeo Digital de Chip - Sprint 1
==========================================
  [OK] Docker funcionando!
  [OK] GCC compilando!
  [OK] Ambiente EDA pronto para o Sprint 2!

[SUCESSO] Pipeline validado!
\\\

---

## 📞 Contato

**Rui Correa Parente**
- E-mail: ruicorreaparente@gmail.com
- Celular: (41) 98809-0502
- LinkedIn: https://www.linkedin.com/in/rcpanalistadedados/

---

*Documento assinado digitalmente - Verifique em https://validar.iti.gov.br*