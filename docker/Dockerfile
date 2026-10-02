# =============================================================
# Sprint 4 - Dockerfile com Toolchain RISC-V
# Base: Sprint 3 (SystemC + QEMU via APT)
# Adiciona: RISC-V bare-metal toolchain
# =============================================================

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# 1. Dependencias basicas + SystemC + QEMU + RISC-V toolchain
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    gcc \
    make \
    cmake \
    git \
    vim \
    nano \
    tree \
    sudo \
    wget \
    curl \
    ca-certificates \
    qemu-system-misc \
    libsystemc-dev \
    gcc-riscv64-unknown-elf \
    binutils-riscv64-unknown-elf \
    picolibc-riscv64-unknown-elf \
    && rm -rf /var/lib/apt/lists/*

# 2. Variaveis de ambiente
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
ENV RISCV_PREFIX=riscv64-unknown-elf-

WORKDIR /workspace
CMD ["/bin/bash"]