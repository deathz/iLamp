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

static void set_channel(int channel, int value);
static void set_all_channel(int value_cn_r, int value_cn_g, int value_cn_b, int value_cn_w);
static void lamp_set_mode(int mode);
