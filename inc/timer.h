/*******************************************************************************
 * @file   timer.h
 * @brief  Hardware/software timers header file.
 ******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include "main.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/*===== STM32 HAL Time Base ==================================================*/

/**
 * @brief  This function configures the TIM16 as a time base source.
 *         The time source is configured to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called automatically at the beginning of the program 
 *         after reset by HAL_Init() or at any time when the clock is configured 
 *         by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status.
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);

/**
 * @brief  Suspend tick increment.
 * @note   Disable the tick increment by disabling the TIM16 update interrupt.
 * @retval None.
 */
void HAL_SuspendTick(void);

/**
 * @brief  Resume tick increment.
 * @note   Enable the tick increment by enabling the TIM16 update interrupt.
 * @retval None.
 */
void HAL_ResumeTick(void);

/**
 * @brief  Timer period elapsed call-back function (in non-blocking mode).
 * @param  htim: TIM handle.
 * @retval None.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/*============================================================================*/

#endif /* TIMER_H ============================================================*/