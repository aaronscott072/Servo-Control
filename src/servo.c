/*******************************************************************************
 * @file   servo.c
 * @brief  Servo motor control source file.
 * 
 *         Refer to .h file top-level comment for information.
 ******************************************************************************/

#include "servo.h"
#include "timer.h"

/*===== Defines & Macros =====================================================*/

/* Position values (angle in degrees). */
#define SERVO_POSITION_MIN_DEG_INT  (-90)
#define SERVO_POSITION_MAX_DEG_INT  90
#define SERVO_POSITION_MIN_DEG_UINT 0    /* == -90 degrees. */
#define SERVO_POSITION_MAX_DEG_UINT 180  /* == +90 degrees. */

/* PWM pulse-widths to achieve certain positions given a 20 ms period signal. */
#define SERVO_PWM_PERIOD_IN_MS                     20  /* 20 ms PWM period. */
#define SERVO_PWM_PULSE_WIDTH_FOR_NEG_90_DEG_IN_MS 0.5 /* 0.5 ms pulse-width = -90 Deg. */
#define SERVO_PWM_PULSE_WIDTH_FOR_0_DEG_IN_MS      1.5 /* 1.5 ms pulse-width =   0 Deg. */
#define SERVO_PWM_PULSE_WIDTH_FOR_POS_90_DEG_IN_MS 2.5 /* 2.5 ms pulse-width = +90 Deg. */

/**
 * PWM pulse values for TIMx_CCRx register to set pulse-width / duty-cycle.
 * Note that the values have -1 as the timer counts from 0 to (TIMER_TIM2_PWM_COUNTER - 1).
 */
#define SERVO_PWM_PULSE_VALUE_MIN \
    (((SERVO_PWM_PULSE_WIDTH_FOR_NEG_90_DEG_IN_MS / SERVO_PWM_PERIOD_IN_MS) * \
    TIMER_TIM2_PWM_COUNTER) - 1)
#define SERVO_PWM_PULSE_VALUE_MAX \
    (((SERVO_PWM_PULSE_WIDTH_FOR_POS_90_DEG_IN_MS / SERVO_PWM_PERIOD_IN_MS) * \
    TIMER_TIM2_PWM_COUNTER) - 1)

/**
 * Linear equation to convert an angle (0 to 180 deg) into a PWM pulse value:
 *     Val = (((pulse_max - pulse_min)/(angle_max - angle_min)) * angle) + pulse_min
 */
#define SERVO_ANGLE_TO_PULSE_VALUE(a) \
    ((((SERVO_PWM_PULSE_VALUE_MAX - SERVO_PWM_PULSE_VALUE_MIN)/(SERVO_POSITION_MAX_DEG_UINT - \
    SERVO_POSITION_MIN_DEG_UINT)) * (float)a) + SERVO_PWM_PULSE_VALUE_MIN)

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void servo_init(void)
{
    timer_tim2_pwm_init();
}

void servo_set_signal(bool state)
{
    timer_tim2_pwm_enable(state);
}

void servo_set_position(uint8_t angle)
{
    angle = LIMIT_VAR_MAX(SERVO_POSITION_MAX_DEG_UINT, angle);
    uint32_t pulse = SERVO_ANGLE_TO_PULSE_VALUE(angle);
    timer_tim2_pwm_set_pulse(pulse);
}

/*============================================================================*/