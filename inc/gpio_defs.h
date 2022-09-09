/*******************************************************************************
 * @file   gpio_defs.h
 * @brief  GPIO definitions header file.
 *
 * @note   This file provides helper definitions for features such as GPIO ports, 
 *         pins, clocks, etc. It also somewhat acts as a pinout reference.
 *
 ******************************************************************************/

#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#include "main.h"

/*===== USART2 [NUCLEO-L433RC-P virtual COM port] ============================*/

#define GPIO_DEFS__PORT_USART2_TX           GPIOA
#define GPIO_DEFS__PIN_USART2_TX            GPIO_PIN_2
#define GPIO_DEFS__CLK_EN_USART2_TX()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIO_DEFS__PORT_USART2_RX           GPIOA
#define GPIO_DEFS__PIN_USART2_RX            GPIO_PIN_3
#define GPIO_DEFS__CLK_EN_USART2_RX()       __HAL_RCC_GPIOA_CLK_ENABLE()

/*============================================================================*/

#endif /* GPIO_DEFS_H ========================================================*/