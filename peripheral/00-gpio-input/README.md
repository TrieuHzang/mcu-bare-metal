# 00 - GPIO Input

Bare-metal STM32F103C8T6 GPIO input example using CMSIS Device register definitions.

An external push button connected to PA0 controls the onboard PC13 LED.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- Button: PA0
- LED: PC13
- Toolchain: GNU Arm Embedded GCC
- Build system: GNU Make
- Debugger: ST-LINK/V2

## Hardware Connection

```text
PA0 ----- Push Button ----- GND
```

PA0 uses the internal pull-up resistor.

```text
Button released -> PA0 HIGH
Button pressed  -> PA0 LOW
```

The onboard PC13 LED is active-low.

```text
PC13 HIGH -> LED OFF
PC13 LOW  -> LED ON
```

## GPIO Configuration

GPIOA and GPIOC clocks are enabled through RCC:

```c
RCC->APB2ENR |=
    RCC_APB2ENR_IOPAEN |
    RCC_APB2ENR_IOPCEN;
```

PA0 is configured as input with pull-up or pull-down:

```text
CNF0  = 10
MODE0 = 00
```

The corresponding configuration value is:

```text
1000 = 0x8
```

```c
GPIOA->CRL &= ~PA0_CFG_MASK;
GPIOA->CRL |= PA0_INPUT_PUPD;
```

The internal pull-up is selected by setting the PA0 output data bit:

```c
GPIOA->BSRR = (1U << BUTTON_PIN);
```

## Reading Input

PA0 is read from the GPIO input data register:

```c
GPIOA->IDR
```

The button state is checked with:

```c
if ((GPIOA->IDR & (1U << BUTTON_PIN)) == 0U)
{
    return 1U;
}
```

Because the button is active-low:

```text
PA0 = 0 -> pressed
PA0 = 1 -> released
```

## Application Flow

```text
Reset
  |
  v
Reset_Handler
  |
  v
main()
  |
  v
GPIO initialization
  |
  v
Read PA0
  |
  +-- LOW  -> LED ON
  |
  +-- HIGH -> LED OFF
  |
  v
Repeat
```

## Polling

This example uses GPIO polling.

```c
while (1)
{
    if (button_is_pressed() != 0U)
    {
        led_on();
    }
    else
    {
        led_off();
    }
}
```

The CPU continuously checks GPIOA->IDR.

The next example replaces polling with an external interrupt.

## Build

```bash
make
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

STM32F103C8T6 Blue Pill GPIO input demo.

Pressing the PA0 button turns the onboard PC13 LED on.

https://github.com/user-attachments/assets/2eabfc2d-6099-46d7-8b31-1e53f21ba5b1


