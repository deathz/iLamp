/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */
 
/**
 * This file is the main file for the application described in application note
 * nAN-36 Creating Bluetooth® Low Energy Applications Using nRF51822.
 */

#include "ble_conn.h"							// BLE connection setup function
#include "ilamp_control.h"				// iLamp control function
#include "app_scheduler.h"				// Scheduler

/**@brief Function for application main entry.
 */
int main(void)
{
		//nrf_gpio_cfg_output(ADVERTISING_LED_PIN_NO);
		//nrf_gpio_cfg_output(CONNECTED_LED_PIN_NO);
		//nrf_gpio_cfg_output(LEDBUTTON_BUTTON_PIN_NO);
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
		
		//SEGGER_RTT_printf(0, "main > lamp_on: %d\n", is_lamp_on());
		//SEGGER_RTT_WriteString(0, "Hello iLamp!\n");

    // Enter main loop
    for (;;)
    {
        //app_sched_execute();
        //power_manage();
    }
}

/**
 * @}
 */
