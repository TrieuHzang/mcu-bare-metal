# 00 - SysTick Interrupt

Bare-metal SysTick interrupt example for STM32F103C8T6.

This example configures the ARM Cortex-M3 SysTick timer to generate an interrupt every 1 ms and uses the system tick to blink the PC13 LED every 500 ms.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: STM32F103C8T6 Blue Pill
- LED: PC13
- Clock: 8 MHz HSI
- Language: C

## Concepts

- Cortex-M3 vector table
- SysTick timer
- Interrupt handler
- Direct register access
- Non-blocking timing
- Bare-metal C
- No HAL
- No RTOS

## Flow

```text
8 MHz CPU Clock
      |
      v
SysTick
      |
      v
1 ms Interrupt
      |
      v
SysTick_Handler
      |
      v
g_ms_ticks++
      |
      v
main
      |
      v
Toggle PC13 every 500 ms
```

## SysTick Configuration

The MCU uses the 8 MHz HSI clock after reset.

For a 1 ms interrupt:

```text
8,000,000 / 1000 = 8000
```

Therefore:

```text
SysTick LOAD = 8000 - 1
             = 7999
```

## Build

```bash
make
```

## Flash

```bash
make flash
```

## Clean

```bash
make clean
```
