# 04 - Timer Interrupt

Bare-metal STM32F103C8T6 TIM2 periodic interrupt example using CMSIS Device register definitions.

TIM2 generates an update interrupt every 500 ms and toggles the onboard PC13 LED.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- Timer: TIM2
- LED: PC13
- Interrupt period: 500 ms
- Toolchain: GNU Arm Embedded GCC
- Build system: GNU Make
- Debugger: ST-LINK/V2

## Application Flow

```text
TIM2 Clock
    |
    v
Prescaler
    |
    v
Counter
    |
    v
ARR reached
    |
    v
Update Event
    |
    v
TIM2 Interrupt
    |
    v
NVIC
    |
    v
TIM2_IRQHandler()
    |
    v
Toggle PC13 LED
```

## Timer Clock

With the default STM32F103 reset clock configuration:

```text
HSI = 8 MHz
APB1 prescaler = 1
TIM2 clock = 8 MHz
```

TIM2 is connected to APB1.

The peripheral clock is enabled with:

```c
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
```

## Prescaler

TIM2 uses:

```c
TIM2->PSC = 7999U;
```

The counter clock is:

```text
8,000,000 Hz
----------------
7999 + 1

=
1000 Hz
```

Therefore one timer count takes:

```text
1 ms
```

## Auto-Reload Register

The auto-reload value is:

```c
TIM2->ARR = 499U;
```

The counter runs through:

```text
0 ... 499
```

which gives:

```text
500 counts
```

At 1 ms per count:

```text
500 * 1 ms
=
500 ms
```

An update event therefore occurs every 500 ms.

## Timer Period

The general timer frequency is:

```text
timer_update_frequency
=
timer_clock
/
((PSC + 1) * (ARR + 1))
```

For this example:

```text
8,000,000
-------------------
8000 * 500

=
2 Hz
```

Therefore:

```text
interrupt period = 500 ms
```

## Update Interrupt

The TIM2 update interrupt is enabled with:

```c
TIM2->DIER |= TIM_DIER_UIE;
```

UIE means:

```text
Update Interrupt Enable
```

TIM2 is also enabled in the Cortex-M3 NVIC:

```c
NVIC_ClearPendingIRQ(TIM2_IRQn);
NVIC_EnableIRQ(TIM2_IRQn);
```

## Interrupt Handler

The application implements:

```c
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_SR_UIF) != 0U)
    {
        TIM2->SR &= ~TIM_SR_UIF;

        led_toggle();
    }
}
```

UIF means:

```text
Update Interrupt Flag
```

The flag must be cleared after handling the update event.

## Main Loop

The main loop does not perform timing:

```c
while (1)
{
}
```

LED timing is handled by TIM2 interrupts.

```text
main()
  |
  | normal execution
  |
  +------ 500 ms ------+
                       |
                       v
                 TIM2 interrupt
                       |
                       v
                 Toggle LED
                       |
                       v
                 Return to main
```

## SysTick vs TIM2

SysTick belongs to the ARM Cortex-M3 core.

```text
ARM Cortex-M3
    |
    +-- SysTick
```

TIM2 is an STM32 peripheral connected to APB1.

```text
STM32F103
    |
    +-- APB1
         |
         +-- TIM2
```

Therefore:

```text
SysTick = Cortex-M core timer

TIM2 = STM32 peripheral timer
```

## Vector Table

TIM2 is external IRQ number 28.

```text
Core vectors = 16
TIM2 IRQ     = 28

Vector index
= 16 + 28
= 44
```

The TIM2 vector entry is located at:

```text
0x08000000 + (44 * 4)
=
0x080000B0
```

The vector table contains 45 entries:

```text
45 * 4
=
180 bytes
=
0xB4
```

Therefore:

```text
Vector table  = 0x08000000
Reset_Handler = 0x080000B4
```

The TIM2 vector contains the TIM2_IRQHandler address with the Thumb bit set.

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
timer_interrupt.elf
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

STM32F103C8T6 TIM2 interrupt demo with a 500 ms PC13 LED toggle.

https://github.com/user-attachments/assets/99d6ea4c-0333-4f06-8370-0d955e28307c


