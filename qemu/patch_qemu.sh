#!/bin/bash
QEMU=/tmp/qemu-8.2

echo '[1/5] meson.build'
grep -q gemio_gpio $QEMU/hw/misc/meson.build || echo "system_ss.add(when: 'CONFIG_GEMIO_GPIO', if_true: files('gemio_gpio.c'))" >> $QEMU/hw/misc/meson.build

echo '[2/5] hw/misc/Kconfig'
grep -q GEMIO_GPIO $QEMU/hw/misc/Kconfig || sed -i '/^source macio\/Kconfig/i config GEMIO_GPIO\n    bool\n' $QEMU/hw/misc/Kconfig

echo '[3/5] hw/riscv/Kconfig'
grep -q 'select GEMIO_GPIO' $QEMU/hw/riscv/Kconfig || sed -i '/select SIFIVE_PLIC/a\    select GEMIO_GPIO' $QEMU/hw/riscv/Kconfig

echo '[4/5] virt.h memmap'
grep -q VIRT_GEMIO_GPIO $QEMU/include/hw/riscv/virt.h || sed -i 's/    VIRT_PCIE_ECAM$/    VIRT_PCIE_ECAM,\n    VIRT_GEMIO_GPIO/' $QEMU/include/hw/riscv/virt.h

echo '[5/5] virt.h IRQ'
grep -q GEMIO_GPIO_IRQ $QEMU/include/hw/riscv/virt.h || sed -i 's/    RTC_IRQ = 11,$/    RTC_IRQ = 11,\n    GEMIO_GPIO_IRQ = 12,/' $QEMU/include/hw/riscv/virt.h

echo ''
echo '=== VERIFICACAO ==='
tail -3 $QEMU/hw/misc/meson.build
grep -A 2 'config GEMIO_GPIO' $QEMU/hw/misc/Kconfig
grep 'select GEMIO_GPIO' $QEMU/hw/riscv/Kconfig
grep -A 2 'VIRT_PCIE_ECAM' $QEMU/include/hw/riscv/virt.h | head -5
grep -A 2 'RTC_IRQ = 11' $QEMU/include/hw/riscv/virt.h | head -5
