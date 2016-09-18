#ifndef ILAMP_SERVICE_H__
#define ILAMP_SERVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nordic_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"

#define ILAMP_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define ILAMP_SERVICE_UUID 0xF00D
#define ILAMP_LED_CHAR_UUID 0xBEEF
#define ILAMP_SWITCH_CHAR_UUID 0xFEED

typedef struct ble_ils_s ble_ils_t;

/*
	Event handle when change led value
*/
typedef void (*ble_ils_led_write_handler_t) (ble_ils_t * ils, uint8_t channel, uint8_t value);
/*
	Event handle when change switch value
*/
typedef void (*ble_ils_switch_write_handler_t) (ble_ils_t * ils, uint8_t state);

typedef struct
{
    ble_ils_led_write_handler_t led_write_handler;
} led_cn_init_t;

typedef struct
{
    ble_ils_switch_write_handler_t switch_write_handler;
} led_switch_init_t;

/*
	iLamp Service Struct
*/
typedef struct ble_ils_s
{
		uint8_t                     uuid_type;
		uint16_t                    conn_handle;
    uint16_t                    service_handle;        
		ble_gatts_char_handles_t		led_char_handles;
		ble_gatts_char_handles_t		switch_char_handles;
		ble_ils_led_write_handler_t	led_write_handler;
		ble_ils_switch_write_handler_t	switch_write_handler;
} ble_ils_t;

void ilamp_service_on_ble_evt(ble_ils_t * ilamp_service, ble_evt_t * ble_evt);

void ilamp_service_init(ble_ils_t * ilamp_service);
void ilamp_service_add_led_handler(ble_ils_t * ilamp_service, const led_cn_init_t * led_cn_init);
void ilamp_service_add_switch_handler(ble_ils_t * ilamp_service, const led_switch_init_t * switch_init);

// For notification
void ilamp_switch_characteristic_update(ble_ils_t * ilamp_service, bool switch_state);

#endif // ILAMP_SERVICE_H__
