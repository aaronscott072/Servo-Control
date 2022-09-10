/*******************************************************************************
 * @file   leds.c
 * @brief  Status LEDs initialisation and control source file.
 ******************************************************************************/

#include "leds.h"
#include "gpio.h"

typedef struct LED_GPIO_CONFIG_t {
    GPIO_TypeDef *port;          /* Port; GPIOx where x can be (A..E,H). */
    uint16_t     pin;            /* Pin; choose GPIO_PIN_x where x can be (0..15). */
    uint8_t      starting_state; /* Starting state (0 = LOW; 1 = HIGH). */
    uint32_t     mode;           /* Mode; choose GPIO_MODE_OUTPUT_x where x can be _PP/_OD. */
    uint32_t     push_pull;      /* PU/PD; choose GPIO_x where x can be NOPULL/PULLUP/PULLDOWN. */
    uint32_t     speed;          /* Speed; choose GPIO_SPEED_FREQ_ where x can be LOW/MEDIUM/HIGH/VERY_HIGH. */
} LED_GPIO_CONFIG_t;

/**
 * @note: - Edit this array to manage the number/type/config of the LEDs in the
 *          system.
 *        - Concurrently add/remove relevant IDs to/from LED_ID_t in @ref leds.h
 *          to have them be publically available, ensuring the ordering matches 
 *          this array.
 */
static LED_GPIO_CONFIG_t _leds[] = {
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
    for (uint32_t i = 0; i < NUM_ARRAY_ELS(_leds); i++)
    {
        gpio_init_output_pin(_leds[i].port, 
                             _leds[i].pin, 
                             _leds[i].starting_state, 
                             _leds[i].mode,
                             _leds[i].push_pull,
                             _leds[i].speed);
    }
}

void leds_set_one(LED_ID_t id, LED_STATE_t state)
{
    HAL_GPIO_WritePin(_leds[id].port, _leds[id].pin, (GPIO_PinState)state);
}

void leds_set_all(LED_STATE_t state)
{
    for (uint32_t i = 0; i < NUM_ARRAY_ELS(_leds); i++)
    {
        leds_set_one(i, state);
    }
}

/*============================================================================*/