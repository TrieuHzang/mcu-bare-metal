# 00 - Startup in C

Bare-metal LED blink example for the STM32F103C8T6.

This example demonstrates the Cortex-M3 startup process without using STM32 HAL, CMSIS startup files, CubeMX-generated application code, or an operating system.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- LED: PC13
- Debugger: ST-LINK/V2
- Debug interface: SWD

## Features

- Custom Cortex-M3 vector table
- Custom `Reset_Handler`
- Custom linker script
- `.data` initialization from Flash to RAM
- `.bss` zero initialization
- Direct RCC register access
- Direct GPIO register access
- Bare-metal PC13 LED blink
- GNU Arm Embedded GCC build
- OpenOCD flashing with ST-LINK

## Boot Flow

```text
Reset
  |
  v
Vector Table
  |
  +--> MSP = 0x20005000
  |
  +--> Reset_Handler
          |
          +--> Copy .data
          |
          +--> Clear .bss
          |
          v
        main()
          |
          +--> Enable GPIOC clock
          |
          +--> Configure PC13 output
          |
          v
        LED Blink
```

## Memory Map

| Memory | Start Address | Size |
|---|---:|---:|
| Flash | `0x08000000` | 64 KB |
| SRAM | `0x20000000` | 20 KB |

The initial stack pointer is located at the top of SRAM:

```text
0x20000000 + 0x5000 = 0x20005000
```

## GPIO

The onboard Blue Pill LED is connected to PC13 and is active-low.

GPIOC clock is enabled through the `IOPCEN` bit in `RCC_APB2ENR`.

PC13 is configured as a general-purpose push-pull output through `GPIOC_CRH`.

## Build

```bash
make
```

## Flash

Connect ST-LINK to the board using SWD:

```text
ST-LINK      STM32F103C8T6
SWDIO   ---> PA13 / SWDIO
SWCLK   ---> PA14 / SWCLK
GND     ---> GND
```

Flash the firmware:

```bash
make flash
```

## Disassembly

```bash
make disasm
```

## Clean

```bash
make clean
```
