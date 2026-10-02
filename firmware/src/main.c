// =============================================================
// Firmware Bare-Metal RISC-V
// Sprint 4 - Gemeo Digital de Chip
// =============================================================
#define GPIO_BASE  0x10000000
#define GPIO_DATA  (GPIO_BASE + 0x00)
#define GPIO_DIR   (GPIO_BASE + 0x04)
#define GPIO_CTRL  (GPIO_BASE + 0x08)

static inline void reg_write(unsigned int addr, unsigned int value) {
    *(volatile unsigned int*)addr = value;
}

static inline unsigned int reg_read(unsigned int addr) {
    return *(volatile unsigned int*)addr;
}

void main(void) {
    // 1. Configurar direcao (todos saida = 0xFF)
    reg_write(GPIO_DIR, 0xFF);

    // 2. Escrever padrao 0xAA no GPIO
    reg_write(GPIO_DATA, 0xAA);

    // 3. Loop infinito alternando padroes
    while (1) {
        reg_write(GPIO_DATA, 0xAA);
        for (volatile int i = 0; i < 1000; i++);

        reg_write(GPIO_DATA, 0x55);
        for (volatile int i = 0; i < 1000; i++);
    }
}