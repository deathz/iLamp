#ifndef ILAMP_CONTROL_H__
#define ILAMP_CONTROL_H__

#include "ilamp_service.h"
#include "ble_conn.h"
#include "app_button.h"									// Button Handler
#include "nrf_pwm.h"										// PWM library

/*
Number of channel & mapping channel with led
*/
#define NUM_CHANNEL						4
#define CN_RED								0
#define CN_GREEN							1
#define CN_BLUE								2
#define CN_WHITE							3

/*
Led pin
*/
#define RED_PIN 							19
#define GREEN_PIN 						20
#define BLUE_PIN 							21
#define WHITE_PIN 						22

/*
Button pin
*/
#define BTN_ON_OFF 						16
#define BTN_MODE 							17
#define BTN_INTENSITY_UP 			14
#define BTN_INTENSITY_DOWN 		15

/*
Mode & mode value
*/
#define MODE1									1
#define MODE2									2
#define MODE3									3
#define MODE4									4
#define MODE5									5

#define MODE1_VALUE									0,0,0,0
#define MODE2_VALUE									64,64,64,64
#define MODE3_VALUE									128,128,128,128
#define MODE4_VALUE									196,196,196,196
#define MODE5_VALUE									255,255,255,255

static int mode_cur;
static int red_cur;
static int green_cur;
static int blue_cur;
static int white_cur;

static nrf_pwm_config_t pwm_config = PWM_DEFAULT_CONFIG;
static bool lamp_on;

// Initialize
void pwm_init(void);
void buttons_init(void);
void ilamp_init(void);

// Control function
void set_channel(int channel, int value);
void set_all_channel(int value_cn_r, int value_cn_g, int value_cn_b, int value_cn_w);
void lamp_set_mode(int mode);
void turn_on_lamp(void);
void turn_off_lamp(void);

#endif // ILAMP_CONTROL_H__
