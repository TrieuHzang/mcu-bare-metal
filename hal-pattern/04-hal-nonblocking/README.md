# 04 - HAL Non-Blocking

STM32F103C8T6 LED blink using STM32 HAL with non-blocking timing.

This example keeps the same LED blink behavior as `03-hal-blocking`, but replaces `HAL_Delay()` with elapsed-time checking using `HAL_GetTick()`.

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

The SysTick interrupt updates the HAL tick counter:

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

## Non-Blocking Timing

Instead of:

```c
HAL_Delay(500U);
```

the application uses:

```c
if ((uint32_t)(HAL_GetTick() - last_toggle) >= 500U)
{
    last_toggle = HAL_GetTick();

    /* Toggle LED */
}
```

The main loop continues running while waiting for 500 ms to elapse.

## Blocking vs Non-Blocking

Blocking:

```c
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

HAL_Delay(500U);

HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

HAL_Delay(500U);
```

Non-blocking:

```c
if ((uint32_t)(HAL_GetTick() - last_toggle) >= 500U)
{
    last_toggle = HAL_GetTick();

    /* Toggle LED */
}
```

With non-blocking timing, other application tasks can be added to the main loop without waiting for a delay to finish.

For example:

```c
while (1)
{
    check_led();
    check_button();
    check_uart();
    check_sensor();
}
```

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
    |
    v
04-hal-nonblocking
```

## Dependencies

This example uses:

```text
CMSIS Core
CMSIS Device STM32F1
STM32F1xx HAL Driver
```

## Build

```bash
make
```

Generated files:

```text
hal_nonblocking.elf
hal_nonblocking.bin
hal_nonblocking.map
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
