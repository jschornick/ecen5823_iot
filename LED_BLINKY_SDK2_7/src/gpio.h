// File        : gpio.c
// Description : GPIO initialization
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __GPIO_H
#define __GPIO_H

// Function : gpio_init()
//
// Configure the entire GPIO subsystem. Individual port/pin setup is handled by
// the specific peripherals which use those particular GPIOs.
void gpio_init(void);

#endif /* __GPIO_H */
