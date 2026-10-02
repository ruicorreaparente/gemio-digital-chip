// =============================================================
// GPIO - Interface (SystemC + TLM 2.0)
// Sprint 3 - Gemeo Digital de Chip
// =============================================================
#ifndef GPIO_H
#define GPIO_H

#include <systemc.h>
#include <tlm.h>

// Enderecos dos registradores
#define GPIO_ADDR_DATA  0x00
#define GPIO_ADDR_DIR   0x04
#define GPIO_ADDR_CTRL  0x08

SC_MODULE(GPIO) {
    // Portas
    sc_in<bool> clk;
    sc_in<bool> rst_n;
    sc_out<sc_uint<8>> gpio_out;
    sc_in<sc_uint<8>>  gpio_in;
    sc_out<bool>       irq;

    // Registradores internos
    sc_uint<8> reg_data;
    sc_uint<8> reg_dir;
    sc_uint<8> reg_ctrl;

    // Construtor
    SC_CTOR(GPIO);

    // Metodos
    void reset();
    void update_outputs();
    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);
};

#endif // GPIO_H