// =============================================================
// Bridge QEMU <-> SystemC (Header)
// Sprint 3 - Gemeo Digital de Chip
// =============================================================
#ifndef BRIDGE_H
#define BRIDGE_H

#include <systemc.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

SC_MODULE(Bridge) {
    sc_in<bool> clk;
    sc_out<sc_uint<8>> gpio_out;
    sc_in<sc_uint<8>>  gpio_in;
    sc_in<bool>        irq_in;

    int server_fd;
    int client_fd;
    const char* SOCKET_PATH;

    SC_CTOR(Bridge);
    void init_socket();
    void run_bridge();
};

#endif // BRIDGE_H