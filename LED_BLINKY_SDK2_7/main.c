// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

// NOTE: The timer configuration parameters (minimum energy mode, period, on time) are all
//       defined in letimer.h

// Board and MCU initialization routines from Silicon Labs
#include "init_mcu.h"
#include "init_board.h"

#include "cmu.h"
#include "sleepmodes.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "letimer.h"

#include "scheduler.h"

// Overall lowerst energy mode. This may be raised by individual peripherals.
#define LOWEST_ENERGY_MODE 3

int main(void)
{

  // Device and board initialization
  initMcu();
  initBoard();

  // Initialize stack and BGAPI using the example configuration from Silicon Labs
  //gecko_init(&config);

  // Create an overall block to force the MCU to stay in a certain energy mode.
  block_sleep_mode(LOWEST_ENERGY_MODE+1);

  /* Initialize LED via corresponding GPIOs */
  led_init();

  led_off(LED0);
  led_off(LED1);

  letimer_init();

  set_timer(0, 2000, TIMER_REPEAT, EVENT_SAMPLE_WAKEUP);
  set_timer(1, 1000, TIMER_REPEAT, EVENT_SENSOR_READY);

  // The program is entirely event driven. Sleep immediately after interrupts are serviced.
  while (1) {
	  while( event_flags ) {
		  if (event_flags & EVENT_SAMPLE_WAKEUP) {
			  event_flags &= ~EVENT_SAMPLE_WAKEUP;
			  led_toggle(LED0);
			 // set_timer(1, 250, TIMER_ONCE, EVENT_SENSOR_READY);

			  // power on timer (if using LPM)
			  // schedule warmup timer (if using LPM)
			  //   else set EVENT_SENSOR_READY
		  }
		  if (event_flags & EVENT_SENSOR_READY) {
			  event_flags &= ~EVENT_SENSOR_READY;
			  // request temp over i2c
			  led_on(LED1);
			  set_timer(2, 250, TIMER_ONCE, EVENT_I2C_MSG);
			  block_sleep_mode(EM2);  // need EM1 to clock I2C for response
		  }
		  if (event_flags & EVENT_I2C_MSG) {
			  event_flags &= ~EVENT_I2C_MSG;
			  led_off(LED1)
			  unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM3
			  // read i2c response
			  // handle temp reading
		  }
	  }
	  sleep();
  }

}

