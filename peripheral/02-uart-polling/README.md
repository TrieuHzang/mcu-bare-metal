# 02 - UART Polling

Bare-metal STM32F103C8T6 USART1 communication example using CMSIS Device register definitions.

The application sends startup messages through USART1 and echoes every received byte back to the sender using polling.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- USART: USART1
- TX: PA9
- RX: PA10
- Baud rate: 115200
- Format: 8N1
- Toolchain: GNU Arm Embedded GCC
- Build system: GNU Make
- Debugger: ST-LINK/V2

## Hardware Connection

Connect a 3.3 V USB-UART adapter.

```text
STM32F103          USB-UART

PA9  TX ----------> RX
PA10 RX <---------- TX
GND    ------------ GND
```

TX and RX must be crossed.

```text
STM32 TX -> USB-UART RX
STM32 RX <- USB-UART TX
```

## Application

After startup the STM32 sends:

```text
Hello from STM32F103
UART polling echo ready
```

The application then waits for incoming bytes.

```text
PC sends byte
     |
     v
USART1 RX
     |
     v
RXNE = 1
     |
     v
Read USART1->DR
     |
     v
Write byte to USART1->DR
     |
     v
PC receives same byte
```

For example:

```text
PC -> STM32: A
STM32 -> PC: A
```

## GPIO Configuration

USART1 uses:

```text
PA9  = USART1_TX
PA10 = USART1_RX
```

PA9 is configured as alternate-function push-pull output:

```text
CNF9  = 10
MODE9 = 11

1011 = 0xB
```

PA10 is configured as floating input:

```text
CNF10  = 01
MODE10 = 00

0100 = 0x4
```

## USART Clock

USART1 is connected to APB2.

With the default STM32F103 reset clock configuration:

```text
HSI   = 8 MHz
PCLK2 = 8 MHz
```

USART1 clock is enabled with:

```c
RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
```

## Baud Rate

The example uses:

```text
115200 baud
```

For an 8 MHz peripheral clock, the USART1 BRR value is approximately:

```text
BRR = 0x45
```

The source calculates the value from:

```c
#define PCLK2_HZ   8000000U
#define USART_BAUD 115200U

#define USART1_BRR_VALUE \
    ((PCLK2_HZ + (USART_BAUD / 2U)) / USART_BAUD)
```

## USART Configuration

USART1 is configured with:

```c
USART1->CR1 =
    USART_CR1_TE |
    USART_CR1_RE |
    USART_CR1_UE;
```

The enabled features are:

```text
TE = Transmitter Enable
RE = Receiver Enable
UE = USART Enable
```

## Transmit Polling

Before writing a byte, the CPU waits for TXE:

```c
while ((USART1->SR & USART_SR_TXE) == 0U)
{
}

USART1->DR = data;
```

TXE means:

```text
Transmit Data Register Empty
```

## Receive Polling

The CPU waits for RXNE:

```c
while ((USART1->SR & USART_SR_RXNE) == 0U)
{
}

return (uint8_t)(USART1->DR & 0xFFU);
```

RXNE means:

```text
Read Data Register Not Empty
```

## Polling Behavior

The main loop is:

```c
while (1)
{
    received_byte = uart1_read_byte();

    uart1_write_byte(received_byte);
}
```

While no byte is available, the CPU remains inside:

```c
while ((USART1->SR & USART_SR_RXNE) == 0U)
{
}
```

This is polling.

```text
CPU
 |
 +-- Check RXNE
 |
 +-- Check RXNE
 |
 +-- Check RXNE
 |
 +-- Byte received
 |
 v
Read DR
```

The next example replaces receive polling with a USART interrupt.

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
