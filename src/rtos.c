/*******************************************************************************
 * @file   rtos.c
 * @brief  RTOS source file. Provides framework to initialise/start the RTOS.
 ******************************************************************************/

#include "rtos.h"
#include "op_mode.h"
#include "servo.h"
#include "usart.h"

/*===== Defines & Typedefs ===================================================*/
/*===== Task Delays =====*/
#define TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_IF          1000
#define TASK_DELAY_MS__TASK_OP_MODE_MGMT                50
#define TASK_DELAY_MS__TASK_LED_CTRL                    50
#define TASK_DELAY_MS__TASK_SERVO_MOTOR_CTRL            100
/*===== Task Priorities =====*/
#define TASK_PRIORITY__TASK_DEFAULT                     1
#define TASK_PRIORITY__TASK_NUCLEO_COM_PORT_IF          4
#define TASK_PRIORITY__TASK_OP_MODE_MGMT                3
#define TASK_PRIORITY__TASK_LED_CTRL                    2
#define TASK_PRIORITY__TASK_SERVO_MOTOR_CTRL            5
/*===== Task Stack Sizes =====*/
#define TASK_STACK_SIZE__TASK_NUCLEO_COM_PORT_IF        (configMINIMAL_STACK_SIZE*2)
/*===== Task Handles =====*/
static TaskHandle_t task_handle_op_mode_mgmt = NULL;
static TaskHandle_t task_handle_led_ctrl = NULL;

/*===== Private Function Prototypes ==========================================*/
/*===== FreeRTOS Tasks =====*/
static void task_default(void *params __attribute__((unused)));
static void task_nucleo_com_port_if(void *params __attribute__((unused)));
static void task_op_mode_mgmt(void *params __attribute__((unused)));
static void task_led_ctrl(void *params __attribute__((unused)));
static void task_servo_motor_ctrl(void *params __attribute__((unused)));
/*===== Other Private Functions =====*/
static void tasks_init(void);
static void tx_op_mode(void);

/*============================================================================*/
/*===== Public Functions =====================================================*/
/*============================================================================*/

void rtos_init(void)
{
    /* Initialisation (tasks/queues/locks/etc.). */
    tasks_init();

    /* Start scheduler. */
    freertos_wrapper_start_scheduler();
}

/*============================================================================*/
/*===== Private Functions ====================================================*/
/*============================================================================*/

/*===== FreeRTOS Tasks =======================================================*/

/**
 * @brief  RTOS task ---
 *         Default task: spins when no other tasks are executing.
 * @param  params: Unused.
 * @retval None.
 */
static void task_default(void *params __attribute__((unused)))
{
    /* Task. */
    while (1)
    {
        TickType_t ticks = 1 / portTICK_PERIOD_MS;
        vTaskDelay(ticks ? ticks : 1); /* Minimum delay = 1 tick */
    }
}

/**
 * @brief  RTOS task ---
 *         Nucleo COM port interface.
 * @param  params: Unused.
 * @retval None.
 */
static void task_nucleo_com_port_if(void *params __attribute__((unused)))
{
    /* Task. */
    while (1)
    {
        /* Transmit operational mode. */
        tx_op_mode();

        /* Block (delay). */
        freertos_wrapper_task_delay_ms(TASK_DELAY_MS__TASK_NUCLEO_COM_PORT_IF);
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
            freertos_wrapper_task_notify_give(task_handle_led_ctrl);
        }

        /* Block (notification or delay/timeout). */
        freertos_wrapper_task_notify_wait_ms(entry, exit, &nv, TASK_DELAY_MS__TASK_OP_MODE_MGMT);
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
        freertos_wrapper_task_notify_wait_ms(entry, exit, &nv, TASK_DELAY_MS__TASK_LED_CTRL);
    }
}

/**
 * @brief  RTOS task ---
 *         Servo motor control.
 * @param  params: Unused.
 * @retval None.
 */
static void task_servo_motor_ctrl(void *params __attribute__((unused)))
{
    servo_set_signal(true);

    /* Task. */
    while (1)
    {
        // @todo: remove below demonstration
        // Demonstration of moving the shaft through its full range of angles
        for (int i=0; i<=180; i++)
        {
            servo_set_angle(i);
            freertos_wrapper_task_delay_ms(100);
        }
        for (int i=180; i>0; i--)
        {
            servo_set_angle(i);
            freertos_wrapper_task_delay_ms(100);
        }

        /* Block (delay). */
        freertos_wrapper_task_delay_ms(TASK_DELAY_MS__TASK_SERVO_MOTOR_CTRL);
    }
}

/*===== Other Private Functions ==============================================*/

/**
 * @brief  Initialise tasks.
 * @retval None.
 */
static void tasks_init(void)
{
    freertos_wrapper_task_create(task_default,
                                 "task_default",
                                 configMINIMAL_STACK_SIZE,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_DEFAULT,
                                 0);
    freertos_wrapper_task_create(task_nucleo_com_port_if,
                                 "task_nucleo_com_port_if",
                                 TASK_STACK_SIZE__TASK_NUCLEO_COM_PORT_IF,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_NUCLEO_COM_PORT_IF,
                                 0);
    freertos_wrapper_task_create(task_op_mode_mgmt,
                                 "task_op_mode_mgmt",
                                 configMINIMAL_STACK_SIZE,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_OP_MODE_MGMT,
                                 &task_handle_op_mode_mgmt);
    freertos_wrapper_task_create(task_led_ctrl,
                                 "task_led_ctrl",
                                 configMINIMAL_STACK_SIZE,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_LED_CTRL,
                                 &task_handle_led_ctrl);
    freertos_wrapper_task_create(task_servo_motor_ctrl,
                                 "task_servo_motor_ctrl",
                                 configMINIMAL_STACK_SIZE,
                                 (void *)0,
                                 TASK_PRIORITY__TASK_SERVO_MOTOR_CTRL,
                                 0);
}

/**
 * @brief  Construct and transmit a message via the Nucleo COM port interface:
 *             - Message: operational mode.
 * @retval None.
 */
static void tx_op_mode(void)
{
    #define TX_BUFF_MAX 100
    char data[TX_BUFF_MAX] = {0};
    int pos = 0; /* Tx buffer position; for use with sprintf(). */

    /* Construct message. */
    pos += sprintf(&data[pos], "Operational mode: ");
    switch (op_mode_get())
    {
        case OP_MODE__UNKNOWN:
            pos += sprintf(&data[pos], "UNKNOWN");
            break;
        case OP_MODE__IDLE:
            pos += sprintf(&data[pos], "IDLE");
            break;
        case OP_MODE__MOTOR_RUNNING:
            pos += sprintf(&data[pos], "MOTOR RUNNING");
            break;
        case OP_MODE__ERROR_MOTOR:
            pos += sprintf(&data[pos], "ERROR (MOTOR)");
            break;
        case OP_MODE__ERROR_LCD:
            pos += sprintf(&data[pos], "ERROR (LCD)");
            break;
        case OP_MODE__ERROR_PERIPHERALS:
            pos += sprintf(&data[pos], "ERROR (PERIPHERALS)");
            break;
        case OP_MODE__ERROR_FW_FAULT:
            pos += sprintf(&data[pos], "ERROR (FIRMWARE FAULT)");
            break;
        default:
            break;
    }
    sprintf(&data[pos], "\r\n"); /* CRLF. */

    /* Retrieve relevant USART handle. */
    UART_HandleTypeDef *handle = NULL;
    if (usart_get_handle(USART_ID__NUCLEO_COM_PORT, &handle) == false)
    {
        error_handler();
    }

    /* Transmit. */
    usart_tx(handle, (uint8_t *)data, sizeof(data), 1000);
}

/*============================================================================*/