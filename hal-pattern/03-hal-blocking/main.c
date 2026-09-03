#include "stm32f1xx_hal.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
}

static void gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    /*
     * Enable GPIOC peripheral clock.
     */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*
     * Configure PC13 as push-pull output.
     */
    gpio.Pin = GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &gpio);

    /*
     * PC13 HIGH -> LED OFF.
     */
    HAL_GPIO_WritePin(
        GPIOC,
        GPIO_PIN_13,
        GPIO_PIN_SET
    );
}

int main(void)
{
    /*
     * Initialize the STM32 HAL.
     *
     * HAL_Init() configures the HAL tick
     * using SysTick.
     */
    HAL_Init();

    gpio_init();

    while (1)
    {
        /*
         * PC13 LOW -> LED ON.
         */
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_13,
            GPIO_PIN_RESET
        );

        HAL_Delay(500U);

        /*
         * PC13 HIGH -> LED OFF.
         */
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_13,
            GPIO_PIN_SET
        );

        HAL_Delay(500U);
    }
}
