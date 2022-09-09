/*******************************************************************************
 * @file   rtos.H
 * @brief  RTOS components (tasks/queues/locks/etc.) initialisation header file.
 ******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

#include "main.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  RTOS components (tasks/queues/locks/etc.) initialisation.
 * @note   Call in @ref main.c prior to starting the scheduler.
 * @retval None.
 */
void rtos_init(void);

/*============================================================================*/

#endif /* RTOS_H =============================================================*/