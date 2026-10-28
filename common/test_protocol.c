#include <stdio.h>
#include "dtwin_ipc_protocol.h"

int main(void) {
    printf("=== VALIDACAO DO PROTOCOLO ===\n");
    printf("sizeof(dtwin_req_packet_t)  = %zu bytes (esperado: 28)\n", sizeof(dtwin_req_packet_t));
    printf("sizeof(dtwin_resp_packet_t) = %zu bytes (esperado: 20)\n", sizeof(dtwin_resp_packet_t));
    printf("sizeof(uint64_t)            = %zu bytes\n", sizeof(uint64_t));
    printf("sizeof(uint32_t)            = %zu bytes\n", sizeof(uint32_t));

    if (sizeof(dtwin_req_packet_t) != 28) {
        printf("ERRO: req_packet deveria ter 28 bytes!\n");
        return 1;
    }
    if (sizeof(dtwin_resp_packet_t) != 20) {
        printf("ERRO: resp_packet deveria ter 20 bytes!\n");
        return 1;
    }

    printf("\n[OK] Protocolo validado!\n");
    return 0;
}