/*******************************************************************************
 * @file   rtos.c
 * @brief  RTOS components (tasks/queues/locks/etc.) initialisation source file.
 ******************************************************************************/

#include "rtos.h"
#include "usart.h"

/*===== Defines ==============================================================*/
/*===== Task Delays =====*/
#define TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_INTERFACE   1000
/*===== Task Priorities =====*/
#define TASK_PRIORITY__TASK_NUCLEO_COM_PORT_INTERFACE   1 // @todo: priority labels?

/*===== Private Function Prototypes ==========================================*/
static void task_nucleo_com_port_interface(void *params);

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void rtos_init(void)
{
    // @todo: create a FreeRTOS wrapper (to avoid duplicate code)

    BaseType_t ret = xTaskCreate(task_nucleo_com_port_interface,
                                 "task_nucleo_com_port_interface",
                                 configMINIMAL_STACK_SIZE,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_NUCLEO_COM_PORT_INTERFACE,
                                 0);
    if (ret != pdPASS)
    {
        /* Error - Task not created (insufficient memory allocation). */
        error_handler();
    }
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/*===== FreeRTOS Tasks =======================================================*/

/**
 * @brief  RTOS task ---
 *         Nucleo COM port interface.
 * @param  params: unused.
 * @retval None.
 */
static void task_nucleo_com_port_interface(void *params __attribute__((unused)))
{
    UART_HandleTypeDef *handle = NULL;
    if (usart_get_handle(USART_ID_NUCLEO_COM_PORT, &handle) == false)
    {
        error_handler();
    }

    while (1)
    {
        // @todo: wrap Tx; move to usart.c/h
        char data[100] = "Nucleo USART Tx\r\n";
        HAL_UART_Transmit(handle, (uint8_t*)data, sizeof(data), 1000);

        /* Block (task delay). */
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_INTERFACE));
    }
}

/*============================================================================*/