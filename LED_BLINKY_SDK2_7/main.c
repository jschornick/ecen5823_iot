// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

// NOTE: The timer configuration parameters (minimum energy mode, period, on time) are all
//       defined in letimer.h

// Board and MCU initialization routines from Silicon Labs
#include "init_mcu.h"
#include "init_board.h"
#include "board_features.h"  // defines BGM121 (BRD4302A) features (via ble-configuration.h)
//#include "hal-config.h"      // defines HAL_I2CSENSOR_ENABLE
//#include "bsphalconfig.h"
//#include "em_device.h"  // pulls in EFR32BG1B232F256GM56 CMSIS definitions

#include "cmu.h"
#include "sleepmodes.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "si7021.h"
#include "letimer.h"

#include "scheduler.h"

// Overall lowerst energy mode. This may be raised by individual peripherals.
#define LOWEST_ENERGY_MODE 3

#define SENSOR_USES_LPM 1

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

  // setup gpio related to si7021, don't power on
  void si7021_init(void);


  // TODO: init_i2c

  set_timer(0, 2000, TIMER_REPEAT, EVENT_SAMPLE_WAKEUP);

  // The program is entirely event driven. Sleep immediately after interrupts are serviced.
  while (1) {
	  while( event_flags ) {
		  if (event_flags & EVENT_SAMPLE_WAKEUP) {
			  event_flags &= ~EVENT_SAMPLE_WAKEUP;
			  //led_toggle(LED0);
			  si7021_poweron();

              #if SENSOR_USES_LPM
			    set_timer(1, 80, TIMER_ONCE, EVENT_SENSOR_READY);
              #else
			    event_flags |= EVENT_SENSOR_READY;
			  #endif

		  }
		  if (event_flags & EVENT_SENSOR_READY) {
			  event_flags &= ~EVENT_SENSOR_READY;
			  // TODO: request temp over i2c
			  //led_on(LED1);
			  i2c_init();
			  //set_timer(2, 25, TIMER_ONCE, EVENT_I2C_MSG);
			  // send i2c request
			  led_on(LED0);
			  block_sleep_mode(EM2);  // need EM1 to clock I2C for response
			  //si7021_read_user_reg();
			  si7021_request_temp();
			  unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM3

		  }
		  if (event_flags & EVENT_I2C_MSG) {
			  event_flags &= ~EVENT_I2C_MSG;

			  // while (I2C_Status == i2cTransferInProgress) { sleep() };
			  unblock_sleep_mode(EM2);  // done reading I2C, sleep down to EM3
			  // read i2c response
			  // handle temp reading
			  #if SENSOR_USES_LPM
			    si7021_poweroff();
			  #endif
		  }
	  }
	  sleep();
  }

}

