# M1 (Dia 4) — Gemio GPIO no QEMU 8.2 em 0x10010000

Endereço livre na `virt` 8.2: **0x10010000**, tamanho **0x1000**, IRQ **12**.

Por que este endereço:

| Região | Base | Tamanho | Fim |
|--------|------|---------|-----|
| UART0 | `0x10000000` | `0x100` | `0x10000100` |
| virtio-mmio × 8 | `0x10001000` | `8 × 0x1000` | `0x10009000` |
| **GEMIO GPIO (livre)** | **`0x10010000`** | **`0x1000`** | **`0x10011000`** |
| fw_cfg | `0x10100000` | `0x18` | `0x10100018` |

IRQ 12 fica entre RTC (11) e PCIe (32). Não colide com virtio 1–8 nem UART 10.

Os arquivos prontos para copiar estão em `qemu/hw/misc/gemio_gpio.c` e `qemu/include/hw/misc/gemio_gpio.h`.

---

## 1. Estrutura de `hw/misc/gemio_gpio.c`

Arquivo completo no repo. Layout interno:

```
gemio_gpio.c
├── gemio_gpio_read() / gemio_gpio_write()   # DATA 0x00, DIR 0x04, CTRL 0x08
├── gemio_gpio_ops                           # MMIO 32-bit little-endian
├── gemio_gpio_reset()                       # zera DATA/DIR/CTRL
├── gemio_gpio_init()                        # mmio + irq sysbus
├── gemio_gpio_class_init()
├── type_init(gemio_gpio_register_types)
└── gemio_gpio_create(addr, irq)             # helper chamado pela virt
```

Cada acesso imprime `gemio_gpio: WR/RD ...` no stderr do QEMU.

---

## 2. Lista exata de arquivos a patchar (QEMU v8.2.0)

Copiar (arquivos novos):

| Destino na árvore QEMU | Origem neste repo |
|------------------------|-------------------|
| `hw/misc/gemio_gpio.c` | `qemu/hw/misc/gemio_gpio.c` |
| `include/hw/misc/gemio_gpio.h` | `qemu/include/hw/misc/gemio_gpio.h` |

Editar (6 arquivos existentes):

1. `hw/misc/meson.build`
2. `hw/misc/Kconfig`
3. `hw/riscv/Kconfig`
4. `include/hw/riscv/virt.h`
5. `hw/riscv/virt.c`

Não precisa mexer em `default-configs/` se o `select GEMIO_GPIO` estiver em `RISCV_VIRT`.

### 2.1 `hw/misc/meson.build`

Depois do bloco RISC-V (`CONFIG_SIFIVE_U_PRCI`), inserir:

```meson
system_ss.add(when: 'CONFIG_GEMIO_GPIO', if_true: files('gemio_gpio.c'))
```

### 2.2 `hw/misc/Kconfig`

Junto dos outros SiFive:

```kconfig
config GEMIO_GPIO
    bool
```

### 2.3 `hw/riscv/Kconfig`

Dentro de `config RISCV_VIRT`, adicionar:

```kconfig
    select GEMIO_GPIO
```

### 2.4 `include/hw/riscv/virt.h`

No `enum` dos IDs do memmap, **no final** (não reordenar os existentes):

```c
    VIRT_PCIE_ECAM,
    VIRT_GEMIO_GPIO
```

No `enum` de IRQs:

```c
    UART0_IRQ = 10,
    RTC_IRQ = 11,
    GEMIO_GPIO_IRQ = 12,
    VIRTIO_IRQ = 1, /* 1 to 8 */
```

### 2.5 `hw/riscv/virt.c`

Include:

```c
#include "hw/misc/gemio_gpio.h"
```

Em `virt_memmap[]`:

```c
    [VIRT_VIRTIO]      = { 0x10001000,        0x1000 },
    [VIRT_GEMIO_GPIO]  = { 0x10010000,        0x1000 },
    [VIRT_FW_CFG]      = { 0x10100000,          0x18 },
```

Em `virt_machine_init()`, depois de `serial_mm_init(...)`:

```c
    gemio_gpio_create(memmap[VIRT_GEMIO_GPIO].base,
                      qdev_get_gpio_in(DEVICE(mmio_irqchip), GEMIO_GPIO_IRQ));
```

FDT (opcional no M1, recomendado). Nova função, no estilo de `create_fdt_rtc`:

```c
static void create_fdt_gemio_gpio(RISCVVirtState *s, const MemMapEntry *memmap,
                                  uint32_t irq_mmio_phandle)
{
    char *name;
    MachineState *ms = MACHINE(s);

    name = g_strdup_printf("/soc/gpio@%lx", (long)memmap[VIRT_GEMIO_GPIO].base);
    qemu_fdt_add_subnode(ms->fdt, name);
    qemu_fdt_setprop_string(ms->fdt, name, "compatible", "gemio,gpio");
    qemu_fdt_setprop_cells(ms->fdt, name, "reg",
                           0x0, memmap[VIRT_GEMIO_GPIO].base,
                           0x0, memmap[VIRT_GEMIO_GPIO].size);
    qemu_fdt_setprop_cell(ms->fdt, name, "interrupt-parent", irq_mmio_phandle);
    if (s->aia_type == VIRT_AIA_TYPE_NONE) {
        qemu_fdt_setprop_cell(ms->fdt, name, "interrupts", GEMIO_GPIO_IRQ);
    } else {
        qemu_fdt_setprop_cells(ms->fdt, name, "interrupts", GEMIO_GPIO_IRQ, 0x4);
    }
    g_free(name);
}
```

Em `finalize_fdt()`, depois de `create_fdt_rtc`:

```c
    create_fdt_gemio_gpio(s, virt_memmap, irq_mmio_phandle);
```

---

## 3. Build `riscv64-softmmu`

No Ubuntu 22.04 (Docker do projeto ou host):

```bash
sudo apt-get update
sudo apt-get install -y git ninja-build pkg-config python3 python3-venv \
    libglib2.0-dev libpixman-1-dev flex bison meson

git clone --depth 1 --branch v8.2.0 https://gitlab.com/qemu-project/qemu.git qemu-8.2
cd qemu-8.2
git submodule update --init --recursive --depth 1

# copiar device
cp /workspace/qemu/hw/misc/gemio_gpio.c hw/misc/
cp /workspace/qemu/include/hw/misc/gemio_gpio.h include/hw/misc/
# aplicar as 5 edições da seção 2

mkdir -p build
cd build
../configure --target-list=riscv64-softmmu --disable-werror
make -j"$(nproc)"

ls qemu-system-riscv64
```

`--disable-werror` evita que um warning do device novo derrube o build.

---

## 4. Firmware de teste (0x10010000)

Fonte: `firmware/src/main_m1.c`.

```bash
cd /workspace/firmware
riscv64-unknown-elf-gcc -march=rv64imac -mabi=lp64 \
    -nostdlib -nostartfiles -T src/linker.ld \
    -o build/firmware_m1.elf src/main_m1.c

/caminho/qemu-8.2/build/qemu-system-riscv64 \
    -M virt -nographic -bios none \
    -kernel build/firmware_m1.elf
```

Critério de sucesso:

- stderr do QEMU contém `gemio_gpio: WR DIR=0xff`, `WR DATA=0xaa`, `RD ... -> 0xaa`
- stdout UART imprime `OK`
- QEMU encerra via SiFive Test (`FINISHER_PASS` em `0x00100000`)

Atalho no Makefile do firmware:

```bash
make m1
make run-m1 QEMU=/caminho/qemu-8.2/build/qemu-system-riscv64
```

---

## 5. Checklist M1 (Dia 4)

Marcar na ordem. Se um item falhar, não pule para o próximo.

### Build

- [ ] `v8.2.0` clonado (não master)
- [ ] `gemio_gpio.c` / `.h` copiados para a árvore QEMU
- [ ] `meson.build` + `Kconfig` (misc e riscv) editados
- [ ] `virt.h` tem `VIRT_GEMIO_GPIO` e `GEMIO_GPIO_IRQ = 12`
- [ ] `virt.c` tem memmap `0x10010000`, `gemio_gpio_create(...)` e include
- [ ] `./configure --target-list=riscv64-softmmu --disable-werror` ok
- [ ] `make` gera `build/qemu-system-riscv64`

### Mapa de memória (antes do firmware)

No monitor (`-monitor stdio` numa primeira run, ou `-nographic` + Ctrl-A C):

```
info mtree
```

- [ ] Existe região `gemio-gpio` em `0x10010000`
- [ ] UART continua em `0x10000000` (não foi sobrescrita)
- [ ] virtio-mmio não invade `0x10010000`

Dump DTB (se aplicou o nó FDT):

```bash
qemu-system-riscv64 -M virt,dumpdtb=virt.dtb -nographic -bios none
dtc -I dtb -O dts virt.dtb | grep -A8 'gpio@10010000'
```

- [ ] Nó `gpio@10010000`, `compatible = "gemio,gpio"`, `interrupts = <12>`

### Firmware

- [ ] `firmware_m1.elf` compilado com `rv64imac` / `lp64`
- [ ] Entry point em `0x80000000` (`riscv64-unknown-elf-objdump -f`)
- [ ] Run com `-bios none -kernel firmware_m1.elf`

### Comportamento MMIO

- [ ] Log `WR DIR=0xff`
- [ ] Log `WR DATA=0xaa`
- [ ] Log `RD ... -> 0xaa`
- [ ] UART imprime `OK` (não `FAIL`)
- [ ] QEMU sai com shutdown guest (SiFive Test), não hang infinito
- [ ] Escrita em endereço inválido (`0x1001000C`) gera `LOG_GUEST_ERROR`

### Não-regressão

- [ ] Sem `Invalid MMIO` / `unassigned` em `0x10010000`
- [ ] UART ainda funciona (você viu `OK`)
- [ ] Reset zera registradores (segunda run, mesmo resultado)

**M1 fechado** quando todos os itens de Build + Mapa + Firmware + MMIO estiverem marcados.
