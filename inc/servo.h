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

/*============================================================================*/

#endif /* SERVO_H ============================================================*/