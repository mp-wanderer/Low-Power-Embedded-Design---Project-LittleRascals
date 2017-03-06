/*
 * ble_functions.c
 *
 * Created: 12/11/2016 12:06:37 PM
 *  Author: Shiva and Rishabh
 */ 


#include "ble_functions.h"

at_ble_status_t status;


/* Callback registered for AT_BLE_DISCONNECTED event */
static at_ble_status_t ble_disconnected_cb (void *param)
{
	debugPrint("\r\nDisconnected");
	connected = false; 
// 	at_ble_connected_t *conn_params;
// 	conn_params = (at_ble_connected_t *)param;
// 	if (conn_params->conn_status == AT_BLE_SUCCESS)
// 	{
// 		printf("Disconnected from 0x%02x%02x%02x%02x%02x%02x",
// 		conn_params->peer_addr.addr[5],
// 		conn_params->peer_addr.addr[4],
// 		conn_params->peer_addr.addr[3],
// 		conn_params->peer_addr.addr[2],
// 		conn_params->peer_addr.addr[1],
// 		conn_params->peer_addr.addr[0]);
// 	}
	ALL_UNUSED(param);
	return AT_BLE_SUCCESS;
}

/* Callback registered for AT_BLE_DISCONNECTED event */
static at_ble_status_t ble_connected_cb (void *param)
{
	debugPrint("\r\nConnected");
	connected = true; 
// 	at_ble_connected_t *conn_params;
// 	conn_params = (at_ble_connected_t *)param;
// 	if (conn_params->conn_status == AT_BLE_SUCCESS)
// 	{
// 		printf("Address is 0x%02x%02x%02x%02x%02x%02x",
// 		conn_params->peer_addr.addr[5],
// 		conn_params->peer_addr.addr[4],
// 		conn_params->peer_addr.addr[3],
// 		conn_params->peer_addr.addr[2],
// 		conn_params->peer_addr.addr[1],
// 		conn_params->peer_addr.addr[0]);
// 	}
 	ALL_UNUSED(param);
	return AT_BLE_SUCCESS;
}

static const ble_event_callback_t app_gap_cb[] = {
	NULL, // AT_BLE_UNDEFINED_EVENT
	NULL, //ble_scan_info_handler, // AT_BLE_SCAN_INFO
	NULL, //ble_scan_report_handler, // AT_BLE_SCAN_REPORT
	NULL, // AT_BLE_ADV_REPORT
	NULL, // AT_BLE_RAND_ADDR_CHANGED
	ble_connected_cb, // AT_BLE_CONNECTED
	ble_disconnected_cb, // AT_BLE_DISCONNECTED
	NULL, // AT_BLE_CONN_PARAM_UPDATE_DONE
	NULL, // AT_BLE_CONN_PARAM_UPDATE_REQUEST
	NULL, //ble_paired_cb, // AT_BLE_PAIR_DONE
	NULL, // AT_BLE_PAIR_REQUEST
	NULL, // AT_BLE_SLAVE_SEC_REQUEST
	NULL, // AT_BLE_PAIR_KEY_REQUEST
	NULL, // AT_BLE_ENCRYPTION_REQUEST
	NULL, // AT_BLE_ENCRYPTION_STATUS_CHANGED
	NULL, // AT_BLE_RESOLV_RAND_ADDR_STATUS
	NULL, // AT_BLE_SIGN_COUNTERS_IND
	NULL, // AT_BLE_PEER_ATT_INFO_IND
	NULL  // AT_BLE_CON_CHANNEL_MAP_IND
};

void ble_advertise (void)
{
	status = ble_advertisement_data_set();
	if(status != AT_BLE_SUCCESS)
	{
		debugPrint("\n\r## Advertisement data set failed : error");
		errorFunc();
	}
	/* Start of advertisement */
	status = at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED,\
								AT_BLE_ADV_GEN_DISCOVERABLE,\
								NULL,\
								AT_BLE_ADV_FP_ANY,\
								1000,\
								655,\
								0);
	if(status != AT_BLE_SUCCESS)
	{
		errorFunc();
	}
	debugPrint("\r\nAdvertising Started");
}

static void register_ble_callbacks (void)
{
	/* Register GAP Callbacks */
	status = ble_mgr_events_callback_handler(REGISTER_CALL_BACK, BLE_GAP_EVENT_TYPE,app_gap_cb);
	if (status != true) {
		debugPrint("\r\n##Error when Registering  SAMB11 gap callbacks");
	}
	else
	{
		debugPrint("\r\nSuccessfully Register bluetooth events callbacks");
	}
}

void initializeBLE(void)
{
	ble_device_init(NULL);
	register_ble_callbacks();
	//ble_set_ulp_mode(BLE_ULP_MODE_CLEAR);
}

