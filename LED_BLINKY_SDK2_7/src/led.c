// File        : led.c
// Description : Control of Gecko starter kit LEDs
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include "led.h"
#include "em_gpio.h"

void led_init(void) {

	gpio_init();

	// Set LED ports to be standard output drive with default off (cleared)
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);
}
