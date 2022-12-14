/*******************************************************************************
 * @file   main.c
 * @brief  Main source file.
 ******************************************************************************/

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "helper.h"
#include "lcd.h"
#include "leds.h"
#include "op_mode.h"
#include "rtos.h"
#include "servo.h"
#include "timer.h"
#include "usart.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

int main(void)
{
    hal_init();
    clock_config();
    lcd_init();
    leds_init();
    servo_init();
    usart_init();

    /* Initialise and start the RTOS. */
    rtos_init();

    /**
     * Flag an error if we ever get here as control should  
     * have been taken by the scheduler.
     */
    error_handler();
}

void error_handler(void)
{
    __disable_irq();

    /**
     * Set LED status indication for a firmware fault.
     * Ensure GPIOs are configured before attempting to enable LEDs.
     */
    leds_init();
    op_mode_set_error_fw_fault();

    assert(0);
    while (1) {;}
}

/*===== Overwriting Weakly Defined Functions =================================*/

void freertos_wrapper_error_handler(void)
{
    error_handler();
}

void lcd_delay_ms(uint32_t ms)
{
    if (freertos_wrapper_is_scheduler_running())
    {
        freertos_wrapper_task_delay_ms(ms);
    }
    else
    {
        HAL_Delay(ms);
    }
}

/*============================================================================*/