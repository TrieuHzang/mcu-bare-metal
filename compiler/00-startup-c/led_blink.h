#ifndef LED_BLINK_H
#define LED_BLINK_H

#include <stdint.h>

/*
 * STM32F103C8T6 peripheral base addresses
 */
#define RCC_BASE        0x40021000U
#define GPIOC_BASE      0x40011000U

/*
 * RCC registers
 */
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18U))

/*
 * GPIOC registers
 */
#define GPIOC_CRH       (*(volatile uint32_t *)(GPIOC_BASE + 0x04U))
#define GPIOC_BSRR      (*(volatile uint32_t *)(GPIOC_BASE + 0x10U))
#define GPIOC_BRR       (*(volatile uint32_t *)(GPIOC_BASE + 0x14U))

/*
 * GPIOC clock enable
 * RCC_APB2ENR bit 4 = IOPCEN
 */
#define RCC_IOPCEN      (1U << 4U)

/*
 * Blue Pill onboard LED
 */
#define LED_PIN         13U
#define LED_PIN_MASK    (1U << LED_PIN)

/*
 * PC13 configuration in GPIOC_CRH
 *
 * PC13 uses bits 23:20:
 *
 * CNF13  = 00 : General-purpose push-pull
 * MODE13 = 10 : Output mode, max speed 2 MHz
 */
#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

#endif
