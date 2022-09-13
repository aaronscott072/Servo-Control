/*******************************************************************************
 * @file   servo.c
 * @brief  Servo motor control source file.
 *         Refer to .h file top-level comment for information.
 ******************************************************************************/

#include "servo.h"
#include "timer.h"

static uint8_t _angle_expected; 
// @todo: add when implementing controller: static uint8_t _angle_actual;

/*===== Defines & Macros =====================================================*/

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

/*===== Private Function Prototypes ==========================================*/
static void record_angle_expected(uint8_t angle);

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
    record_angle_expected(angle);
    uint32_t pulse = SERVO_ANGLE_TO_PULSE_VALUE(angle);
    timer_tim2_pwm_set_pulse(pulse);
}

uint8_t servo_get_angle_expected(void)
{
    return _angle_expected;
}

void servo_test_oscillate(uint8_t angle_start, uint8_t angle_end, bool reset)
{
    angle_end = LIMIT_VAR_MAX(SERVO_POSITION_MAX_DEG_UINT, angle_end);

    static uint8_t i = 0;
    static bool clockwise = true;

    if (reset)
    {
        i = 0;
        clockwise = true;
        return;
    }

    servo_set_position(i);

    if (clockwise)
    {
        if (++i >= angle_end)
        {
            clockwise = false;
        }
    }
    else
    {
        if (--i <= angle_start)
        {
            clockwise = true;
        }
    }
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/**
 * @brief  Record the *expected* servo motor shaft position (angle in degrees) 
 *         such that external sources can retrieve this value by calling
 *         servo_get_angle_expected.
 * @param  angle: Angle in degrees (0..180).
 * @retval None.
 */
static void record_angle_expected(uint8_t angle)
{
    _angle_expected = angle;
}

/*============================================================================*/