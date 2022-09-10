/*******************************************************************************
 * @file   rtos.c
 * @brief  RTOS components (tasks/queues/locks/etc.) initialisation source file.
 ******************************************************************************/

#include "rtos.h"
#include "op_mode.h"
#include "usart.h"

/*===== Defines & Typedefs ===================================================*/
/*===== Task Delays =====*/
#define TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_IF          1000
#define TASK_DELAY_MS__TASK_OP_MODE_MGMT                50
#define TASK_DELAY_MS__TASK_LED_CTRL                    50
/*===== Task Priorities =====*/
#define TASK_PRIORITY__TASK_NUCLEO_COM_PORT_IF          3 // @todo: priority labels?
#define TASK_PRIORITY__TASK_OP_MODE_MGMT                2
#define TASK_PRIORITY__TASK_LED_CTRL                    1
/*===== Task Stack Sizes =====*/
#define TASK_STACK_SIZE__TASK_NUCLEO_COM_PORT_IF        (configMINIMAL_STACK_SIZE*3)
/*===== Task Handles =====*/
static TaskHandle_t task_handle_op_mode_mgmt = NULL;
static TaskHandle_t task_handle_led_ctrl = NULL;

/*===== Private Function Prototypes ==========================================*/
static void task_nucleo_com_port_if(void *params __attribute__((unused)));
static void task_op_mode_mgmt(void *params __attribute__((unused)));
static void task_led_ctrl(void *params __attribute__((unused)));

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void rtos_init(void)
{
    // @todo: create a FreeRTOS wrapper (to avoid duplicate code)
    BaseType_t ret;

    ret = xTaskCreate(task_nucleo_com_port_if,
                      "task_nucleo_com_port_if",
                      TASK_STACK_SIZE__TASK_NUCLEO_COM_PORT_IF,
                      (void *)0,
                      TASK_PRIORITY__TASK_NUCLEO_COM_PORT_IF,
                      0);
    if (ret != pdPASS)
    {
        /* Error - Task not created (insufficient memory allocation). */
        error_handler();
    }

    ret = xTaskCreate(task_op_mode_mgmt,
                      "task_op_mode_mgmt",
                      configMINIMAL_STACK_SIZE,
                      (void *)0,
                      TASK_PRIORITY__TASK_OP_MODE_MGMT,
                      &task_handle_op_mode_mgmt);
    if (ret != pdPASS)
    {
        /* Error - Task not created (insufficient memory allocation). */
        error_handler();
    }

    ret = xTaskCreate(task_led_ctrl,
                      "task_led_ctrl",
                      configMINIMAL_STACK_SIZE,
                      (void *)0,
                      TASK_PRIORITY__TASK_LED_CTRL,
                      &task_handle_led_ctrl);
    if (ret != pdPASS)
    {
        /* Error - Task not created (insufficient memory allocation). */
        error_handler();
    }
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/*===== FreeRTOS Tasks =======================================================*/

/**
 * @brief  RTOS task ---
 *         Nucleo COM port interface.
 * @param  params: Unused.
 * @retval None.
 */
static void task_nucleo_com_port_if(void *params __attribute__((unused)))
{
    #define TX_BUFF_MAX 100
    char tx_buff[TX_BUFF_MAX] = {0};
    int pos; /* TX buffer position for use w/ sprintf(). */

    /* Retrieve relevant USART handle. */
    UART_HandleTypeDef *handle = NULL;
    if (usart_get_handle(USART_ID__NUCLEO_COM_PORT, &handle) == false)
    {
        error_handler();
    }

    /* Task. */
    while (1)
    {
        /* Transmit operational mode. */
        memset(tx_buff, 0x00, sizeof(tx_buff)); /* Zeroise Tx buffer. */
        pos = 0; /* Reset TX buffer position for use w/ sprintf(). */
        pos += sprintf(&tx_buff[pos], "Operational mode: ");
        switch (op_mode_get())
        {
            case OP_MODE__UNKNOWN:           pos += sprintf(&tx_buff[pos], "UNKNOWN");                break;
            case OP_MODE__IDLE:              pos += sprintf(&tx_buff[pos], "IDLE");                   break;
            case OP_MODE__MOTOR_RUNNING:     pos += sprintf(&tx_buff[pos], "MOTOR RUNNING");          break;
            case OP_MODE__ERROR_MOTOR:       pos += sprintf(&tx_buff[pos], "ERROR (MOTOR)");          break;
            case OP_MODE__ERROR_LCD:         pos += sprintf(&tx_buff[pos], "ERROR (LCD)");            break;
            case OP_MODE__ERROR_PERIPHERALS: pos += sprintf(&tx_buff[pos], "ERROR (PERIPHERALS)");    break;
            case OP_MODE__ERROR_FW_FAULT:    pos += sprintf(&tx_buff[pos], "ERROR (FIRMWARE FAULT)"); break;
            default:                                                                                break;
        }
        sprintf(&tx_buff[pos], "\r\n");
        usart_tx(handle, (uint8_t*)tx_buff, sizeof(tx_buff), 1000);

        /* Block (delay). */
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_IF));
    }
}

/**
 * @brief  RTOS task ---
 *         Operational mode management.
 * @param  params: Unused.
 * @retval None.
 */
static void task_op_mode_mgmt(void *params __attribute__((unused)))
{
    bool mode_changed;

    /* Task notify wait values. */
    uint32_t entry = 0x00;     /* Bits to clear on entry: do not clear any bits. */
    uint32_t exit = ULONG_MAX; /* Bits to clear on exit: reset the value to 0.   */
    uint32_t nv;               /* Variable to store a notification value.        */

    /* Task. */
    while (1)
    {
        /* Update operational mode. */
        mode_changed = op_mode_update();

        /* Notify LEDs control task to run as soon as possible *if* the mode has changed. */
        if (mode_changed)
        {
            xTaskNotifyGive(task_handle_led_ctrl);
        }

        /* Block (notification or delay/timeout). */
        xTaskNotifyWait(entry, exit, &nv, pdMS_TO_TICKS(TASK_DELAY_MS__TASK_OP_MODE_MGMT));
    }
}

/**
 * @brief  RTOS task ---
 *         Control of status LEDs.
 * @param  params: Unused.
 * @retval None.
 */
static void task_led_ctrl(void *params __attribute__((unused)))
{
    /* Task notify wait values. */
    uint32_t entry = 0x00;     /* Bits to clear on entry: do not clear any bits. */
    uint32_t exit = ULONG_MAX; /* Bits to clear on exit: reset the value to 0.   */
    uint32_t nv;               /* Variable to store a notification value.        */

    /* Task. */
    while (1)
    {
        op_mode_set_leds();
       
        /* Block (notification or delay/timeout). */
        xTaskNotifyWait(entry, exit, &nv, pdMS_TO_TICKS(TASK_DELAY_MS__TASK_LED_CTRL));
    }
}

/*============================================================================*/