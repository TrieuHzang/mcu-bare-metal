# MCU Bare-Metal

Bare-metal programming exercises for ARM Cortex-M microcontrollers.

This repository focuses on understanding firmware below high-level hardware abstraction layers, including MCU startup, memory layout, linker scripts, peripheral registers, and low-level debugging.

## Current Target

### STM32F103C8T6

- ARM Cortex-M3
- 64 KB Flash
- 20 KB SRAM
- GNU Arm Embedded GCC
- GNU Make
- OpenOCD
- ST-LINK/V2

## Examples

### Compiler

- [`00-startup-c`](compiler/00-startup-c/) - Custom vector table, `Reset_Handler`, linker script, direct GPIO register access, and PC13 LED blink.

## Build Flow

```text
C Source
   |
   v
arm-none-eabi-gcc
   |
   v
Object File
   |
   +--> Custom Linker Script
   |
   v
ELF Firmware
   |
   v
OpenOCD
   |
   v
ST-LINK
   |
   v
STM32F103C8T6
```

## Bare-Metal Approach

The first example intentionally avoids:

- STM32 HAL
- CMSIS startup files
- CubeMX-generated application code
- RTOS

The goal is to understand the Cortex-M startup process and peripheral control at register level.
