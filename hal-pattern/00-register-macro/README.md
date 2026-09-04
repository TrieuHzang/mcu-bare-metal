# 00 - Register Macro

Bare-metal STM32F103C8T6 LED blink using C macros to access memory-mapped hardware registers directly.

This example focuses on representing hardware registers with C preprocessor macros and using SysTick for non-blocking timing.

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

## Register Macro Pattern

A memory-mapped hardware register can be represented in C as:

```c
#define REGISTER (*(volatile uint32_t *)ADDRESS)
```

For example:

```c
#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018U)
```

The address is converted to a pointer to a 32-bit value.

The volatile keyword tells the compiler that the value may change because of hardware and that register accesses must not be optimized away.

The dereference operator allows the hardware register to be read or written like a C variable.

## RCC

GPIOC requires its peripheral clock to be enabled before use.

```c
#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018U)
#define RCC_IOPCEN  (1U << 4U)
```

GPIOC clock is enabled with:

```c
RCC_APB2ENR |= RCC_IOPCEN;
```

## GPIOC

The example uses:

```c
#define GPIOC_CRH  (*(volatile uint32_t *)0x40011004U)
#define GPIOC_BSRR (*(volatile uint32_t *)0x40011010U)
```

PC13 belongs to GPIOC_CRH because CRH configures GPIO pins 8 through 15.

PC13 is configured as:

```text
CNF13  = 00
MODE13 = 10
```

This selects a general-purpose push-pull output with a maximum output speed of 2 MHz.

The Blue Pill onboard LED on PC13 is active-low.

```text
PC13 LOW  = LED ON
PC13 HIGH = LED OFF
```

GPIOC_BSRR is used for both setting and resetting PC13.

## SysTick

SysTick is part of the ARM Cortex-M3 core.

The example accesses its registers directly:

```c
#define SYST_CSR (*(volatile uint32_t *)0xE000E010U)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014U)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018U)
```

SysTick is configured to generate an interrupt every 1 ms.

With the default 8 MHz HSI clock:

```text
8,000,000 / 1000 = 8000 cycles

Reload value = 8000 - 1 = 7999
```

## SysTick Handler

The interrupt handler increments a millisecond counter:

```c
volatile uint32_t g_ms_ticks = 0U;

void SysTick_Handler(void)
{
    g_ms_ticks++;
}
```

The SysTick entry in the Cortex-M3 vector table points to SysTick_Handler.

## Non-Blocking Timing

The main loop does not use a busy-wait delay.

Instead, it checks the elapsed time:

```c
if ((uint32_t)(g_ms_ticks - last_toggle) >= 500U)
{
    last_toggle = g_ms_ticks;

    /* Toggle LED */
}
```

This allows the CPU to continue executing the main loop while waiting for the next LED toggle.

## Startup Flow

```text
Reset
  |
  v
Vector Table
  |
  +-- Initial MSP
  |
  v
Reset_Handler
  |
  +-- Copy .data
  |
  +-- Clear .bss
  |
  v
main()
  |
  +-- gpio_init()
  |
  +-- systick_init()
  |
  v
Main Loop
```

SysTick interrupt flow:

```text
8 MHz CPU clock
      |
      v
SysTick counter
      |
      | every 8000 cycles
      v
SysTick exception
      |
      v
SysTick_Handler
      |
      v
g_ms_ticks++
```

## Memory

The STM32F103C8T6 memory configuration used by the linker script is:

```text
Flash: 0x08000000, 64 KB
SRAM:  0x20000000, 20 KB
```

The initial Main Stack Pointer is:

```text
0x20005000
```

## Build

```bash
make
```

Generated files:

```text
register_macro.elf
register_macro.bin
register_macro.map
```

## Disassembly

```bash
make disasm
```

Output:

```text
register_macro.lst
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

Connect the STM32F103C8T6 Blue Pill through ST-LINK/V2 and run:

```bash
make flash
```

## Clean

```bash
make clean
```

## Demo

STM32F103C8T6 Blue Pill running the register-macro example with a 500 ms PC13 LED toggle.


https://github.com/user-attachments/assets/3eb7169b-fa8b-4bd0-a042-01dc0251adb0


