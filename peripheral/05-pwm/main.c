#include "stm32f103xb.h"

/*
 * Default STM32F103 reset clock:
 *
 * HSI = 8 MHz
 *
 * APB1 prescaler = 1
 * TIM2 clock     = 8 MHz
 *
 * Prescaler:
 *
 * 8 MHz / (7 + 1)
 * =
 * 1 MHz
 *
 * PWM period:
 *
 * 1 MHz / (999 + 1)
 * =
 * 1 kHz
 *
 * CCR1 = 500
 *
 * Duty cycle:
 *
 * 500 / 1000
 * =
 * 50%
 */
#define TIM2_PRESCALER      7U
#define TIM2_AUTORELOAD     999U
#define TIM2_CCR1_VALUE     500U


/*
 * PA0 is TIM2 Channel 1 with the default
 * STM32F103 alternate-function mapping.
 *
 * PA0 uses GPIOA_CRL bits [3:0].
 *
 * MODE = 11 -> output 50 MHz
 * CNF  = 10 -> alternate-function push-pull
 *
 * CNF MODE = 1011 = 0xB
 */
#define PA0_CFG_SHIFT       0U
#define PA0_CFG_MASK        (0xFU << PA0_CFG_SHIFT)
#define PA0_TIM2_CH1        (0xBU << PA0_CFG_SHIFT)


static void gpio_init(void)
{
    /*
     * Enable GPIOA peripheral clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /*
     * Configure PA0 as alternate-function
     * push-pull output.
     *
     * TIM2 Channel 1 will control this pin.
     */
    GPIOA->CRL &= ~PA0_CFG_MASK;
    GPIOA->CRL |= PA0_TIM2_CH1;
}


static void tim2_pwm_init(void)
{
    /*
     * Enable TIM2 clock.
     *
     * TIM2 is connected to APB1.
     */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * Stop TIM2 while configuring.
     */
    TIM2->CR1 = 0U;

    /*
     * Timer counter clock:
     *
     * 8 MHz / (7 + 1)
     * =
     * 1 MHz
     */
    TIM2->PSC = TIM2_PRESCALER;

    /*
     * PWM frequency:
     *
     * 1 MHz / (999 + 1)
     * =
     * 1 kHz
     */
    TIM2->ARR = TIM2_AUTORELOAD;

    /*
     * Channel 1 compare value.
     *
     * 500 / 1000
     * =
     * 50% duty cycle
     */
    TIM2->CCR1 = TIM2_CCR1_VALUE;

    /*
     * Configure TIM2 Channel 1 as output.
     *
     * CC1S = 00
     * -> Channel 1 configured as output.
     *
     * OC1M = 110
     * -> PWM mode 1.
     *
     * OC1PE = 1
     * -> Enable preload for CCR1.
     */
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |=
        TIM_CCMR1_OC1M_1 |
        TIM_CCMR1_OC1M_2 |
        TIM_CCMR1_OC1PE;

    /*
     * Enable TIM2 Channel 1 output.
     *
     * CC1P = 0
     * -> active high.
     */
    TIM2->CCER &= ~TIM_CCER_CC1P;
    TIM2->CCER |= TIM_CCER_CC1E;

    /*
     * Enable ARR preload.
     */
    TIM2->CR1 |= TIM_CR1_ARPE;

    /*
     * Reset counter.
     */
    TIM2->CNT = 0U;

    /*
     * Generate an update event.
     *
     * This loads PSC, ARR and CCR preload values.
     */
    TIM2->EGR = TIM_EGR_UG;

    /*
     * Start timer.
     */
    TIM2->CR1 |= TIM_CR1_CEN;
}


int main(void)
{
    gpio_init();

    tim2_pwm_init();

    while (1)
    {
        /*
         * PWM generation is handled entirely
         * by TIM2 hardware.
         */
    }
}
