#include "stm32f103xb.h"

#define LED_PIN         13U
#define BUTTON_PIN      0U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

#define PA0_CFG_SHIFT   0U
#define PA0_CFG_MASK    (0xFU << PA0_CFG_SHIFT)
#define PA0_INPUT_PUPD  (0x8U << PA0_CFG_SHIFT)


static void led_toggle(void)
{
    /*
     * PC13 is active-low.
     *
     * ODR = 1 -> LED OFF
     * ODR = 0 -> LED ON
     */
    if ((GPIOC->ODR & (1U << LED_PIN)) != 0U)
    {
        /*
         * Reset PC13 -> LOW -> LED ON.
         */
        GPIOC->BSRR = (1U << (LED_PIN + 16U));
    }
    else
    {
        /*
         * Set PC13 -> HIGH -> LED OFF.
         */
        GPIOC->BSRR = (1U << LED_PIN);
    }
}


static void gpio_init(void)
{
    /*
     * Enable:
     *
     * AFIO
     * GPIOA
     * GPIOC
     */
    RCC->APB2ENR |=
        RCC_APB2ENR_AFIOEN |
        RCC_APB2ENR_IOPAEN |
        RCC_APB2ENR_IOPCEN;

    /*
     * PC13:
     *
     * General-purpose push-pull output
     * 2 MHz
     */
    GPIOC->CRH &= ~PC13_CFG_MASK;
    GPIOC->CRH |= PC13_OUTPUT_PP;

    /*
     * LED initially OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);

    /*
     * PA0:
     *
     * Input pull-up / pull-down.
     */
    GPIOA->CRL &= ~PA0_CFG_MASK;
    GPIOA->CRL |= PA0_INPUT_PUPD;

    /*
     * Select internal pull-up.
     */
    GPIOA->BSRR = (1U << BUTTON_PIN);
}


static void exti0_init(void)
{
    /*
     * EXTI0 can be connected to:
     *
     * PA0
     * PB0
     * PC0
     * ...
     *
     * EXTICR1 bits [3:0] select the GPIO port.
     *
     * 0000 = PA0
     */
    AFIO->EXTICR[0] &= ~(0xFU << 0U);

    /*
     * Unmask EXTI line 0.
     */
    EXTI->IMR |= EXTI_IMR_MR0;

    /*
     * Disable rising-edge trigger.
     */
    EXTI->RTSR &= ~EXTI_RTSR_TR0;

    /*
     * Enable falling-edge trigger.
     *
     * Button:
     *
     * released = HIGH
     * pressed  = LOW
     *
     * HIGH -> LOW = falling edge
     */
    EXTI->FTSR |= EXTI_FTSR_TR0;

    /*
     * Clear any old pending EXTI0 interrupt.
     *
     * EXTI pending bits are cleared by writing 1.
     */
    EXTI->PR = EXTI_PR_PR0;

    /*
     * Clear NVIC pending state before enabling.
     */
    NVIC_ClearPendingIRQ(EXTI0_IRQn);

    /*
     * Enable EXTI0 interrupt in the NVIC.
     */
    NVIC_EnableIRQ(EXTI0_IRQn);
}


void EXTI0_IRQHandler(void)
{
    /*
     * Check whether EXTI line 0 caused the interrupt.
     */
    if ((EXTI->PR & EXTI_PR_PR0) != 0U)
    {
        /*
         * Clear pending flag first.
         */
        EXTI->PR = EXTI_PR_PR0;

        /*
         * Toggle onboard LED.
         */
        led_toggle();
    }
}


int main(void)
{
    gpio_init();

    exti0_init();

    while (1)
    {
        /*
         * No GPIO polling here.
         *
         * Button processing happens inside
         * EXTI0_IRQHandler().
         */
    }
}
