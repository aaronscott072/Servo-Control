/*******************************************************************************
 * @file   timer.h
 * @brief  Hardware/software timers header file.
 ******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include "main.h"

/*===== Defines ==============================================================*/

#define TIMER_TIM2_PWM_PRESCALER            (800)  /* TIM2_PSC prescaler register value. */
#define TIMER_TIM2_PWM_PRESCALER_0INDEXED   (TIMER_TIM2_PWM_PRESCALER - 1)
#define TIMER_TIM2_PWM_COUNTER              (2000) /* TIM2_ARR auto-reload register value. */
#define TIMER_TIM2_PWM_COUNTER_0INDEXED     (TIMER_TIM2_PWM_COUNTER - 1)
#define TIMER_TIM2_PWM_PULSE                (0)    /* TIM2_CCR1 capture/compare register 1 initial value. */

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

/*===== TIM2 (Servo Motor PWM) ===============================================*/

/**
 * @brief  TIM2CH1 PWM initialisation
 * 
 *         PWM period and pulse-width (i.e. duty-cycle) setup:
 *             - PWM period:
 *                 - Timer period (seconds) = [1 / [[CLK(Hz)/prescaler]/counter]]
 *                 - TIM2 uses APB1 clock of 80 MHz.
 *                 - Prescaler of 800 reduces the 80 MHz clock for the timer
 *                   to 100 kHz (80 MHz / 800 = 100 kHz); prescaler is loaded
 *                   into the TIM2_PSC register.
 *                 - A counter value of 2000 in the auto-reload register
 *                   sets the timer frequency to 50 Hz (100 kHz / 2000 = 50 Hz);
 *                   the counter is loaded into the TIM2_ARR register.
 *                 - A timer frequency of 50 Hz gives a period of 20 ms.
 *             - Pulse-width (duty cycle):
 *                 - The value in the TIM2_CCR1 register determines the pulse
 *                   width, where the value is (0..counter) where counter
 *                   is the value in the TIM2_ARR register. For example, 
 *                   value = counter/4 would give a pulse-width of 5 ms (duty 
 *                   cycle of 25%).
 * 
 *          Key registers:
 *              - Prescaler:   TIM2_PSC   prescaler register
 *              - Counter:     TIM2_ARR   auto-reload register 
 *              - Pulse-width: TIM2_CCR1  capture/compare register 1
 * 
 * @retval None.
 */
void timer_tim2_pwm_init(void);

/**
 * @brief  Start/stop TIM2CH1 PWM.
 * @param  state: true|false = start|stop PWM.
 * @retval None.
 */
void timer_tim2_pwm_enable(bool state);

/**
 * @brief  Set TIM2CH1 PWM pulse value (used to set PWM pulse-width / duty-cycle).
 * @param  pulse: Set pulse-width where pulse is (0..[TIMx_ARR value]). Note that
 *                the TIMx_ARR value is the auto-reload register counter value
 *                set during timer initialisation (".Period" field). For example, 
 *                pulse = counter/4 would give a pulse-width of 5 ms (duty cycle 
 *                of 25%).
 * @retval None.
 */
void timer_tim2_pwm_set_pulse(uint32_t pulse);

/*============================================================================*/

#endif /* TIMER_H ============================================================*/