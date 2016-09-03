#include <string.h>
#include "ilamp.h"
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"

/*
	Handle Write event on BLE
*/
static void on_write(ble_ils_t * ilamp_service, ble_evt_t * ble_evt) {
	ble_gatts_evt_write_t * evt_write = &ble_evt->evt.gatts_evt.params.write;
	
	if((evt_write->handle == ilamp_service->led_char_handles.value_handle)
			&& (ilamp_service->led_write_handler != NULL)) {
		ilamp_service->led_write_handler(ilamp_service, evt_write->data[0], evt_write->data[1]);
	} else if(evt_write->handle == ilamp_service->led_char_handles.cccd_handle) {
		
	}
}

/*
	iLamp Service on BLE event
*/
void ilamp_service_on_ble_evt(ble_ils_t * ilamp_service, ble_evt_t * ble_evt) {
	switch(ble_evt->header.evt_id) {
		case BLE_GATTS_EVT_WRITE:
			on_write(ilamp_service, ble_evt);
			break;
		case BLE_GAP_EVT_CONNECTED:
			ilamp_service->conn_handle = ble_evt->evt.gap_evt.conn_handle;
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			ilamp_service->conn_handle = BLE_CONN_HANDLE_INVALID;
			break;
		default:
			break;
	}
}

/*
	Add led characteristic to "iLamp service"
*/
static uint32_t ilamp_led_char_add(ble_ils_t * ilamp_service) {
	uint32_t err_code = 0;
	
	// add custom chracteristic UUID
	ble_uuid_t char_uuid;
	char_uuid.uuid = ILAMP_CHAR_UUID;
	char_uuid.type = ilamp_service->uuid_type;
	
	// add read/write properties to chracteristic metadata
	ble_gatts_char_md_t char_md;
	memset(&char_md, 0, sizeof(char_md));
	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	
	// configuring Client Characteristic Configuration Descriptor (CCCD) metadata & add to char_md
	ble_gatts_attr_md_t cccd_md;
	memset(&cccd_md, 0, sizeof(cccd_md));
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;
	char_md.p_cccd_md = &cccd_md;
	char_md.char_props.notify = 1;
	
	// configure attribute metadata
	ble_gatts_attr_md_t attr_md;
	memset(&attr_md, 0, sizeof(attr_md));
	attr_md.vloc = BLE_GATTS_VLOC_STACK;	
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);	
	
	// configure the characteristic value attribute
	ble_gatts_attr_t attr_char_value;
	memset(&attr_char_value, 0, sizeof(attr_char_value));
	attr_char_value.p_uuid = &char_uuid;
	attr_char_value.p_attr_md = &attr_md;
	
	// set chracteristic length in number of bytes
	attr_char_value.max_len = 2;
	attr_char_value.init_len = 2;
	uint8_t value[2] = {0x12,0x34};
	attr_char_value.p_value = value;
	
	// add our new characteristic to the service
	err_code = sd_ble_gatts_characteristic_add(ilamp_service->service_handle,
																						&char_md,
																						&attr_char_value,
																						&ilamp_service->led_char_handles);
	APP_ERROR_CHECK(err_code);
	
	return NRF_SUCCESS;
}

/*
	Initiating iLamp service
*/
void ilamp_service_init(ble_ils_t * ilamp_service, const led_cn_init_t * led_cn_init) {
	uint32_t err_code = 0;
	
	// declare 128-bit base UUIDs and add them to BLE stack	
	ble_uuid128_t base_uuid = ILAMP_UUID_BASE;	
	//err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	err_code = sd_ble_uuid_vs_add(&base_uuid, &ilamp_service->uuid_type);
	APP_ERROR_CHECK(err_code);
	
	// declare 16-bit service
	ble_uuid_t service_uuid;
	service_uuid.uuid = ILAMP_SERVICE_UUID;
	service_uuid.type = ilamp_service->uuid_type;
	
	// set iLamp service connection handle to default value
	ilamp_service->conn_handle = BLE_CONN_HANDLE_INVALID;
	ilamp_service->led_write_handler = led_cn_init->led_write_handler;
	
	// add iLamp service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
																			&service_uuid,
																			&ilamp_service->service_handle);
	APP_ERROR_CHECK(err_code);
	
	// add our new characreristic
	ilamp_led_char_add(ilamp_service);
}

/*
	Optional for notification
*/
void ilamp_characteristic_update(ble_ils_t * ilamp_service, int32_t value) {
	if (ilamp_service->conn_handle != BLE_CONN_HANDLE_INVALID) {
		uint16_t len = 4;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));
		
		hvx_params.handle = ilamp_service->led_char_handles.value_handle;
		hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len = &len;
		hvx_params.p_data = (uint8_t *)value;
		
		sd_ble_gatts_hvx(ilamp_service->conn_handle, &hvx_params);
	}
}
