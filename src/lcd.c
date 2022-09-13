/*******************************************************************************
 * @file   lcd.c
 * @brief  LCD source file.
 *         Refer to .h file top-level comment for information.
 ******************************************************************************/

#include "lcd.h"
#include "gpio.h"

/*===== Defines and Typedefs =================================================*/

/*===== Register Select Pin (Instruction or Data Register) =====*/
#define LCD_RS_WRITE_TO_IR 0 /* Set RS LOW to write to the instruction register. */
#define LCD_RS_WRITE_TO_DR 1 /* Set RS HIGH to write to the data register. */

typedef enum LCD_RS_t {
    LCD_RS__IR,
    LCD_RS__DR
} LCD_RS_t;

/*===== Instruction Register Helper Flags =====*/
/* Reference: pg. 23-27 of the datasheet, "Table 6" is particularly useful. */

/* Instructions. */
#define LCD_INSTRUCTION_CLEAR_DISPLAY                   0x01
#define LCD_INSTRUCTION_RETURN_HOME                     0x02
#define LCD_INSTRUCTION_ENTRY_MODE_SET                  0x04
#define LCD_INSTRUCTION_DISPLAY_ON_OFF_CONTROL          0x08
#define LCD_INSTRUCTION_CURSOR_OR_DISPLAY_SHIFT         0x10
#define LCD_INSTRUCTION_FUNCTION_SET                    0x20
#define LCD_INSTRUCTION_SET_CGRAM_ADDRESS               0x40
#define LCD_INSTRUCTION_SET_DDRAM_ADDRESS               0x80

/* Entry Mode Set. */
#define LCD_ENTRY_MODE_SET_CURSOR_MOVE_DIRECTION_RIGHT  0x00
#define LCD_ENTRY_MODE_SET_CURSOR_MOVE_DIRECTION_LEFT   0x02
#define LCD_ENTRY_MODE_SET_DISPLAY_SHIFT_RIGHT          0x00
#define LCD_ENTRY_MODE_SET_DISPLAY_SHIFT_LEFT           0x01

/* Display On/Off Control. */
#define LCD_DISPLAY_ON_OFF_CONTROL_ON                   0x04
#define LCD_DISPLAY_ON_OFF_CONTROL_OFF                  0x00
#define LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_ON            0x02
#define LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_OFF           0x00
#define LCD_DISPLAY_ON_OFF_CONTROL_BLINKING_CURSOR_ON   0x01
#define LCD_DISPLAY_ON_OFF_CONTROL_BLINKING_CURSOR_OFF  0x00

/* Cursor or Display Shift. */
#define LCD_CURSOR_OF_DISPLAY_SHIFT_DISPLAY_MOVE        0x08
#define LCD_CURSOR_OF_DISPLAY_SHIFT_CURSOR_MOVE         0x00
#define LCD_CURSOR_OF_DISPLAY_SHIFT_MOVE_RIGHT          0x04
#define LCD_CURSOR_OF_DISPLAY_SHIFT_MOVE_LEFT           0x00

/* Function Set. */
#define LCD_FUNCTION_SET_NUMBER_OF_DISPLAY_LINES_2      0x08

/*===== Objects/Structs ======================================================*/

// @todo: Convert driver to object oriented and generic, move to /drivers/.
//        Afterwards, move the below struct array and enum out of this file and 
//        into the project (e.g. lcd_ctrl.c/h).

static GPIO_CONFIG_t _lcd_gpio[] = {
    /* Data 4. */ {
        GPIO_DEFS__PORT_LCD_DB4, GPIO_DEFS__PIN_LCD_DB4, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
    /* Data 5. */ {
        GPIO_DEFS__PORT_LCD_DB5, GPIO_DEFS__PIN_LCD_DB5, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
    /* Data 6. */ {
        GPIO_DEFS__PORT_LCD_DB6, GPIO_DEFS__PIN_LCD_DB6, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
    /* Data 7. */ {
        GPIO_DEFS__PORT_LCD_DB7, GPIO_DEFS__PIN_LCD_DB7, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
    /* Register Select. */ {
        GPIO_DEFS__PORT_LCD_RS, GPIO_DEFS__PIN_LCD_RS, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
    /* Enable. */ {
        GPIO_DEFS__PORT_LCD_EN, GPIO_DEFS__PIN_LCD_EN, GPIO_DEFS__LOW, 
        GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW
    },
};

typedef enum LCD_GPIO_ID_t {
    LCD_GPIO_ID__DB4,
    LCD_GPIO_ID__DB5,
    LCD_GPIO_ID__DB6,
    LCD_GPIO_ID__DB7,
    LCD_GPIO_ID__RS,
    LCD_GPIO_ID__EN
} LCD_GPIO_ID_t;

/*===== Private Function Prototypes ==========================================*/
static void setup(void);
static void register_select(LCD_RS_t sel);
static void register_select_instruction(void);
static void register_select_data(void);
static void enable_pulse();
static void data_write_nibble(uint8_t data);
static void data_write_byte(uint8_t data);
static void instruction(uint8_t data);
static void print_char(uint8_t data);
static void set_cursor(LCD_LINE_NUM_t line_num, uint8_t digit_num);

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

__weak void lcd_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void lcd_init(void)
{
    /* GPIO configuration. */
    for (uint32_t i = 0; i < NUM_ARRAY_ELS(_lcd_gpio); i++)
    {
        gpio_init_output_pin(_lcd_gpio[i]);
    }

    /* LCD setup. */
    setup();
}

void lcd_clear_display(void)
{
    instruction(LCD_INSTRUCTION_CLEAR_DISPLAY);
    lcd_delay_ms(2);
}

void lcd_write_line(LCD_LINE_NUM_t line_num, uint8_t* data, uint8_t data_len)
{
    /* Check inputs. */
    if (line_num > LCD_LINE_NUM_2 || data_len > LCD_MAX_DIGITS)
    {
        return;
    }
    assert(data);   

    /* Write line. */
    set_cursor(line_num, 0);
    for (uint8_t i = 0; i < LCD_MAX_DIGITS; i++)
    {
        if (i < data_len)
        {
            print_char(data[i]);
        }
        else
        {
            print_char(' '); /* Fill the rest of the line with blank spaces. */
        }
    }
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/**
 * @brief  Initialise the LCD (4-bit interface).
 * 
 *         Reference: Datasheet pg 46. - "Initializing by Instruction" section, 
 *                    "Figure 24 4-Bit Interface".
 * 
 * @retval None.
 */
static void setup(void)
{
    uint8_t data;

    /* Following "Figure 24 4-Bit Interface". */
    lcd_delay_ms(50);               /* Delay (40+ ms) to allow the LCD to come up. */
    register_select_instruction();  /* Enable instruction register write. */
    data_write_nibble(0x03);        /* DB7..4: 0b0011. */
    lcd_delay_ms(5);                /* Delay (4.1+ ms). */
    data_write_nibble(0x03);        /* DB7..4: 0b0011. */
    lcd_delay_ms(1);                /* Delay (100+ us). */
    data_write_nibble(0x03);        /* DB7..4: 0b0011. */
    data_write_nibble(0x02);        /* DB7..4: 0b0010. */

    /* Set number of display lines and character font. */
    data = LCD_INSTRUCTION_FUNCTION_SET
         | LCD_FUNCTION_SET_NUMBER_OF_DISPLAY_LINES_2;
    data_write_byte(data);

    /* Turn the display on; set no cursor and no blinking cursor. */
    data = LCD_INSTRUCTION_DISPLAY_ON_OFF_CONTROL
         | LCD_DISPLAY_ON_OFF_CONTROL_ON 
         | LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_OFF
         | LCD_DISPLAY_ON_OFF_CONTROL_BLINKING_CURSOR_OFF;
    data_write_byte(data);

    /* Clear display. */
    lcd_clear_display();

    /* Set text direction (move from the left). */
    data = LCD_INSTRUCTION_ENTRY_MODE_SET
         | LCD_ENTRY_MODE_SET_CURSOR_MOVE_DIRECTION_LEFT
         | LCD_ENTRY_MODE_SET_DISPLAY_SHIFT_RIGHT;
    data_write_byte(data);
}

/**
 * @brief  Set register select (RS) pin. The controller has two registers: 
 *         instruction register (IR) and data register (DR). The RS pin 
 *         determines which register is set.
 * @param  sel: Register to select; 0 for IR, 1 for DR.
 * @retval None.
 */
static void register_select(LCD_RS_t sel)
{
    GPIO_PinState state = (sel == LCD_RS__IR) 
                        ? LCD_RS_WRITE_TO_IR 
                        : LCD_RS_WRITE_TO_DR;

    HAL_GPIO_WritePin(_lcd_gpio[LCD_GPIO_ID__RS].port,
                      _lcd_gpio[LCD_GPIO_ID__RS].pin,
                      state);
}

/**
 * @brief  Set register select (RS) pin to select the instruction register (IR).
 * @retval None.
 */
static void register_select_instruction(void)
{
    register_select(LCD_RS__IR);
}

/**
 * @brief  Set register select (RS) pin to select the data register (IR).
 * @retval None.
 */
static void register_select_data(void)
{
    register_select(LCD_RS__DR);
}

/**
 * @brief  Send a pulse on the enable pin to signal to the controller that
 *         data (from the data pins) is ready to be digested.
 * 
 *         Per pg. 49 of the datasheet under the "Write Operation" table,
 *         the minimum pulse-width is 450 ns. Additionally, address/data 
 *         set-up/hold times can last up to 195 ns. Lastly, per pg. 24 of
 *         the datasheet under the "Instructions" table, instructions take
 *         37 us to execute. As such, a 1 ms pulse, followed by a 1 ms delay
 *         should be sufficient to satisfy all timing minimums.
 * 
 * @retval None.
 */
static void enable_pulse()
{
    /* Set LOW. */
    HAL_GPIO_WritePin(_lcd_gpio[LCD_GPIO_ID__EN].port, _lcd_gpio[LCD_GPIO_ID__EN].pin, 0);
    lcd_delay_ms(1);

    /* Pulse for 1 ms. */
    HAL_GPIO_WritePin(_lcd_gpio[LCD_GPIO_ID__EN].port, _lcd_gpio[LCD_GPIO_ID__EN].pin, 1);
    lcd_delay_ms(1);
    HAL_GPIO_WritePin(_lcd_gpio[LCD_GPIO_ID__EN].port, _lcd_gpio[LCD_GPIO_ID__EN].pin, 0);

    /* Delay for 1 ms to give time for instructions/data to be digested/executed. */
    lcd_delay_ms(1);
}

/**
 * @brief  Write a nibble (4-bits) of data to the controller by setting the  
 *         data pins (DB4..7) and signalling to the controller that it should 
 *         digest the data on those pins.
 * @param  data: Nibble of data to write to controller.
 * @retval None.
 */
static void data_write_nibble(uint8_t data)
{
    uint8_t bit;

    for (uint8_t i = 0; i < 4; i++)
    {
        bit = (data >> i) & 0x01;
        HAL_GPIO_WritePin(_lcd_gpio[i].port, _lcd_gpio[i].pin, bit);
    }

    /* Signal to the controller that data (DB4..7) is ready to be digested. */
    enable_pulse();
}

/**
 * @brief  Write a byte (8-bits) of data to the controller by setting the  
 *         data pins (DB4..7) and signalling to the controller that it should 
 *         digest the data on those pins.
 * 
 *         Writing a byte of data to the controller happens in two operations 
 *         given this configuration has four (4) data pins. The most significant
 *         nibble is written first (bits 4..7 of the byte).
 * 
 * @param  data: Byte of data to write to controller.
 * @retval None.
 */
static void data_write_byte(uint8_t data)
{
    data_write_nibble(data >> 4); /* Most significant nibble;  bits 4..7. */
    data_write_nibble(data >> 0); /* Least significant nibble; bits 0..3. */
}

/**
 * @brief  Perform an instruction. Instructions are executed by setting the 
 *         register select pin to LOW, then writing the relevant data.
 * @param  data: Instruction byte to execute.
 * @retval None.
 */
static void instruction(uint8_t data)
{
    register_select_instruction();
    data_write_byte(data);
}

/**
 * @brief  Print a byte to the display. Printing to the display is executed by 
 *         setting the register select pin to HIGH, then writing the relevant 
 *         data.
 * @param  data: Data byte to the print to the display.
 * @retval None.
 */
static void print_char(uint8_t data)
{
    register_select_data();
    data_write_byte(data);
}

/**
 * @brief  Set the cursor to a specified line/digit position.
 * @param  line_num:  Line number to set the cursor to (0|1 for line 1|2).
 * @param  digit_num: Digit number to set the cursor to (0..15 for digits 1..16).
 * @retval None.
 */
static void set_cursor(LCD_LINE_NUM_t line_num, uint8_t digit_num)
{
    /* Check inputs. */
    if (line_num > LCD_LINE_NUM_2 || digit_num > (LCD_MAX_DIGITS-1))
    {
        return;
    }

    /* Find offset required when writing to DDRAM. */
    uint8_t line_num_offset = (line_num == LCD_LINE_NUM_2) ? 0x40 : 0x00;

    /* Set cursor. */
    uint8_t data = LCD_INSTRUCTION_SET_DDRAM_ADDRESS
                 | (line_num_offset + digit_num);
    instruction(data);
}

/*============================================================================*/