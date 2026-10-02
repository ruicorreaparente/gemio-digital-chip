// =============================================================
// Bridge QEMU <-> SystemC (Header)
// Sprint 4 - Gemeo Digital de Chip
// =============================================================
#ifndef BRIDGE_H
#define BRIDGE_H

#include <systemc.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

SC_MODULE(Bridge) {
    sc_in<bool> clk;

    // Somente LEITURA da saida do GPIO
    sc_in<sc_uint<8>>  gpio_out_in;
    // Somente ESCRITA na entrada do GPIO
    sc_out<sc_uint<8>> gpio_in_out;
    sc_in<bool>        irq_in;

    int server_fd;
    int client_fd;
    const char* SOCKET_PATH;

    SC_CTOR(Bridge);
    void init_socket();
    void run_bridge();
};

#endif // BRIDGE_H