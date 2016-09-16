#include "ilamp_service.h"											// iLamp service

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"							// Common defines and macros for firmware developed by Nordic Semiconductor.
#include "nrf.h"												// Family selection for main includes. NRF51 must be selected.
#include "app_error.h"									// Common application error handler
#include "nrf_gpio.h"										// nrf_gpio GPIO abstraction
#include "nrf51_bitfields.h"						// nrf51 bit fields
#include "ble.h"												// SoftDevice Common
#include "ble_hci.h"										// Bluetooth status codes
#include "ble_srv_common.h"							// Common service definitions
#include "ble_advdata.h"								// Advertising Data Encoder
#include "ble_conn_params.h"						// Connection Parameters Negotiation
#include "boards.h"											// Board type
#include "app_scheduler.h"							// Scheduler
#include "softdevice_handler.h"					// SoftDevice Event Handler
#include "app_timer.h"									// Application Timer
#include "ble_error_log.h"							// Error Log Modules
#include "app_gpiote.h"									// GPIOTE Handler
#include "app_button.h"									// Button Handler
#include "ble_debug_assert_handler.h"		// Assert Handler for debug purposes
#include "pstorage.h"										// Persistent Storage Interface
#include "bsp.h"												// Board Support Package

#include "nrf_delay.h"									// Delay function
#include "SEGGER_RTT.h"									// Debug printf function

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                           /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define WAKEUP_BUTTON_PIN               BSP_BUTTON_0                                /**< Button used to wake up the application. */

#define ADVERTISING_LED_PIN_NO          BSP_LED_0                                   /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO            BSP_LED_1                                   /**< Is on when device has connected. */

#define LEDBUTTON_LED_PIN_NO            BSP_LED_0
#define LEDBUTTON_BUTTON_PIN_NO         BSP_BUTTON_1

#define DEVICE_NAME                     "iLamp"                             				/**< Name of device. Will be included in the advertising data. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            2                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)            /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define SEC_PARAM_TIMEOUT               30                                          /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                  1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                           /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                          /**< Maximum encryption key size. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */


// Initialize
void timers_init(void);
void gpiote_init(void);
void ble_stack_init(void);
void scheduler_init(void);    
void gap_params_init(void);
void services_init(void);
void advertising_init(void);
void conn_params_init(void);
void sec_params_init(void);

// Start execution
void timers_start(void);
void advertising_start(void);

// Power
void power_manage(void);

static ble_ils_t												m_ilamp_service;
