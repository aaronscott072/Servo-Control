/*******************************************************************************
 * @file   freertos.c
 * @brief  FreeRTOS initialisation source file.
 ******************************************************************************/

#include "main.h"

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
    UNUSED(xTask);
    UNUSED(pcTaskName);

    error_handler();
}

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @brief  RTOS task ---
 *         Default.
 * @param  params: Unused.
 * @retval None.
 */
void StartDefaultTask(void *params __attribute__((unused)))
{
    for(;;)
    {
        TickType_t ticks = 1 / portTICK_PERIOD_MS;
        vTaskDelay(ticks ? ticks : 1); /* Minimum delay = 1 tick */
    }
}

/**
 * @brief  FreeRTOS initialization
 * @retval None.
 */
void MX_FREERTOS_Init(void)
{
    /* Create the default task. */
    xTaskCreate(StartDefaultTask,
                (const char* const)"StartDefaultTask",
                configMINIMAL_STACK_SIZE,
                0,
                tskIDLE_PRIORITY + 1,
                0);
}

/*============================================================================*/