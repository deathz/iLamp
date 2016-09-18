#include "ilamp_control.h"

//-------------------------- Event Handler -------------------------------------------- 

static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
		if(button_action == APP_BUTTON_PUSH) {
			switch (pin_no)
			{
					case BTN_ON_OFF:
							if (lamp_on) {
								turn_off_lamp();
							} else {
								turn_on_lamp();
							}
							ilamp_switch_characteristic_update(get_ilamp_service(), lamp_on);
							break;
					case BTN_MODE:
							lamp_set_mode(mode_cur);
							mode_cur++;
							if (mode_cur == 6)
								mode_cur = 1;
							break;
					case BTN_INTENSITY_UP:
							break;
					case BTN_INTENSITY_DOWN:
							break;
					default:
							APP_ERROR_HANDLER(pin_no);
							break;
			}			
		}
		else if(button_action == APP_BUTTON_RELEASE) {
		}
}

//-------------------------- Init function --------------------------------------------

void pwm_init(void) {
    pwm_config.mode             = PWM_MODE_LED_255;
    pwm_config.num_channels     = NUM_CHANNEL;
    pwm_config.gpio_num[CN_RED]      	= RED_PIN;
    pwm_config.gpio_num[CN_GREEN]      = GREEN_PIN;
    pwm_config.gpio_num[CN_BLUE]      	= BLUE_PIN;
    pwm_config.gpio_num[CN_WHITE]      = WHITE_PIN;	
		
		nrf_pwm_init(&pwm_config);
}

void buttons_init(void)
{
    // Note: Array must be static because a pointer to it will be saved in the Button handler
    //       module.
    static app_button_cfg_t buttons[] =
    {
        {BTN_ON_OFF, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_event_handler},
        {BTN_MODE, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_event_handler},
				{BTN_INTENSITY_UP, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_event_handler},
				{BTN_INTENSITY_DOWN, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_event_handler}
    };

    APP_BUTTON_INIT(buttons, sizeof(buttons) / sizeof(buttons[0]), BUTTON_DETECTION_DELAY, true);
		app_button_enable();
}

void ilamp_init(void)
{
		lamp_set_mode(MODE3);		
		lamp_on = true;
}

//-------------------------- End init function --------------------------------------------

void set_channel(int channel, int value) {
    switch (channel)
    {
				case CN_RED:
						red_cur = value;
						break;
				case CN_GREEN:
						green_cur = value;
						break;
				case CN_BLUE:
						blue_cur = value;
						break;
        case CN_WHITE:
						white_cur = value;
						break;			
        default:
            APP_ERROR_HANDLER(channel);
            break;
    }
		nrf_pwm_set_value(channel, value);
}

void set_all_channel(int value_cn_r, int value_cn_g, int value_cn_b, int value_cn_w) {
    set_channel(CN_RED,value_cn_r);
    set_channel(CN_GREEN,value_cn_g);
    set_channel(CN_BLUE,value_cn_b);
		set_channel(CN_WHITE,value_cn_w);
}

void lamp_set_mode(int mode) {
		mode_cur = mode;
    switch (mode)
    {
				case MODE1:
						set_all_channel(MODE1_VALUE);
						break;
				case MODE2:
						set_all_channel(MODE2_VALUE);
						break;
				case MODE3:
						set_all_channel(MODE3_VALUE);
						break;
        case MODE4:
						set_all_channel(MODE4_VALUE);
						break;
        case MODE5:
						set_all_channel(MODE5_VALUE);
						break;				
        default:
            APP_ERROR_HANDLER(mode);
            break;
    }
}

void turn_on_lamp(void) {
		set_all_channel(red_cur,green_cur,blue_cur,white_cur);
		lamp_on = true;
}

void turn_off_lamp(void) {
		nrf_pwm_set_value(CN_RED, 0);
		nrf_pwm_set_value(CN_GREEN, 0);
		nrf_pwm_set_value(CN_BLUE, 0);
		nrf_pwm_set_value(CN_WHITE, 0);
		lamp_on = false;
}

bool is_lamp_on(void) {
	return lamp_on;
}
