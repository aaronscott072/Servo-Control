/*******************************************************************************
 * @file   timer.c
 * @brief  Hardware/software timers source file.
 ******************************************************************************/

#include "timer.h"

TIM_HandleTypeDef htim16;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/*===== STM32 HAL Time Base ==================================================*/

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;
    HAL_StatusTypeDef status = HAL_OK;

    /* Enable TIM16 clock. */
    __HAL_RCC_TIM16_CLK_ENABLE();

    /* Get clock configuration. */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Compute TIM16 clock. */
    uwTimclock = HAL_RCC_GetPCLK2Freq();

    /* Compute the prescaler value to have TIM16 counter clock equal to 1 MHz. */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM16. */
    htim16.Instance = TIM16;

    /**
     * Initialise TIM16 peripheral:
     *     - Period = [(TIM16CLK/1000) - 1] to have a (1/1000) s time base.
     *     - Prescaler = (uwTimclock/1000000 - 1) to have a 1 MHz counter clock.
     *     - ClockDivision = 0.
     *     - Counter direction = Up.
     */
    htim16.Init.Period = (1000000U / 1000U) - 1U;
    htim16.Init.Prescaler = uwPrescalerValue;
    htim16.Init.ClockDivision = 0;
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&htim16);
    if (status == HAL_OK)
    {
        /* Start the TIM time base generation in interrupt mode. */
        status = HAL_TIM_Base_Start_IT(&htim16);
        if (status == HAL_OK)
        {
            /* Enable the TIM16 global interrupt. */
            HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
            /* Configure the SysTick IRQ priority. */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM16 IRQ priority. */
                HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

void HAL_SuspendTick(void)
{
    /* Disable the TIM16 update interrupt. */
    __HAL_TIM_DISABLE_IT(&htim16, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
    /* Enable the TIM16 update interrupt. */
    __HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /**
     * TIM16 used as the STM32 HAL SYS time base source. Call HAL_IncTick()  
     * to increment @param uwTick which is used as the HAL time base.
     */ 
    if (htim->Instance == TIM16)
    {
        HAL_IncTick();
    }
}

/*============================================================================*/