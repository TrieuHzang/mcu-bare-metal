#include "stm32f103xb.h"

#define PCLK2_HZ       8000000U
#define USART_BAUD     115200U

#define USART1_BRR_VALUE \
    ((PCLK2_HZ + (USART_BAUD / 2U)) / USART_BAUD)


#define PA9_CFG_SHIFT      4U
#define PA9_CFG_MASK       (0xFU << PA9_CFG_SHIFT)
#define PA9_USART_TX       (0xBU << PA9_CFG_SHIFT)

#define PA10_CFG_SHIFT     8U
#define PA10_CFG_MASK      (0xFU << PA10_CFG_SHIFT)
#define PA10_USART_RX      (0x4U << PA10_CFG_SHIFT)


/*
 * Shared between main() and USART1_IRQHandler().
 *
 * volatile is required because these variables can
 * change asynchronously inside an interrupt handler.
 */
static volatile uint8_t g_rx_byte = 0U;
static volatile uint8_t g_rx_ready = 0U;


static void uart1_gpio_init(void)
{
    /*
     * Enable GPIOA clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /*
     * PA9:
     * USART1 TX
     * Alternate-function push-pull
     * 50 MHz output mode
     */
    GPIOA->CRH &= ~PA9_CFG_MASK;
    GPIOA->CRH |= PA9_USART_TX;

    /*
     * PA10:
     * USART1 RX
     * Floating input
     */
    GPIOA->CRH &= ~PA10_CFG_MASK;
    GPIOA->CRH |= PA10_USART_RX;
}


static void uart1_write_byte(uint8_t data)
{
    /*
     * TX remains polling in this example.
     */
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    USART1->DR = data;
}


static void uart1_write_string(const char *str)
{
    while (*str != '\0')
    {
        uart1_write_byte((uint8_t)*str);
        str++;
    }
}


static void uart1_init(void)
{
    /*
     * Enable USART1 peripheral clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /*
     * Disable and clear USART configuration
     * while setting it up.
     */
    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;

    /*
     * 8 MHz PCLK2, 115200 baud.
     */
    USART1->BRR = USART1_BRR_VALUE;

    /*
     * Clear any pending USART1 IRQ state
     * in the NVIC before enabling it.
     */
    NVIC_ClearPendingIRQ(USART1_IRQn);

    /*
     * Enable:
     *
     * RXNEIE = RX register not empty interrupt
     * TE     = transmitter
     * RE     = receiver
     * UE     = USART peripheral
     */
    USART1->CR1 =
        USART_CR1_RXNEIE |
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_UE;

    /*
     * Allow USART1 interrupts to reach the CPU.
     */
    NVIC_EnableIRQ(USART1_IRQn);
}


void USART1_IRQHandler(void)
{
    /*
     * RXNE becomes 1 when a new byte arrives.
     */
    if ((USART1->SR & USART_SR_RXNE) != 0U)
    {
        /*
         * Reading DR clears RXNE.
         */
        g_rx_byte =
            (uint8_t)(USART1->DR & 0xFFU);

        g_rx_ready = 1U;
    }
}


int main(void)
{
    uint8_t data;

    uart1_gpio_init();

    uart1_init();

    uart1_write_string(
        "Hello from STM32F103\r\n"
    );

    uart1_write_string(
        "UART interrupt echo ready\r\n"
    );

    while (1)
    {
        /*
         * USART hardware is NOT polled here.
         *
         * USART1_IRQHandler() receives the byte.
         */
        if (g_rx_ready != 0U)
        {
            /*
             * Protect the shared byte and flag while
             * copying them from the interrupt context.
             */
            NVIC_DisableIRQ(USART1_IRQn);

            data = g_rx_byte;
            g_rx_ready = 0U;

            NVIC_EnableIRQ(USART1_IRQn);

            /*
             * TX is still implemented using polling.
             */
            uart1_write_byte(data);
        }
    }
}
