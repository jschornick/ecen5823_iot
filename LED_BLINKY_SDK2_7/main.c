// File        : main.c
// Description : Low-power temperature alarm and BLE server.
//
// This program uses an interrupt-triggered event loop to periodically read an
// I2C temperature sensor, while remaining in a low-power state as much as possible.
//
// An LED alert is triggered when the temperature is below the configured threshold.
//
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

// Board and MCU initialization routines from Silicon Labs
#include "init_mcu.h"
#include "init_board.h"
//  init_app.h ?
#include "board_features.h"  // defines BGM121 (BRD4302A) features
#include "bsp_trace.h"


// NOTE: HAL_CONFIG=1 and EFR32BG1B232F256GM56=1 defined in Properties -> Settings -> Symbols

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"  // bare-metal BT stack API
#include "gatt_db.h"
#include "infrastructure.h"  // type conversion macros


#include "sleep.h"

//#include "sleepmodes.h"
#include "led.h"
#include "i2c.h"
#include "si7021.h"
#include "letimer.h"
#include "scheduler.h"

// The low temperature alert setting, in degress Celsius
#define LOW_TEMP_ALERT 15.0

// Period between samples in milliseconds
#define WAKEUP_PERIOD_MS  4000

// Max simultaneous BT connections
#define MAX_CONNECTIONS 4

// BT stack memory allocation for connections and internal buffers
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,  // no restrictions, see GECKO_OTA_FLAGS_*
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu;


#define TX_POW_MIN -260     /* -26 dBm (per BGM121 datasheet) */
#define TX_POW_MAX 80   /* 8 dBm */

// Set to 337
#define BLE_ADV_MIN_MS  337
#define BLE_ADV_MAX_MS  337
#define BLE_CONN_IVAL_MIN_MS  75
#define BLE_CONN_IVAL_MAX_MS  75
#define BLE_SLAVE_LATENCY_MS  450
#define BLE_SUPERVISOR_TIMEOUT_MS  2000

#define BLE_ADV_VAL(ms)  ( (ms * 1000) / 625 )
#define BLE_CONN_VAL(ms)  ( (ms * 1000) / 1250 )

// Latency intervals the slave can skip based on maximum time in ms
//   Ex: 450 max, intervals = 450 / 75 - 1 = 5
// Subtract one since we could start at the very end of the "first" interval
#define BLE_LATENCY_IVALS(ms) ( ms / BLE_CONN_IVAL_MAX_MS - 1 )

// Record converted temperature for easy debugging
float tempC;


void temp_update( uint16_t temp_data)
{
	// temperature_measurement characteristc (0x2A1C)
	// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature_measurement.xml
	//  byte    0   :  flags       (0 = temp in C, no timestamp, no type)
	//  bytes   1-4 :  temp_deg_C  (IEEE-11073 32-bit float)
	//  bytes   5-8 :  temp_deg_F
	//  bytes  9-12 :  timestamp
	//  bytes  13-16:  temp_type

	int32_t temp_mC;        // temperature in milli-celsius
	uint8_t buffer[5];       // temporary buffer for indication data
	uint8_t flags = 0x00;    // 0 = temp in C, no timestamp, no type
	uint32_t temp_float;     // temperature in BLE FLOAT format
	uint8_t *p_buf = buffer; // current position in buffer

    // place flags at byte 0 in buffer
	UINT8_TO_BITSTREAM(p_buf, flags);

	// tempC   =  175.72 * TC / 65536 - 46.85
	// temp_mC = 175720 * TC / 65536 - 46850
	// temp_mC = 21965 * TC / 8192 - 46850
	temp_mC = 21965L * temp_data / 8192 - 46850;

	// Use built-in conversion routines to create BLE 32-bit FLOAT format
	temp_float = FLT_TO_UINT32(temp_mC , -3);
	// shift 4-bytes onto buffer
	UINT32_TO_BITSTREAM(p_buf, temp_float);
	// send BLE indication to all (0xFF) connections
	gecko_cmd_gatt_server_send_characteristic_notification(
	      0xFF, gattdb_temperature_measurement, 5, buffer);
}


struct gecko_msg_system_set_tx_power_rsp_t * safe_set_tx_power(int16 power) {
	struct gecko_msg_system_set_tx_power_rsp_t * rsp;
	gecko_cmd_system_halt(1);
	rsp = gecko_cmd_system_set_tx_power(power);
	gecko_cmd_system_halt(0);
	return rsp;
}

uint16 interval = 0;
uint16 latency = 0;
uint16 timeout = 0;
int8 rssi = 0;
int16 power = 0;

int main(void)
{



  // Initialize MCU core (clocks, etc) and board features (display, etc)
  // Must be called at the beginning of main in this order.
  initMcu();
  initBoard();
  // initApp()? PTI?

  // Initialize BT stack and individual stack components
  gecko_init(&config);

  // LEDs only active during temperature alert
  led_init();
  led_off(LED0);
  led_off(LED1);

  letimer_init();

  // setup gpio related to si7021, don't power on
  si7021_init();

  // Repeating timer to wakeup every X ms to take a sample
  set_timer(0, WAKEUP_PERIOD_MS, TIMER_REPEAT, EVENT_SAMPLE_WAKEUP);

  // Enable code correlation profiler for energy mode debugging
  BSP_TraceProfilerSetup();

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    // Block (sleep) until there is an event on the stack
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {

      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

		  gecko_cmd_system_set_tx_power(0);  // start at 0 dBm

        /* Set advertising parameters. 100ms advertisement interval. All channels used.
         * The first two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */

    	  // units are 625us
        //gecko_cmd_le_gap_set_adv_parameters(160, 160, 7);
          gecko_cmd_le_gap_set_adv_parameters(BLE_ADV_VAL(BLE_ADV_MIN_MS), BLE_ADV_VAL(BLE_ADV_MAX_MS), 7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        break;


      case gecko_evt_le_connection_opened_id:
    	  // new connection was opened, now set connection params
    	  //gecko_cmd_le_connection_set_parameters(uint8 connection, uint16 min_interval,
    	  //                      uint16 max_interval, uint16 latency, uint16 timeout);
       	  gecko_cmd_le_connection_set_parameters(
       			evt->data.evt_le_connection_opened.connection,
       			  BLE_CONN_VAL(BLE_CONN_IVAL_MIN_MS), BLE_CONN_VAL(BLE_CONN_IVAL_MAX_MS),
				  BLE_LATENCY_IVALS(BLE_SLAVE_LATENCY_MS),
				  //0,  // set latency to zero
				  BLE_SUPERVISOR_TIMEOUT_MS / 10);

       	  // check our signal strength and adjust as soon as possible
       	  //  NOTE: it seems this always results in a very high RSSI, possibly due to
       	  //     the client transmitting with high power at connection start
       	  //gecko_cmd_le_connection_get_rssi( evt->data.evt_le_connection_opened.connection );
    	  break;


      case gecko_evt_le_connection_closed_id:

		  gecko_cmd_system_set_tx_power(0);  // start at 0 dBm

    	  /* Check if need to boot to dfu mode */

    	  if (boot_to_dfu) {
    		/* Enter to DFU OTA mode */
        	// how is this different than gecko_cmd_dfu_reset()?
            gecko_cmd_system_reset(2);
          } else {
            /* Restart advertising after client has disconnected */
            gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
          }
          break;

      // connection parameters have changed event
      case gecko_evt_le_connection_parameters_id:

      	  interval = evt->data.evt_le_connection_parameters.interval;
      	  timeout = evt->data.evt_le_connection_parameters.timeout;
      	  latency = evt->data.evt_le_connection_parameters.latency;
      	  break;

      // confirm of indication or characteristic updated by by client
      	  // this is a reasonable opportunity to check RSSI according to Prof. Graham
      case gecko_evt_gatt_server_characteristic_status_id:
    	  gecko_cmd_le_connection_get_rssi( evt->data.evt_gatt_server_characteristic_status.connection );
    	  break;



      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      case gecko_evt_le_connection_rssi_id:
		  // power is in units of 0.1 dBm
		  //   So power = -50 ==> -5 dBm
    	  rssi = evt->data.evt_le_connection_rssi.rssi;
    	  if ( rssi > -35 ) {
    		  power = TX_POW_MIN;
    	  } else if ( rssi > -45 ) {
    		  power = -200;
    	  } else if ( rssi > -55 ) {
    		  power = -150;
    	  } else if ( rssi > -65 ) {
    		  power = -50;
    	  } else if ( rssi > -75 ) {
    		  power = 0;
    	  } else if ( rssi > -85 ) {
    		  power = 50;
    	  } else {
    		  power = TX_POW_MAX;
    	  }

    	  // Per the SiLabs BT API reference, we can't be advertising, scanning, or connected
    	  // So halt the radio, change power, and restart it before anyone notices.
    	  // Also block EM2 since halt puts us to sleep
		  //SLEEP_SleepBlockBegin(sleepEM2);
    	  safe_set_tx_power(power);
    	  //gecko_cmd_system_halt(1);
		  //gecko_cmd_system_set_tx_power(power);
    	  //gecko_cmd_system_halt(0);
		  //SLEEP_SleepBlockEnd(sleepEM2);

    	  break;

      case gecko_evt_system_external_signal_id:
    	// TODO: switch event system to use bits of  evt->data.evt_system_external_signal.extsignals

    	if ( CHECK_EVENT(EVENT_SAMPLE_WAKEUP) ) {
    			  // We've woken up out of our primary sleep loop
    			  CLEAR_EVENT(EVENT_SAMPLE_WAKEUP);
    			  si7021_poweron();
    			  set_timer(1, SI7021_WARMUP_MS, TIMER_ONCE, EVENT_SENSOR_READY);

    	}
    	if (event_flags & EVENT_SENSOR_READY) {
    			  // The sensor has completed the warmup phase
    			  CLEAR_EVENT(EVENT_SENSOR_READY);
    			  i2c_init();             // Bring up the I2C peripheral and
    			  //block_sleep_mode(EM2);  // stay in EM1 until the response is read
    			  SLEEP_SleepBlockBegin(sleepEM2);
    			  si7021_request_temp();
    	}
    	if (event_flags & EVENT_I2C_MSG) {
    			  // We've received an I2C response from the sensor (waiting in TX buffer)
    			  CLEAR_EVENT(EVENT_I2C_MSG);

    			  uint16_t temp_data = si7021_read_temp();

    			  //unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM2
    			  SLEEP_SleepBlockEnd(sleepEM2);
    			  i2c_disable();            // Put I2C and sensor into low-power mode
    			  si7021_poweroff();

    			  tempC = (temp_data * 175.72) / 65536 - 46.85;
    			  //  cmd_gatt_server_send_characteristic_notification ??
    			  //  cmd_gatt_server_write_attribute_value ??
    			  if (tempC < LOW_TEMP_ALERT) {
    				  led_on(LED1);
    			  } else {
    				  led_off(LED1);
    			  }
    			  temp_update(temp_data);

    	}
    	break;

      default:
        break;
    }
  }

}


  // native_gecko.h:
  //     gecko_cmd_gatt_server_send_characteristic_notification(uint8 connection,uint16 characteristic,uint8 value_len, const uint8* value_data)
  //     gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temp_measurement, 5, htmTempBuffer);

