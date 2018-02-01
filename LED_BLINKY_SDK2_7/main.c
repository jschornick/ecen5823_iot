// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3
// Attribution : This file uses code from the "Silicon Labs Empty Example Project" as indicated.

// NOTE: The timer configuration parameters (minimum energy mode, period, on time) are all
//       defined in letimer.h

/***********************************************************************************************
 * Silicon Labs Empty Example Project
 ***********************************************************************************************
 * (C) Copyright 2016 Silicon Labs, http://www.silabs.com
 ***********************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 ***********************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
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
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

// End of Silicon Labs sample configuration
//****************************************************************************

#include "cmu.h"
#include "gpio.h"
#include "led.h"

#include "src/sleep.h" // make sure we include our custom sleep.h, not the one from emdrv
#include "letimer.h"

// General program operation modes for energy testing
#define MODE_LED_ON  1
#define MODE_LED_OFF 2
#define MODE_BLINK   3

#define MODE  MODE_BLINK

// Overall lowerst energy mode. This may be raised by individual peripherals.
#define LOWEST_ENERGY_MODE 3

int main(void)
{

  // Device and board initialization
  initMcu();
  initBoard();

  // Initialize clocks
  // cmu_init();
  //
  // Clocks for HW#1 are configured in the individual peripheral
  // initialization routines.

  // Initialize stack and BGAPI using the example configuration from Silicon Labs
  gecko_init(&config);

  // Create an overall block to force the MCU to stay in a certain energy mode.
  block_sleep_mode(LOWEST_ENERGY_MODE+1);

  /* Initialize LED via corresponding GPIOs */
  led_init();

#if MODE == MODE_BLINK
  led_off(LED0);
  led_off(LED1);
  // Configure the low-energy timer and start it
  letimer_init();
#elif MODE == MODE_LED_ON
  led_on(LED0);
  led_off(LED1);
#elif MODE == MODE_LED_OFF
  led_off(LED0);
  led_off(LED1);
#endif

  // The program is entirely event driven. Sleep immediately after interrupts are serviced.
  while (1) {
	  sleep();
  }

}

