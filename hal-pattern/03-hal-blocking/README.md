# 03 - HAL Blocking

STM32F103C8T6 LED blink using the STM32 HAL with blocking delay.

This example introduces the STM32 HAL abstraction layer while keeping the same PC13 LED blink behavior as the previous examples.

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

## HAL Initialization

The HAL is initialized with:

```c
HAL_Init();
```

HAL_Init configures the HAL time base using SysTick.

The SysTick interrupt increments the HAL tick counter:

```c
void SysTick_Handler(void)
{
    HAL_IncTick();
}
```

## GPIO

GPIOC clock is enabled with:

```c
__HAL_RCC_GPIOC_CLK_ENABLE();
```

PC13 is configured using:

```c
GPIO_InitTypeDef gpio = {0};

gpio.Pin = GPIO_PIN_13;
gpio.Mode = GPIO_MODE_OUTPUT_PP;
gpio.Speed = GPIO_SPEED_FREQ_LOW;

HAL_GPIO_Init(GPIOC, &gpio);
```

The LED output is controlled with:

```c
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
```

The Blue Pill onboard LED is active-low.

```text
PC13 LOW  = LED ON
PC13 HIGH = LED OFF
```

## Blocking Timing

The LED is controlled with:

```c
while (1)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    HAL_Delay(500U);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    HAL_Delay(500U);
}
```

HAL_Delay blocks program execution until the requested delay has elapsed.

## Progression

```text
00-register-macro
    |
    v
01-register-struct
    |
    v
02-cmsis-device
    |
    v
03-hal-blocking
```

Register-level GPIO clock enable:

```c
RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
```

HAL GPIO clock enable:

```c
__HAL_RCC_GPIOC_CLK_ENABLE();
```

Register-level output control:

```c
GPIOC->BSRR = (1U << GPIO_PIN);
```

HAL output control:

```c
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
```

## Dependencies

This example uses:

```text
CMSIS Core
CMSIS Device STM32F1
STM32F1xx HAL Driver
```

HAL modules used:

```text
HAL Core
HAL RCC
HAL FLASH
HAL GPIO
HAL Cortex
```

## Build

```bash
make
```

Generated files:

```text
hal_blocking.elf
hal_blocking.bin
hal_blocking.map
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

STM32F103C8T6 Blue Pill running the STM32 HAL blocking example with a 500 ms PC13 LED toggle.

https://github.com/user-attachments/assets/d603f272-adcd-4085-8a4a-29584cdc16ad


