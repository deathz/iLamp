#include "ble_conn.h"							// BLE connection setup function
#include "ilamp_control.h"				// iLamp control function
#include "app_scheduler.h"				// Scheduler

/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize
    timers_init();
    gpiote_init();
    ble_stack_init();
    scheduler_init();    
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();

    // Start execution
    timers_start();
    advertising_start();
	
		buttons_init();
		pwm_init();
		ilamp_init();

    // Enter main loop
    for (;;)
    {
			
    }
}
