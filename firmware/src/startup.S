# =============================================================
# Startup RISC-V Bare-Metal
# Inicializa stack pointer e chama main()
# =============================================================

.section .text.start
.globl _start

_start:
    # 1. Inicializar stack pointer
    la sp, _stack_top

    # 2. Limpar registradores
    li a0, 0
    li a1, 0

    # 3. Chamar main
    call main

    # 4. Loop infinito (se main retornar)
1:
    j 1b