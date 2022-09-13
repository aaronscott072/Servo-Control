/*******************************************************************************
 * @file   op_mode.h
 * @brief  Operational mode header file.
 *******************************************************************************
 *
 *     OPERATIONAL MODE                           LED STATUS INDICATION
 *     ----------------------------------------------------------------
 *     Unknown (during MCU start-up)              None
 *     Idle                                       Green
 *     Motor running                              Yellow
 *     Error state: motor                         Red
 *     Error state: LCD                           Red
 *     Error state: peripherals (USART, etc.)     Red
 *     Error state: firmware fault                Red & Yellow
 * 
 ******************************************************************************/

#ifndef OP_MODE_H
#define OP_MODE_H

#include "main.h"

typedef enum OP_MODE_t {
    OP_MODE__UNKNOWN,
    OP_MODE__IDLE,
    OP_MODE__MOTOR_RUNNING,
    OP_MODE__ERROR_MOTOR,
    OP_MODE__ERROR_LCD,
    OP_MODE__ERROR_PERIPHERALS,
    OP_MODE__ERROR_FW_FAULT
} OP_MODE_t;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Retrieve operational mode.
 * @retval Operational mode.
 */
OP_MODE_t op_mode_get(void);

/**
 * @brief  Update operational mode.
 * 
 *         Functional intended to be called periodically to update the current
 *         operational mode given factors such as the status of the motor.
 * 
 * @retval Boolean indicating if the operational mode has changed.
 */
bool op_mode_update(void);

/**
 * @brief  Illuminate the LEDs based on the current operational mode.
 * @retval None.
 */
void op_mode_set_leds(void);

/**
 * @brief  Force the operational mode to be "Error Firmware Fault" and
 *         illuminate the LEDs based on this mode.
 * @note   IMPORTANT: This function is only intended to be used within the
 *         global error handling function (see @ref main.c:error_handler). 
 * @retval None.
 */
void op_mode_set_error_fw_fault(void);

/*============================================================================*/

#endif /* OP_MODE_H ==========================================================*/