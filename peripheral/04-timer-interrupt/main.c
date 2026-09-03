#include "stm32f103xb.h"

#define LED_PIN         13U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

/*
 * Default clock after reset:
 *
 * HSI = 8 MHz
 *
 * APB1 prescaler = 1
 *
 * TIM2 clock = 8 MHz
 *
 * Prescaler:
 *
 * 8 MHz / (7999 + 1)
 * =
 * 1000 Hz
 *
 * Auto-reload:
 *
 * 1000 Hz / (499 + 1)
 * =
 * 2 Hz
 *
 * Update interrupt occurs every:
 *
 * 1 / 2 Hz
 * =
 * 500 ms
 */
#define TIM2_PRESCALER  7999U
#define TIM2_AUTORELOAD 499U


static void led_init(void)
{
    /*
     * Enable GPIOC clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /*
     * Configure PC13 as:
     *
     * General-purpose push-pull output
     * 2 MHz
     */
    GPIOC->CRH &= ~PC13_CFG_MASK;
    GPIOC->CRH |= PC13_OUTPUT_PP;

    /*
     * PC13 HIGH -> LED OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);
}


static void led_toggle(void)
{
    /*
     * Blue Pill onboard LED is active-low.
     */
    if ((GPIOC->ODR & (1U << LED_PIN)) != 0U)
    {
        /*
         * Reset PC13 -> LOW -> LED ON.
         */
        GPIOC->BSRR =
            (1U << (LED_PIN + 16U));
    }
    else
    {
        /*
         * Set PC13 -> HIGH -> LED OFF.
         */
        GPIOC->BSRR =
            (1U << LED_PIN);
    }
}


static void tim2_init(void)
{
    /*
     * Enable TIM2 peripheral clock.
     *
     * TIM2 is connected to APB1.
     */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * Stop timer while configuring.
     */
    TIM2->CR1 = 0U;

    /*
     * Set prescaler.
     *
     * Timer input clock = 8 MHz
     *
     * Counter clock:
     *
     * 8 MHz / 8000 = 1 kHz
     */
    TIM2->PSC = TIM2_PRESCALER;

    /*
     * Counter counts from 0 to 499.
     *
     * 500 timer ticks at 1 kHz
     * =
     * 500 ms
     */
    TIM2->ARR = TIM2_AUTORELOAD;

    /*
     * Reset counter.
     */
    TIM2->CNT = 0U;

    /*
     * Generate update event so PSC and ARR are
     * loaded into the active timer registers.
     */
    TIM2->EGR = TIM_EGR_UG;

    /*
     * Generating an update event can set UIF.
     * Clear it before enabling the interrupt.
     */
    TIM2->SR &= ~TIM_SR_UIF;

    /*
     * Enable update interrupt.
     */
    TIM2->DIER |= TIM_DIER_UIE;

    /*
     * Clear any pending TIM2 IRQ in the NVIC.
     */
    NVIC_ClearPendingIRQ(TIM2_IRQn);

    /*
     * Enable TIM2 interrupt in the NVIC.
     */
    NVIC_EnableIRQ(TIM2_IRQn);

    /*
     * Start TIM2.
     */
    TIM2->CR1 |= TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
    /*
     * UIF is set when an update event occurs.
     */
    if ((TIM2->SR & TIM_SR_UIF) != 0U)
    {
        /*
         * Clear update interrupt flag.
         */
        TIM2->SR &= ~TIM_SR_UIF;

        /*
         * Toggle onboard LED.
         */
        led_toggle();
    }
}


int main(void)
{
    led_init();

    tim2_init();

    while (1)
    {
        /*
         * LED timing is handled entirely by
         * TIM2 interrupts.
         */
    }
}
