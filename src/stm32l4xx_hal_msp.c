/*******************************************************************************
 * @file   stm32l4xx_hal_msp.c
 * @brief  MSP initialisation and deinitialisation.
 ******************************************************************************/

#include "main.h"
#include "usart.h"

/**
 * @brief  Global MSP initialisation.
 * @retval None.
 */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init. */
    /* PendSV_IRQn interrupt configuration. */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    /* SysTick_IRQn interrupt configuration - Used by FreeRTOS system timer. */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
 * @brief  TIM PWM MSP initialisation.
 * @param  tim_pwmHandle: TIM PWM handle.
 * @retval None.
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *tim_pwmHandle)
{
    if (tim_pwmHandle->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

/**
 * @brief  TIM PWM MSP deinitialisation.
 * @param  tim_pwmHandle: TIM PWM handle.
 * @retval None.
 */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *tim_pwmHandle)
{
    if (tim_pwmHandle->Instance == TIM2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
}

/**
 * @brief  TIM MSP post initialisation.
 * @param  timHandle: TIM handle.
 * @retval None.
 */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (timHandle->Instance == TIM2)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**
         * TIM2 GPIO Configuration
         *     - PA0: TIM2_CH1
         */
        GPIO_InitStruct.Pin = GPIO_DEFS__PIN_SERVO_MOTOR_PWM;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIO_DEFS__PORT_SERVO_MOTOR_PWM, &GPIO_InitStruct);
    }
}

/**
 * @brief  UART/USART MSP initialisation.
 * @param  uartHandle: UART/USART handle.
 * @retval None.
 */
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    USART_TypeDef *uart_handle_instance = NULL;
    if (usart_get_instance(USART_ID__NUCLEO_COM_PORT, &uart_handle_instance) == false)
    {
        error_handler();
    }

    if (uartHandle->Instance == uart_handle_instance)
    {
        /* Initialises the peripheral's clock. */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
        {
            error_handler();
        }

        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();
        
        /**
         * USART2 GPIO Configuration
         *     - PA2: USART2_TX
         *     - PA3: USART2_RX
         */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        /* Tx. */
        GPIO_DEFS__CLK_EN_USART2_TX();
        GPIO_InitStruct.Pin = GPIO_DEFS__PIN_USART2_TX;
        HAL_GPIO_Init(GPIO_DEFS__PORT_USART2_TX, &GPIO_InitStruct);
        /* Rx. */
        GPIO_DEFS__CLK_EN_USART2_RX();
        GPIO_InitStruct.Pin = GPIO_DEFS__PIN_USART2_RX;
        HAL_GPIO_Init(GPIO_DEFS__PORT_USART2_RX, &GPIO_InitStruct);
    }
}

/**
 * @brief  UART/USART MSP deinitialisation.
 * @param  uartHandle: UART/USART handle.
 * @retval None.
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
    USART_TypeDef *uart_handle_instance = NULL;
    if (usart_get_instance(USART_ID__NUCLEO_COM_PORT, &uart_handle_instance) == false)
    {
        error_handler();
    }

    if (uartHandle->Instance == uart_handle_instance)
    {
        /* Peripheral clock disable. */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**
         * USART2 GPIO Configuration
         *     - PA2: USART2_TX
         *     - PA3: USART2_RX
         */
        HAL_GPIO_DeInit(GPIO_DEFS__PORT_USART2_TX, GPIO_DEFS__PIN_USART2_TX);
        HAL_GPIO_DeInit(GPIO_DEFS__PORT_USART2_RX, GPIO_DEFS__PIN_USART2_RX);
    }
}

/*============================================================================*/