#include <stdint.h>

/*
 * STM32F103C8T6
 *
 * After reset, the MCU uses the internal HSI clock:
 *
 *     HSI = 8 MHz
 *
 * This example does not configure PLL or switch to 72 MHz.
 */

/* RCC */
#define RCC_APB2ENR        (*(volatile uint32_t *)0x40021018U)
#define RCC_IOPCEN         (1U << 4)

/* GPIOC */
#define GPIOC_CRH          (*(volatile uint32_t *)0x40011004U)
#define GPIOC_ODR          (*(volatile uint32_t *)0x4001100CU)

/*
 * Cortex-M3 SysTick registers
 */
#define SYST_CSR           (*(volatile uint32_t *)0xE000E010U)
#define SYST_RVR           (*(volatile uint32_t *)0xE000E014U)
#define SYST_CVR           (*(volatile uint32_t *)0xE000E018U)

/* SysTick CTRL bits */
#define SYST_CSR_ENABLE    (1U << 0)
#define SYST_CSR_TICKINT   (1U << 1)
#define SYST_CSR_CLKSOURCE (1U << 2)

#define CPU_CLOCK_HZ       8000000U
#define SYSTICK_HZ         1000U

#define LED_PIN            13U

volatile uint32_t g_ms_ticks = 0U;

static void gpio_init(void)
{
    /*
     * Enable GPIOC clock.
     */
    RCC_APB2ENR |= RCC_IOPCEN;

    /*
     * PC13 is in GPIOC_CRH.
     *
     * Each GPIO pin uses 4 configuration bits.
     *
     * PC13 -> bits 23:20
     *
     * MODE13 = 10 -> Output mode, max speed 2 MHz
     * CNF13  = 00 -> General purpose push-pull
     *
     * Therefore the nibble is 0b0010 = 0x2.
     */
    GPIOC_CRH &= ~(0xFU << 20);
    GPIOC_CRH |=  (0x2U << 20);

    /*
     * Blue Pill onboard LED on PC13 is active-low.
     *
     * Set PC13 high initially -> LED OFF.
     */
    GPIOC_ODR |= (1U << LED_PIN);
}

static void led_toggle(void)
{
    GPIOC_ODR ^= (1U << LED_PIN);
}

static void systick_init(void)
{
    /*
     * CPU clock = 8 MHz
     *
     * We want:
     *
     *     1000 interrupts / second
     *
     * Therefore:
     *
     *     8,000,000 / 1000 = 8000 clocks
     *
     * SysTick counts from LOAD down to zero,
     * therefore:
     *
     *     LOAD = 8000 - 1 = 7999
     */

    SYST_RVR = (CPU_CLOCK_HZ / SYSTICK_HZ) - 1U;

    /*
     * Writing any value to CURRENT clears the counter.
     */
    SYST_CVR = 0U;

    /*
     * CLKSOURCE = CPU clock
     * TICKINT   = generate interrupt
     * ENABLE    = start SysTick
     */
    SYST_CSR =
        SYST_CSR_CLKSOURCE |
        SYST_CSR_TICKINT   |
        SYST_CSR_ENABLE;
}

void SysTick_Handler(void)
{
    g_ms_ticks++;
}

int main(void)
{
    uint32_t previous_tick = 0U;

    gpio_init();
    systick_init();

    while (1)
    {
        /*
         * Toggle LED every 500 ms.
         *
         * No busy-wait delay is used.
         */
        if ((g_ms_ticks - previous_tick) >= 500U)
        {
            previous_tick += 500U;
            led_toggle();
        }
    }
}
