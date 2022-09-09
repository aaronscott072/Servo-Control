/*******************************************************************************
 * @file   usart.h
 * @brief  USART header file.
 ******************************************************************************/

#ifndef USART_H
#define USART_H

#include "main.h"

/*===== Typedefs =============================================================*/
typedef enum USART_ID_t {
    USART_ID_NUCLEO_COM_PORT,
    /* @note: Add future USART's as required. */
} USART_ID_t;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Initialise UART/USART.
 * 
 *             (+) USART2: NUCLEO-L433RC-P virtual COM port.
 * 
 * @retval None.
 */
void usart_init(void);

/**
 * @brief  Retrieve a pointer to the HAL USART handle for the specified USART ID.
 * 
 *         Example usage:
 *             UART_HandleTypeDef *p_handle = NULL;
 *             bool ret = usart_get_handle(USART_ID, &p_handle);
 * 
 * @param  id:         USART ID; see @ref USART_ID_t for options.
 * @param  return_var: The requested handle pointer. Passed by reference.
 * @retval Boolean indicating whether a relevant handle pointer was found and
 *         passed to @param return_var.
 */
bool usart_get_handle(USART_ID_t id, UART_HandleTypeDef **return_var);

/**
 * @brief  Retrieve a pointer to the HAL USART handle instance for the specified USART ID.
 * 
 *         Example usage:
 *             USART_TypeDef *p_instance= NULL;
 *             bool ret = usart_get_instance(USART_ID, &p_instance);
 * 
 * @param  id:         USART ID; see @ref USART_ID_t for options.
 * @param  return_var: The requested handle instance pointer. Passed by reference.
 * @retval Boolean indicating whether a relevant handle instance pointer was found and
 *         passed to @param return_var.
 */
bool usart_get_instance(USART_ID_t id, USART_TypeDef **return_var);

/*============================================================================*/

#endif /* USART_H ============================================================*/