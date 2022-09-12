/*******************************************************************************
 * @file   gpio.h
 * @brief  GPIO header file.
 ******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

#include "main.h"

/*===== Typedefs =============================================================*/

typedef struct GPIO_CONFIG_t {
    GPIO_TypeDef *port;          /* Port; GPIOx where x can be (A..E,H). */
    uint16_t     pin;            /* Pin; choose GPIO_PIN_x where x can be (0..15). */
    uint8_t      starting_state; /* Starting state (0 = LOW; 1 = HIGH). */
    uint32_t     mode;           /* Mode; choose GPIO_MODE_OUTPUT_x where x can be _PP/_OD. */
    uint32_t     push_pull;      /* PU/PD; choose GPIO_x where x can be NOPULL/PULLUP/PULLDOWN. */
    uint32_t     speed;          /* Speed; choose GPIO_SPEED_FREQ_ where x can be LOW/MEDIUM/HIGH/VERY_HIGH. */
} GPIO_CONFIG_t;

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Initialise a GPIO as an output pin.
 * 
 * @param  gpio_config: struct of type GPIO_CONFIG_t where:
 *             (+) gpio_config.port:
 *                 - Port / GPIO peripheral; choose GPIOx where x can be (A..E,H).
 *             (+) gpio_config.pin:
 *                 - Pin (port bit); choose GPIO_PIN_x where x can be (0..15).
 *             (+) gpio_config.starting_state:
 *                 - Starting state of the pin:
 *                     - 0: pull the pin LOW (clear the port pin) or 
 *                     - 1: pull the pin HIGH (set the port pin).
 *             (+) gpio_config.mode:
 *                 - Configuration mode:
 *                     - Push-pull:  GPIO_MODE_OUTPUT_PP
 *                     - Open-drain: GPIO_MODE_OUTPUT_OD
 *             (+) gpio_config.push_pull:
 *                 - Pull-up or pull-down activation:
 *                     - Neither:   GPIO_NOPULL
 *                     - Pull-up:   GPIO_PULLUP
 *                     - Pull-down: GPIO_PULLDOWN
 *             (+) gpio_config.speed:
 *                 - Speed/frequency - select one of:
 *                     - GPIO_SPEED_FREQ_LOW
 *                     - GPIO_SPEED_FREQ_MEDIUM
 *                     - GPIO_SPEED_FREQ_HIGH
 *                     - GPIO_SPEED_FREQ_VERY_HIGH
 * 
 * @retval None.
 */
void gpio_init_output_pin(GPIO_CONFIG_t gpio_config);

/*============================================================================*/

#endif /* GPIO_H =============================================================*/