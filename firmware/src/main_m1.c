/*
 * Firmware M1 (Dia 4) — prova de vida do Gemio GPIO em 0x10010000
 *
 * Sequencia:
 *   1. DIR  = 0xFF
 *   2. DATA = 0xAA
 *   3. Le DATA e compara
 *   4. Escreve "OK\n" ou "FAIL\n" na UART 0x10000000
 *   5. Termina a simulacao via SiFive Test (0x00100000)
 */

#define GPIO_BASE   0x10010000UL
#define GPIO_DATA   (GPIO_BASE + 0x00)
#define GPIO_DIR    (GPIO_BASE + 0x04)
#define GPIO_CTRL   (GPIO_BASE + 0x08)

#define UART_BASE   0x10000000UL
#define TEST_BASE   0x00100000UL
#define FINISHER_PASS  0x5555
#define FINISHER_FAIL  0x3333

static inline void reg_write(unsigned long addr, unsigned int value)
{
    *(volatile unsigned int *)addr = value;
}

static inline unsigned int reg_read(unsigned long addr)
{
    return *(volatile unsigned int *)addr;
}

static void uart_putc(char c)
{
    reg_write(UART_BASE, (unsigned int)(unsigned char)c);
}

static void uart_puts(const char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}

void main(void)
{
    unsigned int got;

    reg_write(GPIO_DIR, 0xFF);
    reg_write(GPIO_DATA, 0xAA);
    got = reg_read(GPIO_DATA) & 0xFF;

    if (got == 0xAA) {
        uart_puts("OK\n");
        reg_write(TEST_BASE, FINISHER_PASS);
    } else {
        uart_puts("FAIL\n");
        reg_write(TEST_BASE, FINISHER_FAIL | (got << 16));
    }

    while (1) {
    }
}
