#include <stdint.h>

/*
 * RCC register layout
 */
typedef struct
{
    volatile uint32_t CR;        /* 0x00 */
    volatile uint32_t CFGR;      /* 0x04 */
    volatile uint32_t CIR;       /* 0x08 */
    volatile uint32_t APB2RSTR;  /* 0x0C */
    volatile uint32_t APB1RSTR;  /* 0x10 */
    volatile uint32_t AHBENR;    /* 0x14 */
    volatile uint32_t APB2ENR;   /* 0x18 */
    volatile uint32_t APB1ENR;   /* 0x1C */
    volatile uint32_t BDCR;      /* 0x20 */
    volatile uint32_t CSR;       /* 0x24 */
} RCC_TypeDef;

/*
 * GPIO register layout
 */
typedef struct
{
    volatile uint32_t CRL;       /* 0x00 */
    volatile uint32_t CRH;       /* 0x04 */
    volatile uint32_t IDR;       /* 0x08 */
    volatile uint32_t ODR;       /* 0x0C */
    volatile uint32_t BSRR;      /* 0x10 */
    volatile uint32_t BRR;       /* 0x14 */
    volatile uint32_t LCKR;      /* 0x18 */
} GPIO_TypeDef;

/*
 * SysTick register layout
 */
typedef struct
{
    volatile uint32_t CSR;       /* 0x00 */
    volatile uint32_t RVR;       /* 0x04 */
    volatile uint32_t CVR;       /* 0x08 */
    volatile uint32_t CALIB;     /* 0x0C */
} SysTick_TypeDef;


/*
 * Peripheral base addresses
 */
#define RCC_BASE        0x40021000U
#define GPIOC_BASE      0x40011000U
#define SYSTICK_BASE    0xE000E010U


/*
 * Peripheral instances
 */
#define RCC             ((RCC_TypeDef *)RCC_BASE)
#define GPIOC           ((GPIO_TypeDef *)GPIOC_BASE)
#define SYSTICK         ((SysTick_TypeDef *)SYSTICK_BASE)


/*
 * RCC bits
 */
#define RCC_IOPCEN      (1U << 4U)


/*
 * PC13 configuration
 */
#define LED_PIN         13U

#define PC13_CFG_SHIFT  20U
#define PC13_CFG_MASK   (0xFU << PC13_CFG_SHIFT)
#define PC13_OUTPUT_PP  (0x2U << PC13_CFG_SHIFT)


/*
 * SysTick control bits
 */
#define SYST_CSR_ENABLE     (1U << 0U)
#define SYST_CSR_TICKINT    (1U << 1U)
#define SYST_CSR_CLKSOURCE  (1U << 2U)


/*
 * Clock configuration
 */
#define CPU_CLOCK_HZ        8000000U
#define SYSTICK_HZ          1000U

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
     * Enable GPIOC clock.
     */
    RCC->APB2ENR |= RCC_IOPCEN;

    /*
     * Configure PC13 as
     * general-purpose push-pull output, 2 MHz.
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
    SYSTICK->CSR = 0U;

    /*
     * 1 ms period.
     */
    SYSTICK->RVR = SYSTICK_RELOAD;

    /*
     * Clear current value.
     */
    SYSTICK->CVR = 0U;

    /*
     * Use CPU clock.
     * Enable interrupt.
     * Enable SysTick.
     */
    SYSTICK->CSR =
        SYST_CSR_CLKSOURCE |
        SYST_CSR_TICKINT |
        SYST_CSR_ENABLE;
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
