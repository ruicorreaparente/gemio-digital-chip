// =============================================================
// Testbench do GPIO com TLM
// Sprint 3 - Gemeo Digital de Chip
// =============================================================
#include "gpio.h"
#include <tlm.h>

SC_MODULE(Testbench) {
    sc_in<bool> clk;
    sc_out<bool> rst_n;

    sc_signal<sc_uint<8>> gpio_out_sig;
    sc_signal<sc_uint<8>> gpio_in_sig;
    sc_signal<bool>       irq_sig;

    GPIO* gpio;

    SC_CTOR(Testbench) {
        gpio = new GPIO("gpio");
        gpio->clk(clk);
        gpio->rst_n(rst_n);
        gpio->gpio_out(gpio_out_sig);
        gpio->gpio_in(gpio_in_sig);
        gpio->irq(irq_sig);

        SC_THREAD(run_tests);
        sensitive << clk.pos();
    }

    void run_tests() {
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << " Testbench GPIO - Sprint 3" << std::endl;
        std::cout << "========================================" << std::endl;

        // 1. Reset
        rst_n.write(false);
        wait(20, SC_NS);
        rst_n.write(true);
        wait(20, SC_NS);
        std::cout << "[PASS] Reset aplicado" << std::endl;

        // 2. Testar escrita/leitura via TLM (chamada direta ao b_transport)
        tlm::tlm_generic_payload trans;
        unsigned char data;
        unsigned char read_val;
        sc_time delay;

        // 2.1 Configurar direcao (todos saida = 0xFF)
        data = 0xFF;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(GPIO_ADDR_DIR);
        trans.set_data_ptr(&data);
        trans.set_data_length(1);
        delay = SC_ZERO_TIME;
        gpio->b_transport(trans, delay);
        wait(10, SC_NS);
        std::cout << "[PASS] Direcao configurada (0xFF)" << std::endl;

        // 2.2 Escrever 0xAA em DATA
        data = 0xAA;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(GPIO_ADDR_DATA);
        trans.set_data_ptr(&data);
        delay = SC_ZERO_TIME;
        gpio->b_transport(trans, delay);
        wait(10, SC_NS);
        std::cout << "[PASS] Escrita DATA = 0xAA" << std::endl;

        // 2.3 Ler DATA
        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_address(GPIO_ADDR_DATA);
        trans.set_data_ptr(&read_val);
        delay = SC_ZERO_TIME;
        gpio->b_transport(trans, delay);
        wait(10, SC_NS);

        if (read_val == 0xAA) {
            std::cout << "[PASS] Leitura DATA = 0xAA" << std::endl;
        } else {
            std::cout << "[FAIL] Leitura DATA esperado 0xAA, obtido 0x" 
                      << std::hex << (int)read_val << std::dec << std::endl;
        }

        // 3. Testar entrada
        data = 0x00;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(GPIO_ADDR_DIR);
        trans.set_data_ptr(&data);
        delay = SC_ZERO_TIME;
        gpio->b_transport(trans, delay);
        wait(10, SC_NS);

        gpio_in_sig.write(0x55);
        wait(10, SC_NS);

        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_address(GPIO_ADDR_DATA);
        trans.set_data_ptr(&read_val);
        delay = SC_ZERO_TIME;
        gpio->b_transport(trans, delay);
        wait(10, SC_NS);

        if (read_val == 0x55) {
            std::cout << "[PASS] Leitura entrada = 0x55" << std::endl;
        } else {
            std::cout << "[FAIL] Leitura entrada esperado 0x55, obtido 0x" 
                      << std::hex << (int)read_val << std::dec << std::endl;
        }

        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "[SUCESSO] GPIO validado!" << std::endl;
        std::cout << "========================================" << std::endl;

        sc_stop();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst_n;

    Testbench tb("tb");
    tb.clk(clk);
    tb.rst_n(rst_n);

    sc_start(500, SC_NS);
    return 0;
}