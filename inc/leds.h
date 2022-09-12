/*******************************************************************************
 * @file   leds.h
 * @brief  Status LEDs initialisation and control header file.
 ******************************************************************************/

#ifndef LEDS_H
#define LEDS_H

#include "main.h"

/**
 * @note: - Edit this enum to manage the publically available IDs for the LEDs 
 *          in the system.
 *        - Concurrently add/remove relevant entries to/from _leds_gpio in @ref 
 *          leds.c as required, ensuring the ordering matches this enum.
 */
typedef enum LED_ID_t {
    LED_ID__GREEN_1,
    LED_ID__YELLOW_1,
    LED_ID__RED_1
} LED_ID_t;

typedef enum LED_STATE_t {
    LED_STATE__OFF, /* 0 == LOW. */
    LED_STATE__ON   /* 1 == HIGH. */
} LED_STATE_t;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Initialise LEDs (GPIO configuration).
 * @retval None.
 */
void leds_init(void);

/**
 * @brief  Set one (1) LED to on/off.
 * @param  colour: LED to set (use LED_ID_t).
 * @param  state:  State (on/off) where 0|1 = off|on (use LED_STATE_t).
 * @retval None.
 */
void leds_set_one(LED_ID_t colour, LED_STATE_t state);

/**
 * @brief  Set all LEDs to on/off.
 * @param  state: State (on/off) where 0|1 = off|on (use LED_STATE_t).
 * @retval None.
 */
void leds_set_all(LED_STATE_t state);

/*============================================================================*/

#endif /* LEDS_H =============================================================*/