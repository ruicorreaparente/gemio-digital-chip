/*
 * QEMU Gemio GPIO (MMIO)
 *
 * M1 / Sprint 5 — device model mapeado em 0x10010000 na máquina virt.
 * Registradores alinhados ao GPIO SystemC (DATA/DIR/CTRL).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 */

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "hw/misc/gemio_gpio.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "qemu/module.h"

#define GEMIO_GPIO_DATA  0x00
#define GEMIO_GPIO_DIR   0x04
#define GEMIO_GPIO_CTRL  0x08

static uint8_t gemio_gpio_effective_out(GemioGpioState *s)
{
    return s->reg_data & s->reg_dir;
}

static void gemio_gpio_update_irq(GemioGpioState *s)
{
    /* Bit 1 de CTRL habilita IRQ, igual ao modelo SystemC. */
    qemu_set_irq(s->irq, (s->reg_ctrl & 0x02) != 0);
}

static uint64_t gemio_gpio_read(void *opaque, hwaddr addr, unsigned size)
{
    GemioGpioState *s = opaque;
    uint32_t val = 0;

    switch (addr) {
    case GEMIO_GPIO_DATA:
        /* Saídas: lê o latch. Entradas: devolve o banco de pinos. */
        if (s->reg_dir == 0xFF) {
            val = s->reg_data;
        } else {
            val = s->in_pins;
        }
        break;
    case GEMIO_GPIO_DIR:
        val = s->reg_dir;
        break;
    case GEMIO_GPIO_CTRL:
        val = s->reg_ctrl;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "gemio_gpio: leitura inválida addr=0x%" HWADDR_PRIx "\n",
                      addr);
        return 0;
    }

    qemu_log("gemio_gpio: RD 0x%08x @ 0x10010000+0x%" HWADDR_PRIx
             " size=%u -> 0x%02x\n",
             (uint32_t)(GEMIO_GPIO_MMIO_BASE + addr), addr, size,
             val & 0xFF);
    return val;
}

static void gemio_gpio_write(void *opaque, hwaddr addr,
                             uint64_t val64, unsigned size)
{
    GemioGpioState *s = opaque;
    uint8_t val = val64 & 0xFF;

    switch (addr) {
    case GEMIO_GPIO_DATA:
        s->reg_data = val;
        qemu_log("gemio_gpio: WR DATA=0x%02x  out=0x%02x\n",
                 s->reg_data, gemio_gpio_effective_out(s));
        break;
    case GEMIO_GPIO_DIR:
        s->reg_dir = val;
        qemu_log("gemio_gpio: WR DIR=0x%02x\n", s->reg_dir);
        break;
    case GEMIO_GPIO_CTRL:
        s->reg_ctrl = val;
        qemu_log("gemio_gpio: WR CTRL=0x%02x irq=%d\n",
                 s->reg_ctrl, (s->reg_ctrl & 0x02) != 0);
        gemio_gpio_update_irq(s);
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "gemio_gpio: escrita inválida addr=0x%" HWADDR_PRIx
                      " val=0x%" PRIx64 "\n",
                      addr, val64);
        return;
    }
}

static const MemoryRegionOps gemio_gpio_ops = {
    .read = gemio_gpio_read,
    .write = gemio_gpio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
    .impl = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
};

static void gemio_gpio_reset(DeviceState *dev)
{
    GemioGpioState *s = GEMIO_GPIO(dev);

    s->reg_data = 0x00;
    s->reg_dir  = 0x00;
    s->reg_ctrl = 0x00;
    s->in_pins  = 0x00;
    qemu_set_irq(s->irq, 0);
}

static void gemio_gpio_init(Object *obj)
{
    GemioGpioState *s = GEMIO_GPIO(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);

    memory_region_init_io(&s->mmio, obj, &gemio_gpio_ops, s,
                          TYPE_GEMIO_GPIO, GEMIO_GPIO_MMIO_SIZE);
    sysbus_init_mmio(sbd, &s->mmio);
    sysbus_init_irq(sbd, &s->irq);
}

static void gemio_gpio_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = gemio_gpio_reset;
    dc->desc = "Gemio GPIO (8-bit MMIO)";
}

static const TypeInfo gemio_gpio_info = {
    .name = TYPE_GEMIO_GPIO,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(GemioGpioState),
    .instance_init = gemio_gpio_init,
    .class_init = gemio_gpio_class_init,
};

static void gemio_gpio_register_types(void)
{
    type_register_static(&gemio_gpio_info);
}

type_init(gemio_gpio_register_types)

DeviceState *gemio_gpio_create(hwaddr addr, qemu_irq irq)
{
    DeviceState *dev = qdev_new(TYPE_GEMIO_GPIO);

    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
    sysbus_connect_irq(SYS_BUS_DEVICE(dev), 0, irq);
    return dev;
}
