/*******************************************************************************
 * @file   stm32l4xx_it.c
 * @brief  Interrupt service routines/handlers source file.
 ******************************************************************************/

#include "stm32l4xx_it.h"

/*============================================================================*/
/*===== Cortex-M4 Processor Interrupt and Exception Handlers =================*/
/*============================================================================*/

void NMI_Handler(void)
{
    while (1)
    {
    }
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{
    extern void xPortSysTickHandler(void);

    #if (INCLUDE_xTaskGetSchedulerState == 1)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
    #else
    xPortSysTickHandler();
    #endif
}

/*============================================================================*/
/*===== STM32L4xx Peripheral Interrupt Handlers ==============================*/
/*============================================================================*/
/* See @ref startup_stm32l4xx.s for the available peripheral                  */
/* interrupt handler names.                                                   */
/*============================================================================*/

void TIM1_UP_TIM16_IRQHandler(void)
{
    extern TIM_HandleTypeDef htim16;
    HAL_TIM_IRQHandler(&htim16);
}

/*============================================================================*/