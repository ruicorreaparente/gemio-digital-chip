#include <systemc.h>
#include "socket_server.h"

SC_MODULE(Testbench) {
    sc_clock clk;
    SocketServer* server;

    SC_CTOR(Testbench) : clk("clk", 10, SC_NS) {
        server = new SocketServer("server");
        server->clk(clk);

        SC_THREAD(run_test);
    }

    void run_test() {
        std::cout << "========================================" << std::endl;
        std::cout << " Socket Server Test - Etapa 2" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Aguardando conexao do QEMU..." << std::endl;

        // Manter simulacao rodando
        while (true) {
            wait(100, SC_NS);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    Testbench tb("tb");
    sc_start(5000, SC_NS);
    return 0;
}