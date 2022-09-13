/*******************************************************************************
 * @file   op_mode.c
 * @brief  Operational mode source file.
 ******************************************************************************/

#include "op_mode.h"
#include "leds.h"
#include "servo.h"

static OP_MODE_t _op_mode = OP_MODE__UNKNOWN;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

OP_MODE_t op_mode_get(void)
{
    return _op_mode;
}

bool op_mode_update(void)
{
    //
    // @todo:
    //     -> get motor status (idle, running, errors)
    //     -> get LCD status and errors
    //     -> other errors (USART, etc.)
    //
    // -> consider implementing as a state-machine
    //

    static uint8_t angle_prev = 0;
    uint8_t angle_new = servo_get_angle_expected();
    if (angle_new != angle_prev)
    {
        /* Motor running. */
        _op_mode = OP_MODE__MOTOR_RUNNING;
    }
    else
    {
        /* System idle. */
        _op_mode = OP_MODE__IDLE;
    }
    angle_prev = angle_new;

    /* Determine if the mode has changed. */
    static OP_MODE_t previous = OP_MODE__UNKNOWN;
    OP_MODE_t current = _op_mode;
    bool mode_changed = (current != previous);
    previous = _op_mode;
    return mode_changed;
}

void op_mode_set_leds(void)
{
    switch (_op_mode)
    {
        case OP_MODE__UNKNOWN:
            leds_set_all(LED_STATE__OFF);
            break;
        case OP_MODE__IDLE:
            leds_set_all(LED_STATE__OFF);
            leds_set_one(LED_ID__GREEN_1, LED_STATE__ON);
            break;
        case OP_MODE__MOTOR_RUNNING:
            leds_set_all(LED_STATE__OFF);
            leds_set_one(LED_ID__YELLOW_1, LED_STATE__ON);
            break;
        case OP_MODE__ERROR_MOTOR:
        case OP_MODE__ERROR_LCD:
        case OP_MODE__ERROR_PERIPHERALS:
            leds_set_all(LED_STATE__OFF);
            leds_set_one(LED_ID__RED_1, LED_STATE__ON);
            break;
        case OP_MODE__ERROR_FW_FAULT:
        default:
            leds_set_all(LED_STATE__OFF);
            leds_set_one(LED_ID__YELLOW_1, LED_STATE__ON);
            leds_set_one(LED_ID__RED_1, LED_STATE__ON);
            break;
    }
}

void op_mode_set_error_fw_fault(void)
{
    _op_mode = OP_MODE__ERROR_FW_FAULT;
    op_mode_set_leds();
}

/*============================================================================*/