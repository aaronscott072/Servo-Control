/*******************************************************************************
 * @file   gpio.c
 * @brief  GPIO source file.
 ******************************************************************************/

#include "gpio.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void gpio_init_output_pin(GPIO_TypeDef *port, 
                          uint16_t     pin, 
                          uint8_t      starting_state, 
                          uint32_t     mode,
                          uint32_t     push_pull,
                          uint32_t     speed)
{
    assert(IS_GPIO_ALL_INSTANCE(port));
    assert(IS_GPIO_PIN(pin));
    assert(IS_GPIO_PIN_ACTION(starting_state));
    assert(mode == GPIO_MODE_OUTPUT_PP || mode == GPIO_MODE_OUTPUT_OD);
    assert(IS_GPIO_PULL(push_pull));
    assert(IS_GPIO_SPEED(speed));

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO port clock enable. */
    if      (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (port == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
    else                     error_handler();

    /* Configure initial GPIO output level. */
    HAL_GPIO_WritePin(port, pin, starting_state);

    /* Configure GPIO pin. */
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = push_pull;
    GPIO_InitStruct.Speed = speed;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/*============================================================================*/