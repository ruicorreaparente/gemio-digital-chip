#!/usr/bin/env python3
# Patch virt.c para adicionar o device gemio-gpio

path = '/tmp/qemu-8.2/hw/riscv/virt.c'
with open(path, 'r') as f:
    lines = f.readlines()

out = []
for line in lines:
    out.append(line)
    # Patch 6: include
    if '#include "qapi/qapi-visit-common.h"' in line:
        out.append('#include "hw/misc/gemio_gpio.h"\n')
    # Patch 7: memmap entry
    if '[VIRT_PCIE_ECAM]' in line and '{' in line:
        out.append('    [VIRT_GEMIO_GPIO] =   { 0x10010000,         0x1000 },\n')
    # Patch 8: gemio_gpio_create no init
    if 'serial_hd(0), DEVICE_LITTLE_ENDIAN);' in line:
        out.append('\n')
        out.append('    /* Gemio GPIO */\n')
        out.append('    gemio_gpio_create(memmap[VIRT_GEMIO_GPIO].base,\n')
        out.append('                      qdev_get_gpio_in(mmio_irqchip, GEMIO_GPIO_IRQ));\n')

with open(path, 'w') as f:
    f.writelines(out)

print('[6/8] Include adicionado')
print('[7/8] Memmap entry adicionado')
print('[8/8] gemio_gpio_create adicionado')
print()
print('=== VERIFICACAO ===')