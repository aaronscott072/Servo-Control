/*******************************************************************************
 * @file   main.c
 * @brief  Main source file.
 ******************************************************************************/

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "leds.h"
#include "op_mode.h"
#include "rtos.h"
#include "timer.h"
#include "usart.h"

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

int main(void)
{
    HAL_Init();
    clock_config();
    leds_init();
    usart_init();

    /* Initialise RTOS components. */
    extern void MX_FREERTOS_Init(void);
    MX_FREERTOS_Init();
    rtos_init();

    /* Start scheduler. */
    freertos_wrapper_start_scheduler();

    /**
     * Flag an error if we ever get here as control should  
     * have been taken by the scheduler.
     */
    error_handler();
}

void error_handler(void)
{
    __disable_irq();
    op_mode_set_error_fw_fault();
    assert(0);
    while (1) {;}
}

void freertos_wrapper_error_handler(void)
{
    error_handler();
}

/*============================================================================*/