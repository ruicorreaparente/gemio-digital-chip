// =============================================================
// GPIO - Implementacao (SystemC + TLM 2.0)
// Sprint 3 - Gemeo Digital de Chip
// =============================================================
#include "gpio.h"

GPIO::GPIO(sc_module_name name) : sc_module(name) {
    reg_data = 0x00;
    reg_dir  = 0x00;
    reg_ctrl = 0x00;

    SC_METHOD(reset);
    sensitive << rst_n.neg();

    SC_METHOD(update_outputs);
    sensitive << clk.pos();
}

// reset() APENAS atualiza registradores internos
// NAO escreve em sinais (update_outputs faz isso)
void GPIO::reset() {
    reg_data = 0x00;
    reg_dir  = 0x00;
    reg_ctrl = 0x00;
}

// update_outputs() e o UNICO driver dos sinais de saida
void GPIO::update_outputs() {
    sc_uint<8> output_val = 0;
    for (int i = 0; i < 8; i++) {
        if (reg_dir[i] == 1) {
            output_val[i] = reg_data[i];
        }
    }
    gpio_out.write(output_val);
    irq.write(reg_ctrl[1] == 1);
}

void GPIO::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    addr = trans.get_address();
    unsigned char*   ptr  = trans.get_data_ptr();

    if (cmd == tlm::TLM_WRITE_COMMAND) {
        switch (addr) {
            case GPIO_ADDR_DATA: reg_data = *ptr; break;
            case GPIO_ADDR_DIR:  reg_dir  = *ptr; break;
            case GPIO_ADDR_CTRL: reg_ctrl = *ptr; break;
            default:
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                return;
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
    else if (cmd == tlm::TLM_READ_COMMAND) {
        switch (addr) {
            case GPIO_ADDR_DATA:
                *ptr = (reg_dir == 0xFF) ? reg_data.to_uint() : gpio_in.read().to_uint();
                break;
            case GPIO_ADDR_DIR:  *ptr = reg_dir.to_uint();  break;
            case GPIO_ADDR_CTRL: *ptr = reg_ctrl.to_uint(); break;
            default:
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                return;
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
    else {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
    }

    delay += sc_time(10, SC_NS);
}