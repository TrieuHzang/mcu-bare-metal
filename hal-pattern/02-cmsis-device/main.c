#include "stm32f103xb.h"

/*
 * PC13 configuration
 */
#define LED_PIN         13U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

/*
 * System timing
 */
#define CPU_CLOCK_HZ    8000000U
#define SYSTICK_HZ      1000U

#define SYSTICK_RELOAD \
    ((CPU_CLOCK_HZ / SYSTICK_HZ) - 1U)


volatile uint32_t g_ms_ticks = 0U;


void SysTick_Handler(void)
{
    g_ms_ticks++;
}


static void gpio_init(void)
{
    /*
     * Enable GPIOC clock using CMSIS Device definitions.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /*
     * Configure PC13 as general-purpose
     * push-pull output, 2 MHz.
     */
    GPIOC->CRH &= ~PC13_CFG_MASK;
    GPIOC->CRH |= PC13_OUTPUT_PP;

    /*
     * PC13 HIGH -> LED OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);
}


static void systick_init(void)
{
    /*
     * Disable SysTick before configuration.
     */
    SysTick->CTRL = 0U;

    /*
     * Configure 1 ms period.
     */
    SysTick->LOAD = SYSTICK_RELOAD;

    /*
     * Clear current counter.
     */
    SysTick->VAL = 0U;

    /*
     * CPU clock.
     * SysTick interrupt.
     * Counter enable.
     */
    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk |
        SysTick_CTRL_ENABLE_Msk;
}


static void led_on(void)
{
    /*
     * Reset PC13.
     * PC13 LOW -> LED ON.
     */
    GPIOC->BSRR = (1U << (LED_PIN + 16U));
}


static void led_off(void)
{
    /*
     * Set PC13.
     * PC13 HIGH -> LED OFF.
     */
    GPIOC->BSRR = (1U << LED_PIN);
}


int main(void)
{
    uint32_t last_toggle = 0U;
    uint32_t led_state = 0U;

    gpio_init();
    systick_init();

    while (1)
    {
        if ((uint32_t)(g_ms_ticks - last_toggle) >= 500U)
        {
            last_toggle = g_ms_ticks;

            if (led_state == 0U)
            {
                led_on();
                led_state = 1U;
            }
            else
            {
                led_off();
                led_state = 0U;
            }
        }
    }
}
