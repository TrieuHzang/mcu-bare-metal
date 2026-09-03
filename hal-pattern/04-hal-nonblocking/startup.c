#include <stdint.h>

/*
 * Symbols provided by the linker script.
 */
extern uint32_t _estack;

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

/*
 * Application functions.
 */
extern int main(void);
extern void SysTick_Handler(void);

/*
 * Exception handlers.
 */
void Reset_Handler(void);
void Default_Handler(void);

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

/*
 * Cortex-M3 vector table.
 */
__attribute__((section(".isr_vector"), used))
void (*const vector_table[])(void) =
{
    (void (*)(void))(&_estack), /* Initial MSP */
    Reset_Handler,              /* Reset */
    NMI_Handler,                /* NMI */
    HardFault_Handler,          /* HardFault */
    MemManage_Handler,          /* MemManage */
    BusFault_Handler,           /* BusFault */
    UsageFault_Handler,         /* UsageFault */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    SVC_Handler,                /* SVCall */
    DebugMon_Handler,           /* Debug Monitor */
    0,                          /* Reserved */
    PendSV_Handler,             /* PendSV */
    SysTick_Handler             /* SysTick */
};

void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /*
     * Copy initialized .data variables
     * from Flash to RAM.
     */
    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
    {
        *dst = *src;
        src++;
        dst++;
    }

    /*
     * Clear .bss variables.
     */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst = 0U;
        dst++;
    }

    /*
     * Enter the application.
     */
    main();

    /*
     * main() should never return.
     */
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
