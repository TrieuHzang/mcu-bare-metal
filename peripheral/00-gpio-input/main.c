#include "stm32f103xb.h"

/*
 * Blue Pill onboard LED
 */
#define LED_PIN         13U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

/*
 * External button on PA0
 *
 * PA0 is configured as input with internal pull-up.
 *
 * Button released -> PA0 HIGH
 * Button pressed  -> PA0 LOW
 */
#define BUTTON_PIN      0U

#define PA0_CFG_SHIFT   0U
#define PA0_CFG_MASK    (0xFU << PA0_CFG_SHIFT)

/*
 * STM32F1 GPIO input pull-up / pull-down:
 *
 * MODE = 00
 * CNF  = 10
 *
 * Configuration nibble = 1000 = 0x8
 */
#define PA0_INPUT_PUPD  (0x8U << PA0_CFG_SHIFT)


static void gpio_init(void)
{
    /*
     * Enable GPIOA and GPIOC peripheral clocks.
     */
    RCC->APB2ENR |=
        RCC_APB2ENR_IOPAEN |
        RCC_APB2ENR_IOPCEN;

    /*
     * Configure PC13 as:
     *
     * General-purpose push-pull output
     * Maximum output speed 2 MHz
     */
    GPIOC->CRH &= ~PC13_CFG_MASK;
    GPIOC->CRH |= PC13_OUTPUT_PP;

    /*
     * PC13 HIGH -> LED OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);

    /*
     * Configure PA0 as input pull-up / pull-down.
     */
    GPIOA->CRL &= ~PA0_CFG_MASK;
    GPIOA->CRL |= PA0_INPUT_PUPD;

    /*
     * For input pull-up / pull-down mode:
     *
     * ODR bit = 1 -> pull-up
     * ODR bit = 0 -> pull-down
     *
     * Select internal pull-up for PA0.
     */
    GPIOA->BSRR = (1U << BUTTON_PIN);
}


static uint32_t button_is_pressed(void)
{
    /*
     * Read PA0 from GPIOA input data register.
     *
     * Internal pull-up:
     *
     * HIGH -> released
     * LOW  -> pressed
     */
    if ((GPIOA->IDR & (1U << BUTTON_PIN)) == 0U)
    {
        return 1U;
    }

    return 0U;
}


static void led_on(void)
{
    /*
     * PC13 LOW -> LED ON.
     */
    GPIOC->BSRR = (1U << (LED_PIN + 16U));
}


static void led_off(void)
{
    /*
     * PC13 HIGH -> LED OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);
}


int main(void)
{
    gpio_init();

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
}
