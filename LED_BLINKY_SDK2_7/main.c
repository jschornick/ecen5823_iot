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
#define WAKEUP_PERIOD_MS  1000

// Overall lowerst energy mode. This may be raised by individual peripherals.
//#define LOWEST_ENERGY_MODE 3

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


#define BLE_ADV_MIN_MS  100
#define BLE_ADV_MAX_MS  100

#define BLE_TICKS(ms)  ( (ms * 1000) / 625)

// Record converted temperature for easy debugging
float tempC;

int main(void)
{

  // Initialize MCU core (clocks, etc) and board features (display, etc)
  // Must be called at the beginning of main in this order.
  initMcu();
  initBoard();
  // initApp()? PTI?

  // Initialize BT stack and individual stack components
  gecko_init(&config);

  // Create an overall block to force the MCU to stay in at least EM3
  //block_sleep_mode(LOWEST_ENERGY_MODE+1);

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

        /* Set advertising parameters. 100ms advertisement interval. All channels used.
         * The first two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */

    	  // units are 625us
        //gecko_cmd_le_gap_set_adv_parameters(160, 160, 7);
          gecko_cmd_le_gap_set_adv_parameters(BLE_TICKS(BLE_ADV_MIN_MS), BLE_TICKS(BLE_ADV_MAX_MS), 7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        break;


      case gecko_evt_le_connection_opened_id:
    	  // new connection was opened, now set connection params?
    	  // gecko_cmd_le_connection_set_parameters(uint8 connection, uint16 min_interval,
    	  //                      uint16 max_interval, uint16 latency, uint16 timeout);
    	  break;

      case gecko_evt_le_connection_closed_id:

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
    	  // command to get rssi is complete, now set power?
    	  //    response has rssi value
    	  // do we need to call gecko_cmd_le_connection_get_rssi(uint8 connection) on a schedule?
    	  //    use letimer interrupt?

    	  break;

      case gecko_evt_system_external_signal_id:
    	// TODO: check bits of  evt->data.evt_system_external_signal.extsignals

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
    			  tempC = (si7021_read_temp() * 175.72) / 65536 - 46.85;
    			  //  cmd_gatt_server_send_characteristic_notification ??
    			  //  cmd_gatt_server_write_attribute_value ??
    			  if (tempC < LOW_TEMP_ALERT) {
    				  led_on(LED1);
    			  } else {
    				  led_off(LED1);
    			  }
    			  //unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM2
    			  SLEEP_SleepBlockEnd(sleepEM2);
    			  i2c_disable();            // Put I2C and sensor into low-power mode
    			  si7021_poweroff();
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

