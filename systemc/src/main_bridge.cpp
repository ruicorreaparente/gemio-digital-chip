// =============================================================
// Testbench Integrado com Bridge (Sprint 4)
// =============================================================
#include "gpio.h"
#include "bridge.h"

SC_MODULE(Testbench) {
    sc_in<bool> clk;

    sc_signal<bool>       rst_n;
    sc_signal<sc_uint<8>> gpio_out_sig;
    sc_signal<sc_uint<8>> gpio_in_sig;
    sc_signal<bool>       irq_sig;

    GPIO* gpio;
    Bridge* bridge;

    SC_CTOR(Testbench) {
        gpio = new GPIO("gpio");
        gpio->clk(clk);
        gpio->rst_n(rst_n);
        gpio->gpio_out(gpio_out_sig);
        gpio->gpio_in(gpio_in_sig);
        gpio->irq(irq_sig);

        bridge = new Bridge("bridge");
        bridge->clk(clk);
        bridge->gpio_out_in(gpio_out_sig);
        bridge->gpio_in_out(gpio_in_sig);
        bridge->irq_in(irq_sig);

        SC_THREAD(run_tests);
        sensitive << clk.pos();
    }

    void run_tests() {
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << " Bridge ativa - aguardando conexao" << std::endl;
        std::cout << "========================================" << std::endl;

        rst_n.write(false);
        wait(20, SC_NS);
        rst_n.write(true);
        wait(20, SC_NS);

        std::cout << "[PASS] Reset aplicado" << std::endl;
        std::cout << "[BRIDGE] Aguardando conexao externa..." << std::endl;

        while (true) {
            wait(100, SC_NS);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS);

    Testbench tb("tb");
    tb.clk(clk);

    sc_start(5000, SC_NS);
    return 0;
}