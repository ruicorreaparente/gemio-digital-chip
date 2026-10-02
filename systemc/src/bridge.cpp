// =============================================================
// Bridge QEMU <-> SystemC (Implementacao CORRIGIDA)
// Sprint 3 - Gemeo Digital de Chip
// =============================================================
#include "bridge.h"
#include <iostream>
#include <cstring>
#include <cstdio>

Bridge::Bridge(sc_module_name name) : sc_module(name), SOCKET_PATH("/tmp/qemu_systemc.sock") {
    init_socket();
    SC_THREAD(run_bridge);
    sensitive << clk.pos();
    SC_REPORT_INFO("BRIDGE", "Bridge inicializada");
}

void Bridge::init_socket() {
    unlink(SOCKET_PATH);
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        SC_REPORT_ERROR("BRIDGE", "Falha ao criar socket");
        return;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        SC_REPORT_ERROR("BRIDGE", "Falha no bind");
        return;
    }
    if (listen(server_fd, 1) == -1) {
        SC_REPORT_ERROR("BRIDGE", "Falha no listen");
        return;
    }
    SC_REPORT_INFO("BRIDGE", ("Socket criado em " + std::string(SOCKET_PATH)).c_str());
}

void Bridge::run_bridge() {
    std::cout << "[BRIDGE] Aguardando conexao do QEMU..." << std::endl;
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        SC_REPORT_ERROR("BRIDGE", "Falha no accept");
        return;
    }
    std::cout << "[BRIDGE] QEMU conectado!" << std::endl;
    char buffer[256];
    while (true) {
        ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            std::cout << "[BRIDGE] QEMU desconectado." << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        if (buffer[0] == 'W') {
            unsigned int addr, data;
            sscanf(buffer, "W%x %x", &addr, &data);
            std::cout << "[BRIDGE] Escrita: addr=0x" << std::hex << addr
                      << " data=0x" << data << std::dec << std::endl;
            gpio_out.write(data & 0xFF);
            const char* ack = "ACK\n";
            write(client_fd, ack, strlen(ack));
        }
        else if (buffer[0] == 'R') {
            unsigned int addr;
            sscanf(buffer, "R%x", &addr);
            std::cout << "[BRIDGE] Leitura: addr=0x" << std::hex << addr << std::dec << std::endl;
            unsigned int data = gpio_in.read().to_uint();
            char response[32];
            snprintf(response, sizeof(response), "0x%02X\n", data);
            write(client_fd, response, strlen(response));
        }
        wait();
    }
    close(client_fd);
}