/*******************************************************************************
 * @file   lcd.h
 * @brief  LCD header file.
 ******************************************************************************
 *
 *             - LCD driven by Hitachi HD44780U controller.
 *             - Operating in 4-bit mode (4 data pins).
 *             - Display is 2 lines, 16 digits, 5x8 dot.
 *
 *                            ===== API Use =====
 * 
 *     (+) (Optional) Overwrite lcd_delay_ms:
 *             - The user can utilise their own delay function by defining
 *               this function in their host project. For example, to use
 *               an RTOS task delay rather than a STM32 HAL delay when the
 *               scheduler is running.
 *     (+) Call lcd_init() once to configure/initialise the required GPIO pins
 *         and setup the LCD such that it is ready for user interaction.
 *     (+) Features:
 *         (+) Clear display    lcd_clear_display
 *         (+) Write line       lcd_write_line
 *         (+) @todo
 * 
 ******************************************************************************/

#ifndef LCD_H
#define LCD_H

#include "main.h"

/*===== Defines and Typedefs =================================================*/
/*===== Line/Digit Number Selection =====*/
typedef enum LCD_LINE_NUM_t {
    LCD_LINE_NUM_1 = 0, /* Line numbers are 0-indexed in the driver. */
    LCD_LINE_NUM_2
} LCD_LINE_NUM_t;

#define LCD_MAX_DIGITS 16

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

/**
 * @brief  LCD delay function (in milliseconds) used throughout the driver.
 * @note   IMPORTANT: The user can over-write this function with their own 
 *         implementation.
 * @param  ms: delay (in milliseconds).
 * @retval None.
 */
__weak void lcd_delay_ms(uint32_t ms);

/**
 * @brief  LCD initialisation.
 *             - GPIO configuration.
 *             - Display setup.
 * @retval None.
 */
void lcd_init(void);

/**
 * @brief  LCD clear display. Fills the screen with blank characters.
 * @retval None.
 */
void lcd_clear_display(void);

/**
 * @brief  LCD write line to the display.
 * 
 *         Note that if the written line is shorter than the width of the
 *         display, the remaining digits will be filled with blank spaces.
 * 
 * @param  line_num: Line number to write to (0|1 for line 1|2).
 * @param  data:     Data to write.
 * @param  data_len: Length of data to write.
 * @retval None.
 */
void lcd_write_line(LCD_LINE_NUM_t line_num, uint8_t* data, uint8_t data_len);

/*============================================================================*/

#endif /* LCD_H ==============================================================*/