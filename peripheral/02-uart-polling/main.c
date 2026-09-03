#include "stm32f103xb.h"

/*
 * STM32F103 reset clock:
 *
 * HSI = 8 MHz
 *
 * USART1 is connected to APB2, so with the default
 * clock configuration:
 *
 * PCLK2 = 8 MHz
 */
#define PCLK2_HZ       8000000U
#define USART_BAUD     115200U

/*
 * With oversampling by 16 on STM32F1:
 *
 * BRR ~= PCLK / baud
 *
 * 8000000 / 115200 ~= 69
 *
 * 69 decimal = 0x45
 */
#define USART1_BRR_VALUE \
    ((PCLK2_HZ + (USART_BAUD / 2U)) / USART_BAUD)


/*
 * PA9 is located in GPIOA_CRH bits [7:4].
 *
 * USART1 TX:
 *
 * MODE = 11 -> output 50 MHz
 * CNF  = 10 -> alternate function push-pull
 *
 * CNF MODE = 1011 = 0xB
 */
#define PA9_CFG_SHIFT      4U
#define PA9_CFG_MASK       (0xFU << PA9_CFG_SHIFT)
#define PA9_USART_TX       (0xBU << PA9_CFG_SHIFT)


/*
 * PA10 is located in GPIOA_CRH bits [11:8].
 *
 * USART1 RX:
 *
 * MODE = 00 -> input
 * CNF  = 01 -> floating input
 *
 * CNF MODE = 0100 = 0x4
 */
#define PA10_CFG_SHIFT     8U
#define PA10_CFG_MASK      (0xFU << PA10_CFG_SHIFT)
#define PA10_USART_RX      (0x4U << PA10_CFG_SHIFT)


static void uart1_gpio_init(void)
{
    /*
     * Enable GPIOA peripheral clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /*
     * Configure PA9 as USART1 TX:
     *
     * Alternate function push-pull output.
     */
    GPIOA->CRH &= ~PA9_CFG_MASK;
    GPIOA->CRH |= PA9_USART_TX;

    /*
     * Configure PA10 as USART1 RX:
     *
     * Floating input.
     */
    GPIOA->CRH &= ~PA10_CFG_MASK;
    GPIOA->CRH |= PA10_USART_RX;
}


static void uart1_init(void)
{
    /*
     * Enable USART1 peripheral clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /*
     * Disable USART while configuring it.
     */
    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;

    /*
     * Set baud rate.
     *
     * PCLK2 = 8 MHz
     * Baud  = 115200
     */
    USART1->BRR = USART1_BRR_VALUE;

    /*
     * Enable:
     *
     * TE = transmitter
     * RE = receiver
     * UE = USART
     */
    USART1->CR1 =
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_UE;
}


static void uart1_write_byte(uint8_t data)
{
    /*
     * Wait until the transmit data register is empty.
     */
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    /*
     * Write byte to USART data register.
     */
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


static uint8_t uart1_read_byte(void)
{
    /*
     * Wait until a byte has been received.
     */
    while ((USART1->SR & USART_SR_RXNE) == 0U)
    {
    }

    /*
     * Reading DR clears RXNE.
     */
    return (uint8_t)(USART1->DR & 0xFFU);
}


int main(void)
{
    uint8_t received_byte;

    uart1_gpio_init();

    uart1_init();

    uart1_write_string(
        "Hello from STM32F103\r\n"
    );

    uart1_write_string(
        "UART polling echo ready\r\n"
    );

    while (1)
    {
        received_byte = uart1_read_byte();

        uart1_write_byte(received_byte);
    }
}
