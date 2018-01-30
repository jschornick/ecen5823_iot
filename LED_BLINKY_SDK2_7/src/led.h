// File        : led.h
// Description : Control of Gecko starter kit LEDs
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LED_H
#define __LED_H

#include "em_gpio.h"

#define LED_ON  1
#define LED_OFF 0

// LED0
#define	LED0_port    gpioPortF
#define LED0_pin     4
#define LED0_default LED_OFF

// LED1
#define LED1_port    gpioPortF
#define LED1_pin     5
#define LED1_default LED_OFF

#define led_on(led)   GPIO_PinOutSet(led##_port, led##_pin);
#define led_off(led)  GPIO_PinOutClear(led##_port, led##_pin);


void led_init(void);

#endif /* __LED_H */
