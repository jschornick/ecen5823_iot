// File        : led.c
// Description : Control of Gecko starter kit LEDs
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include "led.h"
#include "gpio.h"
#include "em_gpio.h"

void led_init(void) {

	// GPIOs are used to drive the LEDs, make sure they are ready to use.
	gpio_init();

	// Use GPIOs in a push-pull configuration to act as direct led drivers.
	// Current is limited by a series resistor, so drive strength setting
	// is not critical.
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);
}
