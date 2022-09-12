/*******************************************************************************
 * @file   gpio.c
 * @brief  GPIO source file.
 ******************************************************************************/

#include "gpio.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void gpio_init_output_pin(GPIO_CONFIG_t gpio_config)
{
    assert(IS_GPIO_ALL_INSTANCE(gpio_config.port));
    assert(IS_GPIO_PIN(gpio_config.pin));
    assert(IS_GPIO_PIN_ACTION(gpio_config.starting_state));
    assert(gpio_config.mode == GPIO_MODE_OUTPUT_PP || gpio_config.mode == GPIO_MODE_OUTPUT_OD);
    assert(IS_GPIO_PULL(gpio_config.push_pull));
    assert(IS_GPIO_SPEED(gpio_config.speed));

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO port clock enable. */
    if      (gpio_config.port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (gpio_config.port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (gpio_config.port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (gpio_config.port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (gpio_config.port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (gpio_config.port == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
    else                     error_handler();

    /* Configure initial GPIO output level. */
    HAL_GPIO_WritePin(gpio_config.port, gpio_config.pin, gpio_config.starting_state);

    /* Configure GPIO pin. */
    GPIO_InitStruct.Pin = gpio_config.pin;
    GPIO_InitStruct.Mode = gpio_config.mode;
    GPIO_InitStruct.Pull = gpio_config.push_pull;
    GPIO_InitStruct.Speed = gpio_config.speed;
    HAL_GPIO_Init(gpio_config.port, &GPIO_InitStruct);
}

/*============================================================================*/