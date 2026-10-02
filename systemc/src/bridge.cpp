// =============================================================
// Bridge QEMU <-> SystemC (Implementacao - Multiplas conexoes)
// Sprint 4 - Gemeo Digital de Chip
// =============================================================
#include "bridge.h"
#include <iostream>

Bridge::Bridge(sc_module_name name) : sc_module(name), SOCKET_PATH("/tmp/qemu_systemc.sock") {
    init_socket();
    SC_THREAD(run_bridge);
    sensitive << clk.pos();
}

void Bridge::init_socket() {
    unlink(SOCKET_PATH);
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        SC_REPORT_ERROR("BRIDGE", "socket() falhou");
        return;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        SC_REPORT_ERROR("BRIDGE", "bind() falhou");
        return;
    }
    if (listen(server_fd, 5) < 0) {   // backlog = 5
        SC_REPORT_ERROR("BRIDGE", "listen() falhou");
        return;
    }
    SC_REPORT_INFO("BRIDGE", "Socket criado em /tmp/qemu_systemc.sock");
}

void Bridge::run_bridge() {
    std::cout << "[BRIDGE] Aguardando conexoes do QEMU..." << std::endl;

    while (true) {   // loop externo: aceita multiplas conexoes
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            SC_REPORT_ERROR("BRIDGE", "accept() falhou");
            continue;
        }
        std::cout << "[BRIDGE] Cliente conectado!" << std::endl;

        char buffer[256];
        while (true) {   // loop interno: processa comandos do cliente
            ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes <= 0) {
                std::cout << "[BRIDGE] Cliente desconectado." << std::endl;
                break;
            }
            buffer[bytes] = '\0';

            if (buffer[0] == 'W') {
                unsigned int addr, data;
                sscanf(buffer, "W%x %x", &addr, &data);
                std::cout << "[BRIDGE] Escrita: addr=0x" << std::hex << addr
                          << " data=0x" << data << std::dec << std::endl;
                gpio_in_out.write(data & 0xFF);
                const char* ack = "ACK\n";
                write(client_fd, ack, strlen(ack));
            }
            else if (buffer[0] == 'R') {
                unsigned int addr;
                sscanf(buffer, "R%x", &addr);
                std::cout << "[BRIDGE] Leitura: addr=0x" << std::hex << addr << std::dec << std::endl;
                unsigned int data = gpio_out_in.read().to_uint();
                char response[32];
                snprintf(response, sizeof(response), "0x%02X\n", data);
                write(client_fd, response, strlen(response));
            }
            wait();   // avanca tempo de simulacao
        }
        close(client_fd);
    }
}