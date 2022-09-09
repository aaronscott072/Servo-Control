/*******************************************************************************
 * @file   stm32l4xx_it.h
 * @brief  Interrupt service routines/handlers header file.
 ******************************************************************************/

#ifndef STM32L4xx_IT_H
#define STM32L4xx_IT_H

#include "main.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Non maskable interrupt handler.
 * @retval None.
 */
void NMI_Handler(void);

/**
 * @brief  Hard fault interrupt handler.
 * @retval None.
 */
void HardFault_Handler(void);

/**
 * @brief  Memory management fault handler.
 * @retval None.
 */
void MemManage_Handler(void);

/**
 * @brief  Prefetch fault or memory access fault handler.
 * @retval None.
 */
void BusFault_Handler(void);

/**
 * @brief  Undefined instruction or illegal state handler.
 * @retval None.
 */
void UsageFault_Handler(void);

/**
 * @brief  Debug monitor handler.
 * @retval None.
 */
void DebugMon_Handler(void);

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void);

/**
 * @brief  TIM1 update interrupt and TIM16 global interrupt handler.
 * @retval None.
 */
void TIM1_UP_TIM16_IRQHandler(void);

/*============================================================================*/

#endif /* STM32L4xx_IT_H =====================================================*/