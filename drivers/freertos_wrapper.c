/*******************************************************************************
 * @file   freertos_wrapper.c
 * @brief  FreeRTOS wrapper source file.
 ******************************************************************************/

#include "freertos_wrapper.h"

/*===== Private Function Prototypes ==========================================*/
static bool check_pass(BaseType_t retval);

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/*===== OS Control and General ===============================================*/

void freertos_wrapper_start_scheduler(void)
{
    vTaskStartScheduler();
}

BaseType_t freertos_wrapper_get_scheduler_state(void)
{
    return xTaskGetSchedulerState();
}

bool freertos_wrapper_is_scheduler_running(void)
{
    return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING);
}

/*===== Tasks ================================================================*/

void freertos_wrapper_task_create(TaskFunction_t               fxn_name, 
                                  const char * const           debug_str_id, 
                                  const configSTACK_DEPTH_TYPE stack_depth, 
                                  void * const                 parameters, 
                                  UBaseType_t                  priority, 
                                  TaskHandle_t * const         handle)
{
    BaseType_t retval = xTaskCreate(fxn_name, 
                                    debug_str_id,
                                    stack_depth,
                                    parameters,
                                    priority,
                                    handle);

    check_pass(retval);
}

void freertos_wrapper_task_delay_ms(const uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void freertos_wrapper_task_delay_ticks(const TickType_t ticks)
{
    vTaskDelay(ticks);
}

BaseType_t freertos_wrapper_task_notify(TaskHandle_t  handle,
                                        uint32_t      value,
                                        eNotifyAction action)
{
    BaseType_t retval = xTaskNotify(handle, value, action);
    check_pass(retval);
    return retval;
}

void freertos_wrapper_task_notify_give(TaskHandle_t handle)
{
    BaseType_t retval = xTaskNotifyGive(handle);
    check_pass(retval);
}

BaseType_t freertos_wrapper_task_notify_wait_ms(uint32_t   entry,
                                                uint32_t   exit,
                                                uint32_t * nv,
                                                uint32_t   ms)
{
    BaseType_t retval = xTaskNotifyWait(entry, exit, nv, pdMS_TO_TICKS(ms));

    /**
     * @note: do not enter the error handler when xTaskNotifyWait() returns
     * pdFAIL as this indicates that the timeout has occurred before receiving
     * a notification (i.e. an error may or may not have occurred, that is up
     * to the user to determine with @retval retval).
     */

    return retval;
}

BaseType_t freertos_wrapper_task_notify_wait_ticks(uint32_t   entry,
                                                   uint32_t   exit,
                                                   uint32_t * nv,
                                                   TickType_t ticks)
{
    BaseType_t retval = xTaskNotifyWait(entry, exit, nv, ticks);

    /**
     * @note: do not enter the error handler when xTaskNotifyWait() returns
     * pdFAIL as this indicates that the timeout has occurred before receiving
     * a notification (i.e. an error may or may not have occurred, that is up
     * to the user to determine with @retval retval).
     */

    return retval;
}

/*============================================================================*/
/*===== Weak Public Functions ================================================*/
/*============================================================================*/

/*===== Error Handling =======================================================*/

__weak void freertos_wrapper_error_handler(void)
{
    assert(0);
    while (1) {;}
}

/*===== Error Detection ======================================================*/

__weak void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;

    freertos_wrapper_error_handler();
}

__weak void vApplicationMallocFailedHook(void)
{
    freertos_wrapper_error_handler();
}

/*===== Static Allocation Support ============================================*/

#if (configSUPPORT_STATIC_ALLOCATION == 1)
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

__weak void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
#endif

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/**
 * @brief  Check if a FreeRTOS API return value indicates a pass (no errors).
 *         An error handler is called if an error is detected.
 * @param  retval: FreeRTOS API return value.
 * @retval Boolean indicating if the return value indicates a pass (no errors).
 */
static bool check_pass(BaseType_t retval)
{
    bool pass = (retval == pdPASS);

    if (pass == false)
    {
        freertos_wrapper_error_handler();
    }

    return pass;
}

/*============================================================================*/