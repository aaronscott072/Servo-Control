/*******************************************************************************
 * @file   servo.h
 * @brief  Servo motor control header file.
 * 
 *         Provides:
 *             - STM32 HAL timer/PWM initialisation.
 *             - Set servo motor shaft position (angle in degrees).
 *             - @todo
 *  
 ******************************************************************************/

#ifndef SERVO_H
#define SERVO_H

#include "main.h"

/*===== Defines ==============================================================*/

/* Position values (angle in degrees). */
#define SERVO_POSITION_MIN_DEG_INT  (-90)
#define SERVO_POSITION_MAX_DEG_INT  90
#define SERVO_POSITION_MIN_DEG_UINT 0    /* == -90 degrees. */
#define SERVO_POSITION_MAX_DEG_UINT 180  /* == +90 degrees. */

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Servo motor initialisation:
 *             - PWM initialisation.
 *             - @todo
 * @retval None.
 */
void servo_init(void);

/**
 * @brief  Set PWM signal to the servo motor (on/off).
 * @param  state: PWM signal state to set where true|false = on|off.
 * @retval None.
 */
void servo_set_signal(bool state);

/**
 * @brief  Set servo motor shaft position (angle in degrees).
 * @param  angle: Angle in degrees (0..180).
 * @retval None.
 */
void servo_set_position(uint8_t angle);

/**
 * @brief  Retrieve the *expected* servo motor shaft position (angle in degrees).
 * @param  angle: Angle in degrees (0..180).
 * @retval None.
 */
uint8_t servo_get_angle_expected(void);

/**
 * @brief  Test function: oscillate servo motor shaft position (angle in 
 *         degrees) between two specified angles.
 * 
 *         Each single call of this function moves the position one (1) degree;
 *         as such, the user project should call this function periodically,
 *         for example, in an RTOS task with a task delay after each call.
 * 
 *         The position will start at 0 degrees, then oscillate between 
 *         the specified angles.
 * 
 *         If running the test with new parameters or after stopping: call this
 *         function once with @param reset set to true, then run the new test.
 * 
 * @param  angle_start: Starting angle in degrees (0..180).
 * @param  angle_end:   Starting angle in degrees (0..180).
 * @param  reset:       Boolean to reset the test, see @brief. Default: false.
 * @retval None.
 */
void servo_test_oscillate(uint8_t angle_start, uint8_t angle_end, bool reset);

/*============================================================================*/

#endif /* SERVO_H ============================================================*/