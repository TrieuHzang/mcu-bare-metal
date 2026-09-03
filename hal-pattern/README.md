# HAL Patterns

Examples showing different ways to access and control STM32F103C8T6 hardware peripherals.

The application behavior is intentionally kept similar between examples so that the main difference is the hardware abstraction method.

## Examples

| Example | Description |
|---|---|
| [00-register-macro](00-register-macro/) | Direct memory-mapped register access using C macros |
| [01-register-struct](01-register-struct/) | Peripheral register access using C structures |
| [02-cmsis-device](02-cmsis-device/) | CMSIS Core and STM32F1 CMSIS Device register definitions |
| [03-hal-blocking](03-hal-blocking/) | STM32 HAL with blocking delay |
| [04-hal-nonblocking](04-hal-nonblocking/) | STM32 HAL with non-blocking timing |

## Progression

Register Macro
-> Register Struct
-> CMSIS Device
-> STM32 HAL

The first three examples remain close to the hardware registers.

The later examples introduce the STM32 HAL abstraction layer.
