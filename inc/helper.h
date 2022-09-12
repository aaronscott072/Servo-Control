/*******************************************************************************
 * @file   helper.h
 * @brief  Helper header file. Contains general-use defines, macros, and 
 *         functions (for example, the renaming of HAL functions to maintain  
 *         the project codying style).
 ******************************************************************************/

#ifndef HELPER_H
#define HELPER_H

/*===== C Standard Library =====*/
#include <stdio.h>
/*===== STM32 HAL =====*/
#include "stm32l4xx_hal.h"

/*===== Defines & Macros =====================================================*/

#ifndef UNUSED
#define UNUSED(X) ((void)X) /* Alternatively use __unused or __attribute__((unused)). */
#endif

#define NUM_ARRAY_ELS(array) (sizeof(array) / sizeof((array)[0]))
#define MCU_SOFT_RESET() __NVIC_SystemReset()
#define LIMIT_VAR_MAX(max, var) ((var > max) ? max : var)
#define LIMIT_VAR_MIN(min, var) ((var < min) ? min : var)
#define LIMIT_VAR_RANGE(min, max, var) (LIMIT_VAR_MAX(max, LIMIT_VAR_MIN(min, var)))

/*===== Function Pointers ====================================================*/

/**
 * @brief  Function pointer to HAL_Init to provide an alternative function 
 *         name (to maintain project coding style).
 * @retval None.
 */
extern HAL_StatusTypeDef (*hal_init)(void);

/*============================================================================*/

#endif /* HELPER_H ===========================================================*/