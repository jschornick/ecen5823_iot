// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3
// Attribution : This file uses code from the "Silicon Labs Empty Example Project" as indicated.

// NB: The low-energy timer configuration (minimum energy mode, etc) is in letimer.h

/***********************************************************************************************//**
 * Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

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


// End of Silicon Labs example configuration


//***********************************************************************************
// Include files
//***********************************************************************************

//#include "main.h"
#include "cmu.h"
#include "gpio.h"
#include "led.h"

#include "src/sleep.h" // make sure we include our custom sleep.h, not the one from emdrv
#include "letimer.h"


#define LED_BLINK_PERIOD    1.0  // in seconds??
#define LED_BLINK_ON_TIME   0.2  // 200 ms


int main(void)
{
  int i;

  // Initialize device
  initMcu();

  // Initialize board
  initBoard();

  // Initialize clocks
  //cmu_init();

  /* Initialize GPIO */
  led_init();


  // Initialize stack and BGAPI using the example configuration from Silicon Labs
  gecko_init(&config);

  letimer_init();

  //  GPIO_PinOutSet(LED0_port, LED0_pin);
  //  GPIO_PinOutSet(LED1_port, LED1_pin);
  led_on(LED0);
  led_on(LED1);

  while (1) {
	  for (i = 0; i < 500000; i++);
	  //GPIO_PinOutClear(LED0_port, LED0_pin);
	  led_off(LED0);

	  for (i = 0; i < 500000; i++);
	  //GPIO_PinOutClear(LED1_port, LED1_pin);
	  led_off(LED1);

	  sleep();
  }
}

