/*******************************************************************************
 * @file   main.h
 * @brief  Main header file.
 ******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

/*===== C Standard Library =====*/
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/*===== STM32 HAL =====*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"
/*===== FreeRTOS =====*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "limits.h"
#include "semphr.h"
/*===== Drivers =====*/
#include "freertos_wrapper.h"
/*===== Other =====*/
#include "gpio_defs.h"
#include "helper.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Application entry point.
 * @retval None.
 */
int main(void);

/**
 * @brief  Global error handler.
 * @retval None.
 */
void error_handler(void);

/**
 * @brief  Overwrite weakly defined function in @ref freertos_wrapper.h.
 * @retval None.
 */
void freertos_wrapper_error_handler(void);

/*============================================================================*/

#endif /* MAIN_H =============================================================*/