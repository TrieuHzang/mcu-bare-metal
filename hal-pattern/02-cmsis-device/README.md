# 02 - CMSIS Device

Bare-metal STM32F103C8T6 LED blink using CMSIS Core and STM32F1 CMSIS Device headers.

This example keeps the same application behavior as the previous examples while replacing manually defined peripheral structures, base addresses, and register bit masks with definitions provided by CMSIS Device.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- LED: PC13
- Clock after reset: 8 MHz HSI
- Toolchain: GNU Arm Embedded GCC
- Build system: GNU Make
- Debugger: ST-LINK/V2
- Flash tool: OpenOCD

## CMSIS Headers

The application includes:

```c
#include "stm32f103xb.h"
```

CMSIS Device provides definitions such as:

```c
RCC
GPIOC
RCC_APB2ENR_IOPCEN
```

CMSIS Core provides Cortex-M3 definitions such as:

```c
SysTick
SysTick_CTRL_CLKSOURCE_Msk
SysTick_CTRL_TICKINT_Msk
SysTick_CTRL_ENABLE_Msk
```

## Progression

### 00-register-macro

Register addresses and bit positions are defined manually.

```c
#define RCC_APB2ENR \
    (*(volatile uint32_t *)0x40021018U)

RCC_APB2ENR |= (1U << 4U);
```

### 01-register-struct

Peripheral register layouts are represented using manually created structures.

```c
RCC->APB2ENR |= (1U << 4U);
```

### 02-cmsis-device

Peripheral structures, addresses, and standard bit masks are provided by CMSIS Device.

```c
RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
```

The hardware register being accessed is still the same.

## GPIOC

PC13 is configured as a general-purpose push-pull output at 2 MHz.

```c
GPIOC->CRH &= ~PC13_CFG_MASK;
GPIOC->CRH |= PC13_OUTPUT_PP;
```

The onboard Blue Pill LED is active-low.

```text
PC13 LOW  = LED ON
PC13 HIGH = LED OFF
```

## SysTick

SysTick is configured using CMSIS Core definitions.

```c
SysTick->LOAD = SYSTICK_RELOAD;
SysTick->VAL = 0U;

SysTick->CTRL =
    SysTick_CTRL_CLKSOURCE_Msk |
    SysTick_CTRL_TICKINT_Msk |
    SysTick_CTRL_ENABLE_Msk;
```

With an 8 MHz CPU clock:

```text
8,000,000 / 1000 = 8000 cycles

Reload value = 7999
```

The SysTick interrupt runs every 1 ms.

## Non-Blocking Timing

The SysTick handler increments:

```c
volatile uint32_t g_ms_ticks;
```

The main loop checks elapsed time:

```c
if ((uint32_t)(g_ms_ticks - last_toggle) >= 500U)
{
    last_toggle = g_ms_ticks;

    /* Toggle LED */
}
```

No busy-wait delay is used.

## Build

```bash
make
```

Generated files:

```text
cmsis_device.elf
cmsis_device.bin
cmsis_device.map
```

## Disassembly

```bash
make disasm
```

## Symbols

```bash
make symbols
```

## Sections

```bash
make sections
```

## Flash

```bash
make flash
```

## Clean

```bash
make clean
```

## Demo

STM32F103C8T6 Blue Pill running the CMSIS Device example with a 500 ms PC13 LED toggle.


https://github.com/user-attachments/assets/e5161fd2-aaa4-4954-949c-fcb3e88915c7


