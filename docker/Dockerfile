# =============================================================
# Sprint 3 - Dockerfile Final (SystemC e QEMU via APT)
# =============================================================
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# 1. Instala todas as dependências de uma vez, incluindo SystemC e QEMU.
#    O apt resolve as dependências automaticamente, evitando erros de download.
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
    && rm -rf /var/lib/apt/lists/*

# 2. Configuração de ambiente (as bibliotecas são instaladas no local padrão).
#    A flag -Wl,-rpath garante que o linker encontre as bibliotecas em tempo de execução.
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu

WORKDIR /workspace
CMD ["/bin/bash"]