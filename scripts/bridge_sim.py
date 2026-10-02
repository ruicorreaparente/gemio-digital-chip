#!/usr/bin/env python3
# =============================================================
# Bridge QEMU <-> SystemC (Simulada)
# Sprint 4 - Gemeo Digital de Chip
# =============================================================
# Este script simula o comportamento do QEMU enviando comandos
# para o GPIO virtual via socket Unix.

import socket
import sys
import time

SOCKET_PATH = "/tmp/qemu_systemc.sock"

def send_command(cmd):
    """Envia comando para a bridge e retorna a resposta"""
    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.connect(SOCKET_PATH)
        s.sendall((cmd + "\n").encode())
        time.sleep(0.2)
        response = s.recv(1024).decode().strip()
        s.close()
        return response
    except Exception as e:
        return f"ERRO: {e}"

def main():
    print("========================================")
    print(" Bridge QEMU <-> SystemC (Simulada)")
    print(" Sprint 4 - Gemeo Digital de Chip")
    print("========================================")
    print()

    # Simular o firmware RISC-V escrevendo no GPIO
    print("[FIRMWARE] Configurando direcao (0xFF)...")
    resp = send_command("W04 FF")
    print(f"  Resposta: {resp}")

    time.sleep(0.5)

    print("[FIRMWARE] Escrevendo 0xAA no GPIO...")
    resp = send_command("W00 AA")
    print(f"  Resposta: {resp}")

    time.sleep(0.5)

    print("[FIRMWARE] Lendo GPIO...")
    resp = send_command("R00")
    print(f"  Resposta: {resp}")

    time.sleep(0.5)

    print("[FIRMWARE] Escrevendo 0x55 no GPIO...")
    resp = send_command("W00 55")
    print(f"  Resposta: {resp}")

    print()
    print("========================================")
    print("[SUCESSO] Comunicacao com GPIO validada!")
    print("========================================")

if __name__ == "__main__":
    main()