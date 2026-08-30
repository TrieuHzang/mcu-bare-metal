#include <stdint.h>
#include "led_blink.h"

/*
 * Symbols defined by the linker script.
 */
extern uint32_t _estack;

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

/*
 * Function declarations.
 */
int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/*
 * Cortex-M3 vector table.
 *
 * Entry 0: Initial Main Stack Pointer
 * Entry 1: Reset Handler
 * Entry 2...: Core exception handlers
 */
__attribute__((section(".isr_vector"), used))
void (*const vector_table[])(void) =
{
    (void (*)(void))(&_estack), /* Initial Stack Pointer */
    Reset_Handler,              /* Reset */
    Default_Handler,            /* NMI */
    Default_Handler,            /* HardFault */
    Default_Handler,            /* MemManage */
    Default_Handler,            /* BusFault */
    Default_Handler,            /* UsageFault */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    Default_Handler,            /* SVCall */
    Default_Handler,            /* Debug Monitor */
    0,                          /* Reserved */
    Default_Handler,            /* PendSV */
    Default_Handler             /* SysTick */
};

/*
 * Simple busy-wait delay.
 */
static void delay(volatile uint32_t count)
{
    while (count > 0U)
    {
        __asm volatile ("nop");
        count--;
    }
}

/*
 * Reset Handler
 *
 * This is the first C function executed after reset.
 */
void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /*
     * Copy initialized variables (.data)
     * from Flash to RAM.
     */
    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
    {
        *dst = *src;

        dst++;
        src++;
    }

    /*
     * Initialize zero-initialized variables (.bss)
     * to zero.
     */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst = 0U;
        dst++;
    }

    /*
     * Enter application.
     */
    main();

    /*
     * main() should never return.
     */
    while (1)
    {
    }
}

/*
 * Default exception handler.
 */
void Default_Handler(void)
{
    while (1)
    {
    }
}

/*
 * Application entry point.
 */
int main(void)
{
    /*
     * Enable GPIOC peripheral clock.
     */
    RCC_APB2ENR |= RCC_IOPCEN;

    /*
     * Configure PC13:
     *
     * CNF13  = 00 -> General-purpose push-pull
     * MODE13 = 10 -> Output mode, max speed 2 MHz
     */
    GPIOC_CRH &= ~PC13_CFG_MASK;
    GPIOC_CRH |= PC13_OUTPUT_PP;

    /*
     * PC13 HIGH -> LED OFF
     */
    GPIOC_BSRR = LED_PIN_MASK;

    while (1)
    {
        /*
         * PC13 LOW -> LED ON
         */
        GPIOC_BRR = LED_PIN_MASK;

        delay(500000U);

        /*
         * PC13 HIGH -> LED OFF
         */
        GPIOC_BSRR = LED_PIN_MASK;

        delay(500000U);
    }
}
