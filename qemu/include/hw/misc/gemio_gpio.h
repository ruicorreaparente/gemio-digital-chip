/*
 * QEMU Gemio GPIO — interface
 *
 * Copiar para include/hw/misc/gemio_gpio.h na árvore QEMU 8.2.
 */

#ifndef HW_MISC_GEMIO_GPIO_H
#define HW_MISC_GEMIO_GPIO_H

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_GEMIO_GPIO "gemio-gpio"

typedef struct GemioGpioState GemioGpioState;
DECLARE_INSTANCE_CHECKER(GemioGpioState, GEMIO_GPIO, TYPE_GEMIO_GPIO)

#define GEMIO_GPIO_MMIO_BASE  0x10010000ULL
#define GEMIO_GPIO_MMIO_SIZE  0x1000

struct GemioGpioState {
    SysBusDevice parent_obj;

    MemoryRegion mmio;
    qemu_irq irq;

    uint8_t reg_data;
    uint8_t reg_dir;
    uint8_t reg_ctrl;
    uint8_t in_pins;
};

DeviceState *gemio_gpio_create(hwaddr addr, qemu_irq irq);

#endif
