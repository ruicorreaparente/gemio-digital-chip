// =============================================================
// GPIO - Interface (SystemC + TLM 2.0)
// Sprint 2 - Gemeo Digital de Chip
// =============================================================
#ifndef GPIO_H
#define GPIO_H

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>

SC_MODULE(GPIO) {
    // Portas de clock e reset
    sc_in<bool> clk;
    sc_in<bool> rst_n;

    // Portas de I/O fisicas
    sc_out<sc_uint<8>> gpio_out;
    sc_in<sc_uint<8>>  gpio_in;
    sc_out<bool>       irq;

    // Socket TLM 2.0 (para comunicacao com QEMU)
    tlm_utils::simple_target_socket<GPIO> socket;

    // Registradores internos
    sc_uint<8> reg_data;
    sc_uint<8> reg_dir;
    sc_uint<8> reg_ctrl;

    // Enderecos dos registradores
    static const sc_uint<32> ADDR_DATA = 0x00;
    static const sc_uint<32> ADDR_DIR  = 0x04;
    static const sc_uint<32> ADDR_CTRL = 0x08;

    // Metodos
    void reset();
    void update_outputs();
    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);

    SC_CTOR(GPIO);
};

#endif // GPIO_H