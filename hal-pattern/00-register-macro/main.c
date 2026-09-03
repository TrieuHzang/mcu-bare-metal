#include <stdint.h>

/*
 * STM32F103C8T6 peripheral registers
 */

/* RCC */
#define RCC_APB2ENR     (*(volatile uint32_t *)0x40021018U)

/* GPIOC */
#define GPIOC_CRH       (*(volatile uint32_t *)0x40011004U)
#define GPIOC_BSRR      (*(volatile uint32_t *)0x40011010U)

/* SysTick */
#define SYST_CSR        (*(volatile uint32_t *)0xE000E010U)
#define SYST_RVR        (*(volatile uint32_t *)0xE000E014U)
#define SYST_CVR        (*(volatile uint32_t *)0xE000E018U)

/* RCC_APB2ENR bit 4 */
#define RCC_IOPCEN      (1U << 4U)

/* PC13 */
#define LED_PIN         13U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)

/* SysTick CTRL bits */
#define SYST_CSR_ENABLE     (1U << 0U)
#define SYST_CSR_TICKINT    (1U << 1U)
#define SYST_CSR_CLKSOURCE  (1U << 2U)

/*
 * STM32F103 uses the 8 MHz HSI clock
 * immediately after reset.
 */
#define CPU_CLOCK_HZ        8000000U
#define SYSTICK_HZ          1000U

#define SYSTICK_RELOAD \
    ((CPU_CLOCK_HZ / SYSTICK_HZ) - 1U)

/*
 * Millisecond counter updated by SysTick interrupt.
 */
volatile uint32_t g_ms_ticks = 0U;

void SysTick_Handler(void)
{
    g_ms_ticks++;
}

static void gpio_init(void)
{
    /*
     * Enable GPIOC clock.
     */
    RCC_APB2ENR |= RCC_IOPCEN;

    /*
     * Clear PC13 configuration.
     */
    GPIOC_CRH &= ~PC13_CFG_MASK;

    /*
     * PC13:
     * CNF13  = 00
     * MODE13 = 10
     *
     * General-purpose push-pull output, 2 MHz.
     */
    GPIOC_CRH |= PC13_OUTPUT_PP;

    /*
     * PC13 HIGH -> LED OFF
     */
    GPIOC_BSRR = (1U << LED_PIN);
}

static void systick_init(void)
{
    /*
     * Disable SysTick before configuration.
     */
    SYST_CSR = 0U;

    /*
     * 8 MHz / 1000 Hz = 8000 cycles per millisecond.
     *
     * Reload = 8000 - 1 = 7999.
     */
    SYST_RVR = SYSTICK_RELOAD;

    /*
     * Clear current value.
     */
    SYST_CVR = 0U;

    /*
     * Use CPU clock.
     * Enable SysTick interrupt.
     * Enable SysTick counter.
     */
    SYST_CSR =
        SYST_CSR_CLKSOURCE |
        SYST_CSR_TICKINT |
        SYST_CSR_ENABLE;
}

static void led_on(void)
{
    /*
     * BSRR bits 16..31 reset output.
     * PC13 LOW -> LED ON.
     */
    GPIOC_BSRR = (1U << (LED_PIN + 16U));
}

static void led_off(void)
{
    /*
     * BSRR bits 0..15 set output.
     * PC13 HIGH -> LED OFF.
     */
    GPIOC_BSRR = (1U << LED_PIN);
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
