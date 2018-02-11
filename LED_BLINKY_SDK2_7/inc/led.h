// File        : led.h
// Description : Control of Gecko starter kit LEDs
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LED_H
#define __LED_H

#include "em_gpio.h"

// Onboard LEDs are driven on the high side, so use positive on/off logic.
#define LED_ON  1
#define LED_OFF 0

// LED0 on the starter kit board
#define	LED0_port    gpioPortF
#define LED0_pin     4
#define LED0_default LED_OFF

// LED1 on the starter kit board
#define LED1_port    gpioPortF
#define LED1_pin     5
#define LED1_default LED_OFF

// Convenience macros to set/clear are particular LED port/pin pair. Relies on
// LEDx_pin and LEDx_port being defined above.
#define led_on(led)   GPIO_PinOutSet(led##_port, led##_pin);
#define led_off(led)  GPIO_PinOutClear(led##_port, led##_pin);
#define led_toggle(led)  GPIO_PinOutToggle(led##_port, led##_pin);

// Function : led_init()
//
// Initialize the GPIOs which specifically drive the starter kit LEDs.
void led_init(void);

#endif /* __LED_H */
