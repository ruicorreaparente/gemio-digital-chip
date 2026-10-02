// =============================================================
// GPIO - Implementacao (SystemC + TLM 2.0)
// Sprint 2 - Gemeo Digital de Chip
// =============================================================
#include "gpio.h"

SC_CTOR(GPIO) : socket("socket") {
    // Inicializar registradores
    reg_data = 0x00;
    reg_dir  = 0x00;
    reg_ctrl = 0x00;

    // Metodo de reset
    SC_METHOD(reset);
    sensitive << rst_n.neg();

    // Metodo de atualizacao de saidas
    SC_METHOD(update_outputs);
    sensitive << clk.pos();

    // Registrar callback TLM
    socket.register_b_transport(this, &GPIO::b_transport);

    SC_REPORT_INFO("GPIO", "Modulo GPIO inicializado");
}

void GPIO::reset() {
    reg_data = 0x00;
    reg_dir  = 0x00;
    reg_ctrl = 0x00;
    gpio_out.write(0x00);
    irq.write(false);
    SC_REPORT_INFO("GPIO", "Reset aplicado");
}

void GPIO::update_outputs() {
    sc_uint<8> output_val = 0;

    // Aplicar apenas bits configurados como saida
    for (int i = 0; i < 8; i++) {
        if (reg_dir[i] == 1) {
            output_val[i] = reg_data[i];
        }
    }

    gpio_out.write(output_val);

    // Verificar IRQ
    if (reg_ctrl[1] == 1) {
        irq.write(true);
    } else {
        irq.write(false);
    }
}

void GPIO::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    addr = trans.get_address();
    unsigned char*   ptr  = trans.get_data_ptr();
    unsigned int     len  = trans.get_data_length();

    if (cmd == tlm::TLM_WRITE_COMMAND) {
        // ESCrita
        switch (addr) {
            case 0x00: // GPIO_DATA
                reg_data = *ptr;
                SC_REPORT_INFO("GPIO", ("Escrita DATA = 0x" + 
                    std::string(sc_dt::sc_uint<8>(*ptr).to_string(sc_dt::SC_HEX))).c_str());
                break;
            case 0x04: // GPIO_DIR
                reg_dir = *ptr;
                SC_REPORT_INFO("GPIO", ("Escrita DIR = 0x" + 
                    std::string(sc_dt::sc_uint<8>(*ptr).to_string(sc_dt::SC_HEX))).c_str());
                break;
            case 0x08: // GPIO_CTRL
                reg_ctrl = *ptr;
                SC_REPORT_INFO("GPIO", ("Escrita CTRL = 0x" + 
                    std::string(sc_dt::sc_uint<8>(*ptr).to_string(sc_dt::SC_HEX))).c_str());
                break;
            default:
                SC_REPORT_WARNING("GPIO", "Endereco de escrita invalido");
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                return;
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    } 
    else if (cmd == tlm::TLM_READ_COMMAND) {
        // Leitura
        switch (addr) {
            case 0x00: // GPIO_DATA
                // Se direcao = saida, retorna reg_data; senao, retorna gpio_in
                if (reg_dir == 0xFF) {
                    *ptr = reg_data.to_uint();
                } else {
                    *ptr = gpio_in.read().to_uint();
                }
                break;
            case 0x04: // GPIO_DIR
                *ptr = reg_dir.to_uint();
                break;
            case 0x08: // GPIO_CTRL
                *ptr = reg_ctrl.to_uint();
                break;
            default:
                SC_REPORT_WARNING("GPIO", "Endereco de leitura invalido");
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                return;
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    } 
    else {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
    }

    delay += sc_time(10, SC_NS); // Latencia simulada
}