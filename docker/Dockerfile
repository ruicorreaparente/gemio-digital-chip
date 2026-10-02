# =============================================================
# Sprint 2 - Dockerfile com SystemC 3.0.2
# Base: Ubuntu 22.04
# Adiciona: SystemC 3.0.2 (compilado do fonte)
# =============================================================

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Sao_Paulo

# 1. Dependencias do Sprint 1 + download
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
    && rm -rf /var/lib/apt/lists/*

# 2. SystemC 3.0.2
ENV SYSTEMC_HOME=/opt/systemc
ENV SYSTEMC_VERSION=3.0.2

WORKDIR /tmp
RUN wget -q https://www.accellera.org/images/downloads/standards/systemc/systemc-${SYSTEMC_VERSION}.tar.gz \
    && tar -xzf systemc-${SYSTEMC_VERSION}.tar.gz \
    && cd systemc-${SYSTEMC_VERSION} \
    && mkdir -p build && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=${SYSTEMC_HOME} -DCMAKE_CXX_STANDARD=17 \
    && make -j$(nproc) \
    && make install \
    && cd /tmp \
    && rm -rf systemc-${SYSTEMC_VERSION}* \
    && echo "export SYSTEMC_HOME=${SYSTEMC_HOME}" >> /etc/profile.d/systemc.sh \
    && echo "export LD_LIBRARY_PATH=${SYSTEMC_HOME}/lib-linux64:\$LD_LIBRARY_PATH" >> /etc/profile.d/systemc.sh \
    && echo "export CMAKE_PREFIX_PATH=${SYSTEMC_HOME}:\$CMAKE_PREFIX_PATH" >> /etc/profile.d/systemc.sh

# 3. Usuario nao-root
ARG USERNAME=developer
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo "$USERNAME ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

WORKDIR /workspace
RUN chown -R $USERNAME:$USERNAME /workspace

USER $USERNAME

CMD ["/bin/bash"]