/*******************************************************************************
 * @file   usart.c
 * @brief  USART source file.
 ******************************************************************************/

#include "usart.h"

/*===== Handles ==============================================================*/
static UART_HandleTypeDef huart2;

/*===== Private Function Prototypes ==========================================*/
static void hal_uart_init(UART_HandleTypeDef *huart, USART_TypeDef *instance);

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void usart_init(void)
{
    UART_HandleTypeDef *handle = NULL;
    USART_TypeDef *instance= NULL;

    /* USART2: NUCLEO-L433RC-P virtual COM port. */
    if ((usart_get_handle(USART_ID_NUCLEO_COM_PORT, &handle) == false)
    ||  (usart_get_instance(USART_ID_NUCLEO_COM_PORT, &instance) == false))
    {
        error_handler();
    }
    hal_uart_init(handle, instance);
}

bool usart_get_handle(USART_ID_t id, UART_HandleTypeDef **return_var)
{
    bool retval = false;

    switch (id)
    {
        case USART_ID_NUCLEO_COM_PORT:
            *return_var = &huart2;
            retval = true;
            break;
        default:
            retval = false;
            break;
    }

    return retval;
}

bool usart_get_instance(USART_ID_t id, USART_TypeDef **return_var)
{
    bool retval = false;

    switch (id)
    {
        case USART_ID_NUCLEO_COM_PORT:
            *return_var = USART2;
            retval = true;
            break;
        default:
            retval = false;
            break;
    }

    return retval;
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

static void hal_uart_init(UART_HandleTypeDef *huart, USART_TypeDef *instance)
{
    assert(huart);
    assert(instance);

    huart->Instance = instance;
    huart->Init.BaudRate = 115200;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        error_handler();
    }
}

/*============================================================================*/