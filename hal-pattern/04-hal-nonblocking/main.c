#include "stm32f1xx_hal.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
}

static void gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &gpio);

    GPIOC->BSRR = GPIO_PIN_13;
}

int main(void)
{
    uint32_t last_toggle = 0U;
    uint32_t led_state = 0U;

    HAL_Init();

    gpio_init();

    while (1)
    {
        if ((uint32_t)(HAL_GetTick() - last_toggle) >= 500U)
        {
            last_toggle = HAL_GetTick();

            if (led_state == 0U)
            {
                HAL_GPIO_WritePin(
                    GPIOC,
                    GPIO_PIN_13,
                    GPIO_PIN_RESET
                );

                led_state = 1U;
            }
            else
            {
                HAL_GPIO_WritePin(
                    GPIOC,
                    GPIO_PIN_13,
                    GPIO_PIN_SET
                );

                led_state = 0U;
            }
        }
    }
}
