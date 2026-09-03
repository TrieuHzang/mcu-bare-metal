<p align="center">
  <img src="https://hits.sh/github.com/TrieuHzang/mcu-bare-metal.svg?label=Repo%20Traffic&color=007ec6" alt="Repo Traffic">
</p>

# MCU Bare-Metal

Bare-metal programming exercises for ARM Cortex-M microcontrollers.

This repository focuses on understanding how firmware works below high-level hardware abstraction layers, including MCU startup, memory layout, linker scripts, direct peripheral register access, build tools, flashing, and low-level debugging.

The current implementation targets the STM32F103C8T6 and runs directly on hardware without STM32 HAL, CubeMX-generated application code, or an RTOS.

## Current Target

| Item | Configuration |
|---|---|
| MCU | STM32F103C8T6 |
| Core | ARM Cortex-M3 |
| Board | Blue Pill |
| Flash | 64 KB |
| SRAM | 20 KB |
| Debugger | ST-LINK/V2 |
| Debug interface | SWD |
| Toolchain | GNU Arm Embedded GCC |
| Build system | GNU Make |
| Flash and debug | OpenOCD |

## Repository Structure

```text
mcu-bare-metal/
|
+-- compiler/
|   |
|   +-- 00-startup-c/
|       |
|       +-- Makefile
|       +-- README.md
|       +-- led_blink.c
|       +-- led_blink.h
|       +-- stm32f103c8t6.ld
|
+-- .gitignore
+-- README.md
```

## Examples

### 00-startup-c

Location:

```text
compiler/00-startup-c/
```

This example includes:

- Custom Cortex-M3 vector table
- Custom Reset_Handler
- Custom linker script
- .data initialization from Flash to RAM
- .bss zero initialization
- Direct RCC register access
- Direct GPIO register access
- PC13 LED blink
- GNU Arm GCC build
- OpenOCD flashing with ST-LINK

More details:

[00-startup-c](compiler/00-startup-c/)

## Startup Flow

```text
Power / Reset
     |
     v
Vector Table
     |
     +-- Initial MSP = 0x20005000
     |
     +-- Reset_Handler
             |
             +-- Copy .data from Flash to RAM
             |
             +-- Clear .bss
             |
             v
           main()
             |
             +-- Enable GPIOC clock
             |
             +-- Configure PC13 as output
             |
             v
           LED Blink
```

## Memory Layout

```text
FLASH
0x08000000

+--------------------------+
| Vector Table             |
+--------------------------+
| .text                    |
| Program instructions     |
+--------------------------+
| .rodata                  |
+--------------------------+
| Initial .data values     |
+--------------------------+


SRAM
0x20000000

+--------------------------+
| .data                    |
+--------------------------+
| .bss                     |
+--------------------------+
|                          |
| Free RAM                 |
|                          |
+--------------------------+
| Stack                    |
+--------------------------+

0x20005000
```

The initial Main Stack Pointer is placed at the top of SRAM:

```text
0x20000000 + 0x5000 = 0x20005000
```

## Bare-Metal Approach

The first example does not use:

- STM32 HAL
- CMSIS startup files
- CubeMX-generated application code
- RTOS

Peripheral registers are accessed directly using memory-mapped addresses.

For the LED blink example:

```text
RCC_APB2ENR
     |
     +-- Enable GPIOC clock

GPIOC_CRH
     |
     +-- Configure PC13 as output

GPIOC_BRR / GPIOC_BSRR
     |
     +-- Control PC13 output state
```

## Hardware Connection

The STM32F103C8T6 is programmed using ST-LINK/V2 through SWD.

```text
ST-LINK/V2          STM32F103C8T6
----------------------------------
SWDIO       --->    PA13 / SWDIO
SWCLK       --->    PA14 / SWCLK
GND         --->    GND
```

The onboard Blue Pill LED is connected to PC13 and is active-low.

## Development Environment

The project was developed on Ubuntu Linux using:

```text
gcc-arm-none-eabi
make
git
openocd
gdb-multiarch
```

Install the required packages:

```bash
sudo apt update

sudo apt install -y \
    gcc-arm-none-eabi \
    make \
    git \
    openocd \
    gdb-multiarch
```

## Build

Go to the first example:

```bash
cd compiler/00-startup-c
```

Build the firmware:

```bash
make
```

The build generates:

```text
led_blink.o
led_blink.elf
led_blink.bin
led_blink.map
```

## Flash

Connect the board through ST-LINK and run:

```bash
make flash
```

A successful flash should include:

```text
Programming Finished
Verified OK
Resetting Target
```

## Disassembly

Generate the disassembly file:

```bash
make disasm
```

Output:

```text
led_blink.lst
```

## Clean

Remove generated build files:

```bash
make clean
```

## Hardware Verification

The first example was built, flashed, and tested on a STM32F103C8T6 Blue Pill.

Verified values:

```text
Vector table  : 0x08000000
Reset_Handler : 0x08000064
Initial MSP   : 0x20005000
Flash size    : 64 KB
```

Flash result:

```text
Programming Finished
Verified OK
Resetting Target
```

Hardware result:

```text
PC13 onboard LED blinks continuously.
```

## Demo

A short hardware demo is included in the first example.

[View the startup example](compiler/00-startup-c/)

The demo shows the STM32F103C8T6 running the PC13 LED blink firmware after being built with GNU Arm GCC and flashed through ST-LINK using OpenOCD.

## Build Verification

Check the ELF sections:

```bash
arm-none-eabi-objdump -h led_blink.elf
```

The vector table should start at:

```text
0x08000000
```

Check the symbols:

```bash
arm-none-eabi-nm -n led_blink.elf
```

The stack symbol should be:

```text
_estack = 0x20005000
```

## Future Work

Possible next exercises:

```text
arm-cortex-m/
+-- systick/

interrupt/
+-- gpio-interrupt/

peripheral/
+-- uart/
+-- timer/
+-- spi/
```

These examples have not been implemented yet.

## References

- STM32F103C8T6 Datasheet
- STM32F1 RM0008 Reference Manual
- ARM Cortex-M3 documentation
- GNU Arm Embedded Toolchain documentation
- OpenOCD documentation

## Contact & Support

**Trieu Ha Giang** - Embedded Systems Engineering Student

```text
Thank you for visiting this repository.
If you have any questions or feedback about the system design, embedded firmware, or hardware integration, feel free to reach out directly.
```

**My contact:**

[![Email](https://img.shields.io/badge/Email-trieuhagiang1312%40gmail.com-EA4335?style=flat&logo=gmail&logoColor=white)](mailto:trieuhagiang1312@gmail.com)
[![GitHub](https://img.shields.io/badge/GitHub-TrieuHzang-181717?style=flat&logo=github&logoColor=white)](https://github.com/TrieuHzang)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-haazangg-0A66C2?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/haazangg/)

## HAL Patterns

### 00-register-macro

[00-register-macro](hal-pattern/00-register-macro/)

Direct memory-mapped register access using C macros on the STM32F103C8T6.

Topics:

- RCC register access
- GPIO register access
- SysTick register access
- PC13 LED control
- 1 ms system tick
- Non-blocking 500 ms LED timing
