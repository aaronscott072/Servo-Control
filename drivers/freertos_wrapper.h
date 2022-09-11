/*******************************************************************************
 * @file   freertos_wrapper.h
 * @brief  FreeRTOS wrapper header file.
 * 
 *         This wrapper provides a simpler FreeRTOS interface by abstracting
 *         API features such as checking return values for errors and related
 *         error handling, forcing context switches when required, etc.
 * 
 * @note   The user can utilise their own error handling function by defining
 *         freertos_wrapper_error_handler() in their host project.
 * 
 ******************************************************************************/

#ifndef FREERTOS_WRAPPER_H
#define FREERTOS_WRAPPER_H

/*===== C Standard Library =====*/
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/*===== FreeRTOS =====*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "limits.h"
#include "semphr.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/*===== OS Control ===========================================================*/

/**
 * @brief  Start the scheduler.
 * @retval None.
 */
void freertos_wrapper_start_scheduler(void);

/*===== Tasks ================================================================*/

/**
 * @brief  Task create.
 * @param  fxn_name:     Pointer to task function name.
 * @param  debug_str_id: String identifier used during debugging.
 * @param  stack_depth:  Size of the allocated stack in words (@note 1 word=4 bytes).
 * @param  parameters:   Parameters to pass to the task (optional).
 * @param  priority:     Task priority.
 * @param  handle:       Pointer to task handle.
 * @retval None.
 */
void freertos_wrapper_task_create(TaskFunction_t               fxn_name, 
                                  const char * const           debug_str_id, 
                                  const configSTACK_DEPTH_TYPE stack_depth, 
                                  void * const                 parameters, 
                                  UBaseType_t                  priority, 
                                  TaskHandle_t * const         handle);

/**
 * @brief  Task delay (in milliseconds).
 * @param  ms: Milliseconds to delay.
 * @retval None.
 */
void freertos_wrapper_task_delay_ms(const uint32_t ms);

/**
 * @brief  Task delay (in ticks).
 * @param  ticks: Ticks to delay.
 * @retval None.
 */
void freertos_wrapper_task_delay_ticks(const TickType_t ticks);

/**
 * @brief  Task notify (*with* a notification value/action).
 * 
 *         Use freertos_wrapper_task_notify_give() to task notify *without*
 *         a notification value/action.
 * 
 * @param  handle: Task handle.
 * @param  value:  Notification value.
 * @param  action: Notification action.
 * @retval None.
 */
BaseType_t freertos_wrapper_task_notify(TaskHandle_t  handle,
                                        uint32_t      value,
                                        eNotifyAction action);

/**
 * @brief  Task notify (*without* a notification value/action).
 * 
 *         Use freertos_wrapper_task_notify() to task notify *with*
 *         a notification value/action.
 * 
 * @param  handle: Task handle.
 * @retval None.
 */
void freertos_wrapper_task_notify_give(TaskHandle_t handle);

/**
 * @brief  Task notify wait (in milliseconds).
 * @param  entry: Bits to clear on entry.
 * @param  exit:  Bits to clear on exit.
 * @param  nv:    Notification value pointer.
 * @param  ms:    Milliseconds to delay.
 * @retval None.
 */
BaseType_t freertos_wrapper_task_notify_wait_ms(uint32_t   entry,
                                                uint32_t   exit,
                                                uint32_t * nv,
                                                uint32_t   ms);

/**
 * @brief  Task notify wait (in ticks).
 * @param  entry: Bits to clear on entry.
 * @param  exit:  Bits to clear on exit.
 * @param  nv:    Notification value pointer.
 * @param  ticks: Ticks to delay.
 * @retval None.
 */
BaseType_t freertos_wrapper_task_notify_wait_ticks(uint32_t   entry,
                                                   uint32_t   exit,
                                                   uint32_t * nv,
                                                   TickType_t ticks);

/*============================================================================*/
/*===== Weak Public Functions ================================================*/
/*============================================================================*/

/*===== Error Handling =======================================================*/

/**
 * @brief  Error handler - invoked whenever an error is detected.
 * @note   IMPORTANT: The user can over-write this function with their own 
 *         implementation.
 * @retval None.
 */
__weak void freertos_wrapper_error_handler(void);

/*===== Error Detection ======================================================*/

/**
 * @brief  Stack overflow hook.
 * @note   IMPORTANT: The user can over-write this function with their own 
 *         implementation.
 * @retval None.
 */
__weak void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

/**
 * @brief  Malloc failed hook.
 * @note   IMPORTANT: The user can over-write this function with their own 
 *         implementation.
 * @retval None.
 */
__weak void vApplicationMallocFailedHook(void);

/*===== Static Allocation Support ============================================*/

#if (configSUPPORT_STATIC_ALLOCATION == 1)
__weak void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
#endif

#endif /* FREERTOS_WRAPPER_H =================================================*/