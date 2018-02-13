// File        : main.c
// Description : Low-power temperature alarm.
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
#include "board_features.h"  // defines BGM121 (BRD4302A) features
#include "bsp_trace.h"

#include "sleepmodes.h"
#include "led.h"
#include "i2c.h"
#include "si7021.h"
#include "letimer.h"
#include "scheduler.h"

// The low temperature alert setting, in degress Celsius
#define LOW_TEMP_ALERT 15.0

// Period between samples in milliseconds
#define WAKEUP_PERIOD_MS  2000

// Overall lowerst energy mode. This may be raised by individual peripherals.
#define LOWEST_ENERGY_MODE 3

// Record converted temperature for easy debugging
float tempC;

int main(void)
{

  // Device and board initialization
  initMcu();
  initBoard();

  // Initialize BT stack and BGAPI using the example configuration from Silicon Labs
  //gecko_init(&config);

  // Create an overall block to force the MCU to stay in at least EM3
  block_sleep_mode(LOWEST_ENERGY_MODE+1);

  // LEDs only active during temperature alert
  led_init();
  led_off(LED0);
  led_off(LED1);

  letimer_init();

  // setup gpio related to si7021, don't power on
  void si7021_init(void);

  // Repeating timer to wakeup every X ms to take a sample
  set_timer(0, WAKEUP_PERIOD_MS, TIMER_REPEAT, EVENT_SAMPLE_WAKEUP);

  // Enable code correlation profiler for energy mode debugging
  // BSP_TraceProfilerSetup();

  // The program is entirely event driven. Sleep immediately after events are serviced.
  while (1) {
	  while( event_flags ) {
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
			  block_sleep_mode(EM2);  // stay in EM1 until the response is read
			  si7021_request_temp();
		  }
		  if (event_flags & EVENT_I2C_MSG) {
			  // We've received an I2C response from the sensor (waiting in TX buffer)
			  CLEAR_EVENT(EVENT_I2C_MSG);
			  tempC = (si7021_read_temp() * 175.72) / 65536 - 46.85;
			  if (tempC < LOW_TEMP_ALERT) {
				  led_on(LED1);
			  } else {
				  led_off(LED1);
			  }
			  unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM3
			  i2c_disable();            // Put I2C and sensor into low-power mode
			  si7021_poweroff();
		  }
	  }
	  sleep();
  }

}

