# 03 - UART Interrupt

Bare-metal STM32F103C8T6 USART1 receive interrupt example using CMSIS Device register definitions.

USART1 receives bytes using interrupts while transmission remains polling-based.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Board: Blue Pill
- USART: USART1
- TX: PA9
- RX: PA10
- Baud rate: 115200
- Format: 8N1
- Interrupt: USART1 IRQ
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

## Application

After startup the STM32 sends:

```text
Hello from STM32F103
UART interrupt echo ready
```

When a byte arrives:

```text
USB-UART TX
     |
     v
PA10
     |
     v
USART1 RX
     |
     v
RXNE = 1
     |
     v
USART1 interrupt
     |
     v
NVIC
     |
     v
USART1_IRQHandler()
     |
     v
Store received byte
     |
     v
Set receive flag
     |
     v
main()
     |
     v
Echo byte through USART1 TX
```

## GPIO Configuration

USART1 uses:

```text
PA9  = USART1_TX
PA10 = USART1_RX
```

PA9 is configured as alternate-function push-pull output.

PA10 is configured as floating input.

## USART Configuration

USART1 is enabled with:

```c
USART1->CR1 =
    USART_CR1_RXNEIE |
    USART_CR1_TE |
    USART_CR1_RE |
    USART_CR1_UE;
```

The enabled features are:

```text
RXNEIE = RX register not empty interrupt enable
TE     = Transmitter enable
RE     = Receiver enable
UE     = USART enable
```

## NVIC

USART1 interrupts are enabled in the Cortex-M3 NVIC:

```c
NVIC_ClearPendingIRQ(USART1_IRQn);
NVIC_EnableIRQ(USART1_IRQn);
```

Both the USART peripheral interrupt source and the NVIC interrupt must be enabled.

```text
USART1 RXNE
    |
    v
RXNEIE
    |
    v
NVIC
    |
    v
CPU
```

## Interrupt Handler

The application implements:

```c
void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_SR_RXNE) != 0U)
    {
        g_rx_byte =
            (uint8_t)(USART1->DR & 0xFFU);

        g_rx_ready = 1U;
    }
}
```

Reading USART1->DR clears RXNE.

The interrupt handler only receives and stores the byte.

Transmission is performed later from the main loop.

## Shared Variables

The receive byte and flag are shared between the interrupt handler and main():

```c
static volatile uint8_t g_rx_byte = 0U;
static volatile uint8_t g_rx_ready = 0U;
```

`volatile` is required because the variables can change asynchronously inside the interrupt handler.

## Main Loop

Unlike the polling example, main() does not continuously check USART1->SR.

```c
while (1)
{
    if (g_rx_ready != 0U)
    {
        NVIC_DisableIRQ(USART1_IRQn);

        data = g_rx_byte;
        g_rx_ready = 0U;

        NVIC_EnableIRQ(USART1_IRQn);

        uart1_write_byte(data);
    }
}
```

The short interrupt-disabled region protects the shared receive state.

## Polling vs Interrupt

UART polling:

```text
CPU
 |
 +-- Check RXNE
 +-- Check RXNE
 +-- Check RXNE
 +-- Check RXNE
 |
 +-- Byte arrives
 |
 v
Read DR
```

UART interrupt:

```text
CPU runs normal code
        |
        | byte arrives
        v
USART1 interrupt
        |
        v
USART1_IRQHandler()
        |
        v
Return to normal code
```

## Vector Table

USART1 is external IRQ number 37.

```text
Core vectors = 16
USART1 IRQ   = 37

Vector index
= 16 + 37
= 53
```

The USART1 vector entry is located at:

```text
0x08000000 + (53 * 4)
=
0x080000D4
```

The vector table contains 54 entries:

```text
54 * 4
=
216 bytes
=
0xD8
```

Therefore:

```text
Vector table  = 0x08000000
Reset_Handler = 0x080000D8
```

The USART1 vector contains the handler address with the Thumb bit set.

## Current Limitation

This example uses a one-byte receive buffer:

```text
g_rx_byte
```

If bytes arrive faster than main() can process them, received data may be overwritten.

A more complete UART driver can use:

```text
Interrupt
    |
    v
Ring Buffer
```

or:

```text
USART
  |
  v
DMA
  |
  v
RAM Buffer
```

The purpose of this example is to focus on:

```text
USART
RXNE
RXNEIE
NVIC
Vector Table
ISR
volatile
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

## Inspect Vector Table

```bash
arm-none-eabi-objdump \
-s \
-j .isr_vector \
uart_interrupt.elf
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
