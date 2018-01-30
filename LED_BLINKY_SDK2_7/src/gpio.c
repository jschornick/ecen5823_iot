// File        : gpio.c
// Description : GPIO initialization
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include "em_cmu.h"
#include "em_gpio.h"
#include "gpio.h"

//***********************************************************************************
// functions
//***********************************************************************************
void gpio_init(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
}
