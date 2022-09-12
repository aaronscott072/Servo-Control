/*******************************************************************************
 * @file   gpio_defs.h
 * @brief  GPIO definitions header file.
 *
 * @note   This file provides helper definitions for features such as GPIO ports, 
 *         pins, clocks, etc. It also somewhat acts as a pinout reference.
 ******************************************************************************/

#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#include "main.h"

/*===== GENERAL ==============================================================*/

#define GPIO_DEFS__LOW                      0
#define GPIO_DEFS__HIGH                     1

/*===== LEDS (GREEN/YELLOW/RED) ==============================================*/

#define GPIO_DEFS__PORT_LED_GREEN           GPIOC
#define GPIO_DEFS__PIN_LED_GREEN            GPIO_PIN_0
#define GPIO_DEFS__PORT_LED_YELLOW          GPIOC
#define GPIO_DEFS__PIN_LED_YELLOW           GPIO_PIN_1
#define GPIO_DEFS__PORT_LED_RED             GPIOC
#define GPIO_DEFS__PIN_LED_RED              GPIO_PIN_2

#define GPIO_DEFS__LED_ON                   GPIO_DEFS__HIGH
#define GPIO_DEFS__LED_OFF                  GPIO_DEFS__LOW

/*===== USART2 [NUCLEO-L433RC-P virtual COM port] ============================*/

#define GPIO_DEFS__PORT_USART2_TX           GPIOA
#define GPIO_DEFS__PIN_USART2_TX            GPIO_PIN_2
#define GPIO_DEFS__CLK_EN_USART2_TX()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIO_DEFS__PORT_USART2_RX           GPIOA
#define GPIO_DEFS__PIN_USART2_RX            GPIO_PIN_3
#define GPIO_DEFS__CLK_EN_USART2_RX()       __HAL_RCC_GPIOA_CLK_ENABLE()

/*===== SERVO MOTOR CONTROL (TIMx_CHx PWM) ===================================*/

#define GPIO_DEFS__PORT_SERVO_MOTOR_PWM         GPIOA
#define GPIO_DEFS__PIN_SERVO_MOTOR_PWM          GPIO_PIN_0

/*===== LCD ==================================================================*/

#define GPIO_DEFS__PORT_LCD_DB4             GPIOB
#define GPIO_DEFS__PIN_LCD_DB4              GPIO_PIN_12
#define GPIO_DEFS__PORT_LCD_DB5             GPIOC
#define GPIO_DEFS__PIN_LCD_DB5              GPIO_PIN_11
#define GPIO_DEFS__PORT_LCD_DB6             GPIOC
#define GPIO_DEFS__PIN_LCD_DB6              GPIO_PIN_10
#define GPIO_DEFS__PORT_LCD_DB7             GPIOC
#define GPIO_DEFS__PIN_LCD_DB7              GPIO_PIN_12
#define GPIO_DEFS__PORT_LCD_RS              GPIOA
#define GPIO_DEFS__PIN_LCD_RS               GPIO_PIN_1
#define GPIO_DEFS__PORT_LCD_EN              GPIOC
#define GPIO_DEFS__PIN_LCD_EN               GPIO_PIN_3

/*============================================================================*/

#endif /* GPIO_DEFS_H ========================================================*/