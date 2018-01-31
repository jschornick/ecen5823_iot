// File        : gpio.c
// Description : GPIO initialization
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include "em_cmu.h"
#include "em_gpio.h"
#include "gpio.h"

void gpio_init(void){
	// Generic configuration for all GPIOs. The entire GPIO subsystem
	// runs off of a single clock source.
	CMU_ClockEnable(cmuClock_GPIO, true);
}
