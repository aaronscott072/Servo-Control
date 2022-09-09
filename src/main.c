/*******************************************************************************
 * @file   main.c
 * @brief  Main source file.
 ******************************************************************************/

#include "main.h"
#include "clock.h"
#include "gpio.h"
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
    MX_GPIO_Init();
    usart_init();

    /* Initialise RTOS components. */
    extern void MX_FREERTOS_Init(void);
    MX_FREERTOS_Init();
    rtos_init();

    /* Start scheduler. */
    vTaskStartScheduler();

    /**
     * Flag an error if we ever get here as control should  
     * have been taken by the scheduler.
     */
    error_handler();
}

void error_handler(void)
{
    assert(false);
    __disable_irq();
    while (1)
    {
        // @todo: implement any further error handling? (e.g. LED / USART msg)
    }
}

/*============================================================================*/