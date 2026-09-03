# 05 - PWM

Bare-metal STM32F103C8T6 PWM output example using TIM2 Channel 1 and CMSIS Device register definitions.

TIM2 generates a 1 kHz PWM signal with a 50 percent duty cycle on PA0.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- Timer: TIM2
- Channel: TIM2_CH1
- Output pin: PA0
- PWM frequency: 1 kHz
- Duty cycle: 50 percent
- Toolchain: GNU Arm Embedded GCC
- Build system: GNU Make
- Debugger: ST-LINK/V2

## Hardware Output

TIM2 Channel 1 uses PA0 with the default alternate-function mapping.

```text
TIM2
 |
 v
TIM2_CH1
 |
 v
PA0
```

For LED testing:

```text
PA0 ---- resistor ---- LED ---- GND
```

A 220 ohm or 330 ohm resistor can be used with an external LED.

An oscilloscope or logic analyzer can also be connected directly to PA0 to inspect the PWM waveform.

## PWM

PWM means:

```text
Pulse Width Modulation
```

A PWM signal repeatedly switches between HIGH and LOW.

For a 50 percent duty cycle:

```text
       HIGH            HIGH
    +--------+      +--------+
    |        |      |        |
----+        +------+        +----
             LOW
```

The signal in this example has:

```text
Frequency  = 1 kHz
Period     = 1 ms
Duty cycle = 50 percent
```

Therefore:

```text
HIGH time = 0.5 ms
LOW time  = 0.5 ms
```

## GPIO Configuration

PA0 is configured as alternate-function push-pull output.

```text
MODE0 = 11
CNF0  = 10

CNF MODE = 1011 = 0xB
```

```c
GPIOA->CRL &= ~PA0_CFG_MASK;
GPIOA->CRL |= PA0_TIM2_CH1;
```

This allows TIM2 Channel 1 to control PA0 directly.

## Timer Clock

With the default reset clock:

```text
HSI = 8 MHz
APB1 prescaler = 1
TIM2 clock = 8 MHz
```

TIM2 clock is enabled with:

```c
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
```

## Prescaler

The prescaler is:

```c
TIM2->PSC = 7U;
```

Therefore:

```text
8,000,000
-----------
7 + 1

=
1,000,000 Hz
```

The TIM2 counter runs at:

```text
1 MHz
```

One counter tick therefore takes:

```text
1 us
```

## Auto-Reload Register

The period is configured with:

```c
TIM2->ARR = 999U;
```

The counter runs from:

```text
0 ... 999
```

which gives:

```text
1000 timer counts
```

At 1 us per count:

```text
1000 us
=
1 ms
```

Therefore:

```text
PWM frequency
=
1 / 1 ms
=
1 kHz
```

## Compare Register

The duty cycle is configured with:

```c
TIM2->CCR1 = 500U;
```

In PWM mode 1:

```text
CNT < CCR1
-> output active

CNT >= CCR1
-> output inactive
```

For this example:

```text
ARR + 1 = 1000
CCR1    = 500
```

Therefore:

```text
Duty cycle
=
500 / 1000
=
50 percent
```

## Timer Registers

The three main timer values are:

```text
PSC
 |
 +-- counter clock

ARR
 |
 +-- PWM period

CCR1
 |
 +-- duty cycle
```

The PWM frequency depends on both PSC and ARR.

## PWM Mode

TIM2 Channel 1 is configured for PWM mode 1:

```c
TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;

TIM2->CCMR1 |=
    TIM_CCMR1_OC1M_1 |
    TIM_CCMR1_OC1M_2 |
    TIM_CCMR1_OC1PE;
```

The output mode bits are:

```text
OC1M = 110
```

which selects:

```text
PWM mode 1
```

## Channel Output

TIM2 Channel 1 output is enabled with:

```c
TIM2->CCER |= TIM_CCER_CC1E;
```

The output polarity remains active-high.

## Hardware PWM

Unlike the previous timer interrupt example, PWM generation does not require an interrupt handler.

```text
Timer Interrupt

TIM2
 |
 v
NVIC
 |
 v
CPU
 |
 v
TIM2_IRQHandler()


PWM

TIM2
 |
 v
Compare Hardware
 |
 v
TIM2_CH1
 |
 v
PA0
```

After TIM2 starts:

```c
TIM2->CR1 |= TIM_CR1_CEN;
```

the PWM signal continues running without CPU intervention.

## Duty Cycle Examples

For:

```text
ARR = 999
```

approximately 10 percent:

```c
TIM2->CCR1 = 100U;
```

50 percent:

```c
TIM2->CCR1 = 500U;
```

90 percent:

```c
TIM2->CCR1 = 900U;
```

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

## Current Status

Software build and ELF verification completed.

Hardware demo video will be included here after hardware verification.
