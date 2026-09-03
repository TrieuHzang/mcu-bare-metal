# 01 - Register Struct

Bare-metal STM32F103C8T6 LED blink using C structures to represent memory-mapped peripheral registers.

This example keeps the same behavior as `00-register-macro` while replacing individual register macros with peripheral register structures.

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

## Register Struct Pattern

Instead of defining every register separately:

```c
#define RCC_APB2ENR \
    (*(volatile uint32_t *)0x40021018U)
```

this example describes the peripheral register layout with a structure:

```c
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;
```

The peripheral base address is then converted to a pointer:

```c
#define RCC_BASE 0x40021000U
#define RCC ((RCC_TypeDef *)RCC_BASE)
```

Registers can then be accessed as:

```c
RCC->APB2ENR
```

## Register Address Mapping

For RCC:

```text
RCC base       = 0x40021000
APB2ENR offset = 0x18

0x40021000 + 0x18
= 0x40021018
```

Therefore:

```c
RCC->APB2ENR
```

accesses the same hardware register as:

```c
(*(volatile uint32_t *)0x40021018U)
```

## GPIOC

GPIOC is represented by:

```c
typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;
```

The peripheral instance is:

```c
#define GPIOC \
    ((GPIO_TypeDef *)0x40011000U)
```

The application can then use:

```c
GPIOC->CRH
GPIOC->BSRR
```

instead of individual raw register macros.

## SysTick

SysTick is also represented with a register structure:

```c
typedef struct
{
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile uint32_t CALIB;
} SysTick_TypeDef;
```

SysTick generates an interrupt every 1 ms.

The LED state changes every 500 ms using non-blocking timing.

## Comparison

`00-register-macro`:

```c
RCC_APB2ENR |= RCC_IOPCEN;

GPIOC_CRH &= ~PC13_CFG_MASK;
GPIOC_CRH |= PC13_OUTPUT_PP;
```

`01-register-struct`:

```c
RCC->APB2ENR |= RCC_IOPCEN;

GPIOC->CRH &= ~PC13_CFG_MASK;
GPIOC->CRH |= PC13_OUTPUT_PP;
```

The generated hardware access is equivalent. Only the C representation changes.

## Build

```bash
make
```

Generated files:

```text
register_struct.elf
register_struct.bin
register_struct.map
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

Hardware verification will be performed later when the Blue Pill is available.

## Clean

```bash
make clean
```

## Current Status

Software build and ELF verification completed.

Hardware flashing and demo video will be added after hardware verification.
