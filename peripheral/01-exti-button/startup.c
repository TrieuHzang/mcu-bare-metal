#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);

void Reset_Handler(void);
void Default_Handler(void);


/*
 * Cortex-M3 exception handlers.
 */
void NMI_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void HardFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void MemManage_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void BusFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void UsageFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SVC_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void DebugMon_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void PendSV_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SysTick_Handler(void)
    __attribute__((weak, alias("Default_Handler")));


/*
 * STM32F103 external interrupt handlers.
 *
 * EXTI0_IRQHandler is implemented by the application.
 * The weak definition is used if the application does
 * not provide its own handler.
 */
void WWDG_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void PVD_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TAMPER_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void RTC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void FLASH_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void RCC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI0_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));


/*
 * Cortex-M vector table.
 */
__attribute__((section(".isr_vector"), used))
const uintptr_t vector_table[] =
{
    (uintptr_t)&_estack,           /* 0  Initial MSP */
    (uintptr_t)Reset_Handler,      /* 1  Reset */
    (uintptr_t)NMI_Handler,        /* 2  NMI */
    (uintptr_t)HardFault_Handler,  /* 3  HardFault */
    (uintptr_t)MemManage_Handler,  /* 4  MemManage */
    (uintptr_t)BusFault_Handler,   /* 5  BusFault */
    (uintptr_t)UsageFault_Handler, /* 6  UsageFault */

    0U,                            /* 7  Reserved */
    0U,                            /* 8  Reserved */
    0U,                            /* 9  Reserved */
    0U,                            /* 10 Reserved */

    (uintptr_t)SVC_Handler,        /* 11 SVCall */
    (uintptr_t)DebugMon_Handler,   /* 12 Debug monitor */

    0U,                            /* 13 Reserved */

    (uintptr_t)PendSV_Handler,     /* 14 PendSV */
    (uintptr_t)SysTick_Handler,    /* 15 SysTick */

    /*
     * STM32F103 external interrupts.
     */
    (uintptr_t)WWDG_IRQHandler,    /* IRQ 0 */
    (uintptr_t)PVD_IRQHandler,     /* IRQ 1 */
    (uintptr_t)TAMPER_IRQHandler,  /* IRQ 2 */
    (uintptr_t)RTC_IRQHandler,     /* IRQ 3 */
    (uintptr_t)FLASH_IRQHandler,   /* IRQ 4 */
    (uintptr_t)RCC_IRQHandler,     /* IRQ 5 */
    (uintptr_t)EXTI0_IRQHandler    /* IRQ 6 */
};


void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /*
     * Copy initialized data from Flash to SRAM.
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
     * Clear the .bss section.
     */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst = 0U;
        dst++;
    }

    main();

    while (1)
    {
    }
}


void Default_Handler(void)
{
    while (1)
    {
    }
}
