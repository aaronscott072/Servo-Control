/*******************************************************************************
 * @file   gpio.h
 * @brief  GPIO header file.
 ******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

#include "main.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  Initialise a GPIO as an output pin.
 * @param  port:           Port / GPIO peripheral; choose GPIOx where x can be (A..E,H).
 * @param  pin:            Pin (port bit); choose GPIO_PIN_x where x can be (0..15).
 * @param  starting_state: Starting state of the pin -
 *                             0: pull the pin LOW (clear the port pin) or 
 *                             1: pull the pin HIGH (set the port pin).
 * @param  mode:           Configuration mode -
 *                             Push-pull:  GPIO_MODE_OUTPUT_PP
 *                             Open-drain: GPIO_MODE_OUTPUT_OD
 * @param  push_pull:      Pull-up or pull-down activation -
 *                             Neither:   GPIO_NOPULL
 *                             Pull-up:   GPIO_PULLUP
 *                             Pull-down: GPIO_PULLDOWN
 * @param  speed:          Speed/frequency - select one of:
 *                             GPIO_SPEED_FREQ_LOW
 *                             GPIO_SPEED_FREQ_MEDIUM
 *                             GPIO_SPEED_FREQ_HIGH
 *                             GPIO_SPEED_FREQ_VERY_HIGH
 * @retval None.
 */
void gpio_init_output_pin(GPIO_TypeDef *port, 
                          uint16_t     pin, 
                          uint8_t      starting_state, 
                          uint32_t     mode,
                          uint32_t     push_pull,
                          uint32_t     speed);

/*============================================================================*/

#endif /* GPIO_H =============================================================*/