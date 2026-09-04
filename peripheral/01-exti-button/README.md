# 01 - EXTI Button

Bare-metal STM32F103C8T6 external interrupt example using CMSIS Device register definitions.

An external push button connected to PA0 generates an EXTI0 interrupt and toggles the onboard PC13 LED.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- Button: PA0
- LED: PC13
- Interrupt: EXTI0
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

## Interrupt Flow

```text
Button Press
    |
    v
PA0 HIGH -> LOW
    |
    v
EXTI Line 0
    |
    v
NVIC
    |
    v
Vector Table
    |
    v
EXTI0_IRQHandler()
    |
    v
Toggle PC13 LED
```

## GPIO Configuration

GPIOA, GPIOC and AFIO clocks are enabled:

```c
RCC->APB2ENR |=
    RCC_APB2ENR_AFIOEN |
    RCC_APB2ENR_IOPAEN |
    RCC_APB2ENR_IOPCEN;
```

PA0 is configured as input with internal pull-up.

PC13 is configured as a general-purpose push-pull output.

## EXTI Mapping

EXTI0 can be mapped to pin 0 of different GPIO ports.

```text
PA0
PB0
PC0
...
```

AFIO selects which GPIO port is connected to EXTI0.

For PA0:

```c
AFIO->EXTICR[0] &= ~(0xFU << 0U);
```

This selects:

```text
EXTI0 <- PA0
```

## Falling Edge Trigger

The button is active-low.

```text
Released = HIGH
Pressed  = LOW
```

Therefore a button press creates:

```text
HIGH -> LOW
```

which is a falling edge.

```c
EXTI->RTSR &= ~EXTI_RTSR_TR0;
EXTI->FTSR |= EXTI_FTSR_TR0;
```

## Interrupt Mask

EXTI line 0 is unmasked:

```c
EXTI->IMR |= EXTI_IMR_MR0;
```

## NVIC

The Cortex-M3 NVIC is used to enable the EXTI0 interrupt:

```c
NVIC_ClearPendingIRQ(EXTI0_IRQn);
NVIC_EnableIRQ(EXTI0_IRQn);
```

## Interrupt Handler

The application provides a strong EXTI0 interrupt handler:

```c
void EXTI0_IRQHandler(void)
{
    if ((EXTI->PR & EXTI_PR_PR0) != 0U)
    {
        EXTI->PR = EXTI_PR_PR0;

        led_toggle();
    }
}
```

The EXTI pending flag is cleared by writing 1 to the corresponding bit.

## Vector Table

EXTI0 is STM32 external IRQ number 6.

```text
Core vectors = 16

EXTI0 IRQ number = 6

Vector index = 16 + 6
             = 22
```

The EXTI0 vector therefore appears at:

```text
0x08000000 + (22 * 4)
=
0x08000058
```

The vector table contains 23 entries up to EXTI0:

```text
23 * 4 bytes
=
92 bytes
=
0x5C
```

Therefore Reset_Handler starts after the vector table at:

```text
0x0800005C
```

The ELF entry point includes the Thumb bit:

```text
0x0800005D
```

## Polling vs Interrupt

Previous GPIO polling example:

```c
while (1)
{
    if (button_is_pressed())
    {
        ...
    }
}
```

The CPU continuously reads the GPIO input.

This EXTI example:

```c
while (1)
{
}
```

Button processing happens only when the interrupt occurs.

```text
GPIO Polling
     |
     v
CPU checks input continuously


GPIO Interrupt
     |
     v
CPU handles button only when event occurs
```

## Button Bounce

Mechanical push buttons may generate multiple electrical transitions during one physical press.

This example intentionally does not implement debounce so that the focus remains on:

```text
GPIO
AFIO
EXTI
NVIC
Vector Table
ISR
```

Debounce can be added later using SysTick or a hardware timer.

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

## Inspect Vector Table

```bash
arm-none-eabi-objdump \
-s \
-j .isr_vector \
exti_button.elf
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

STM32F103C8T6 Blue Pill EXTI button interrupt demo.

Each PA0 button press generates an EXTI0 interrupt and toggles the onboard PC13 LED.

https://github.com/user-attachments/assets/f83262dc-d7e7-4411-ba1d-2eb17650f388


