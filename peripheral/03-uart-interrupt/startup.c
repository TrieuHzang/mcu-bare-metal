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
 * USART1 interrupt handler.
 *
 * The application provides the real implementation.
 * This weak handler is used if no implementation exists.
 */
void USART1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));


/*
 * Vector table.
 *
 * USART1 is external IRQ number 37.
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
     * STM32F103 external IRQs.
     */
    (uintptr_t)Default_Handler,    /* IRQ 0  WWDG */
    (uintptr_t)Default_Handler,    /* IRQ 1  PVD */
    (uintptr_t)Default_Handler,    /* IRQ 2  TAMPER */
    (uintptr_t)Default_Handler,    /* IRQ 3  RTC */
    (uintptr_t)Default_Handler,    /* IRQ 4  FLASH */
    (uintptr_t)Default_Handler,    /* IRQ 5  RCC */
    (uintptr_t)Default_Handler,    /* IRQ 6  EXTI0 */
    (uintptr_t)Default_Handler,    /* IRQ 7  EXTI1 */
    (uintptr_t)Default_Handler,    /* IRQ 8  EXTI2 */
    (uintptr_t)Default_Handler,    /* IRQ 9  EXTI3 */
    (uintptr_t)Default_Handler,    /* IRQ 10 EXTI4 */

    (uintptr_t)Default_Handler,    /* IRQ 11 DMA1 Channel1 */
    (uintptr_t)Default_Handler,    /* IRQ 12 DMA1 Channel2 */
    (uintptr_t)Default_Handler,    /* IRQ 13 DMA1 Channel3 */
    (uintptr_t)Default_Handler,    /* IRQ 14 DMA1 Channel4 */
    (uintptr_t)Default_Handler,    /* IRQ 15 DMA1 Channel5 */
    (uintptr_t)Default_Handler,    /* IRQ 16 DMA1 Channel6 */
    (uintptr_t)Default_Handler,    /* IRQ 17 DMA1 Channel7 */

    (uintptr_t)Default_Handler,    /* IRQ 18 ADC1 ADC2 */
    (uintptr_t)Default_Handler,    /* IRQ 19 USB HP CAN TX */
    (uintptr_t)Default_Handler,    /* IRQ 20 USB LP CAN RX0 */
    (uintptr_t)Default_Handler,    /* IRQ 21 CAN RX1 */
    (uintptr_t)Default_Handler,    /* IRQ 22 CAN SCE */
    (uintptr_t)Default_Handler,    /* IRQ 23 EXTI9_5 */

    (uintptr_t)Default_Handler,    /* IRQ 24 TIM1 BRK */
    (uintptr_t)Default_Handler,    /* IRQ 25 TIM1 UP */
    (uintptr_t)Default_Handler,    /* IRQ 26 TIM1 TRG COM */
    (uintptr_t)Default_Handler,    /* IRQ 27 TIM1 CC */
    (uintptr_t)Default_Handler,    /* IRQ 28 TIM2 */
    (uintptr_t)Default_Handler,    /* IRQ 29 TIM3 */
    (uintptr_t)Default_Handler,    /* IRQ 30 TIM4 */

    (uintptr_t)Default_Handler,    /* IRQ 31 I2C1 EV */
    (uintptr_t)Default_Handler,    /* IRQ 32 I2C1 ER */
    (uintptr_t)Default_Handler,    /* IRQ 33 I2C2 EV */
    (uintptr_t)Default_Handler,    /* IRQ 34 I2C2 ER */

    (uintptr_t)Default_Handler,    /* IRQ 35 SPI1 */
    (uintptr_t)Default_Handler,    /* IRQ 36 SPI2 */

    (uintptr_t)USART1_IRQHandler   /* IRQ 37 USART1 */
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
     * Clear .bss.
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
