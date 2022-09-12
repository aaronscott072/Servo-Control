/*******************************************************************************
 * @file   leds.c
 * @brief  Status LEDs initialisation and control source file.
 ******************************************************************************/

#include "leds.h"
#include "gpio.h"

/**
 * @note: - Edit this array to manage the number/type/config of the LEDs in the
 *          system.
 *        - Concurrently add/remove relevant IDs to/from LED_ID_t in @ref leds.h
 *          to have them be publically available, ensuring the ordering matches 
 *          this array.
 */
static GPIO_CONFIG_t _leds_gpio[] = {
    /* GREEN #1. */
    {
        GPIO_DEFS__PORT_LED_GREEN,
        GPIO_DEFS__PIN_LED_GREEN,
        GPIO_DEFS__LED_OFF,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    },
    /* YELLOW #1. */
    {
        GPIO_DEFS__PORT_LED_YELLOW,
        GPIO_DEFS__PIN_LED_YELLOW,
        GPIO_DEFS__LED_OFF,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    },
    /* RED #1. */
    {
        GPIO_DEFS__PORT_LED_RED,
        GPIO_DEFS__PIN_LED_RED,
        GPIO_DEFS__LED_OFF,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    },
};

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void leds_init(void)
{
    for (uint32_t i = 0; i < NUM_ARRAY_ELS(_leds_gpio); i++)
    {
        gpio_init_output_pin(_leds_gpio[i]);
    }
}

void leds_set_one(LED_ID_t id, LED_STATE_t state)
{
    HAL_GPIO_WritePin(_leds_gpio[id].port, _leds_gpio[id].pin, (GPIO_PinState)state);
}

void leds_set_all(LED_STATE_t state)
{
    for (uint32_t i = 0; i < NUM_ARRAY_ELS(_leds_gpio); i++)
    {
        leds_set_one(i, state);
    }
}

/*============================================================================*/