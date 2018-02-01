// File        : letimer.h
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LETIMER_H
#define __LETIMER_H

// Lowest energy mode the LE timer is allowed to use
// (e.g., when set to 2, the LE timer won't go into EM3)
#define LETIMER_LOWEST_ENERGY_MODE 3


// LED blink timing for Energy Modes assignment
#define BLINK_PERIOD    3.0    // 3.0 seconds
#define BLINK_ON_TIME   0.075  // 75 milliseconds

#define BLINK_PERIOD_MS ( (uint32_t) (BLINK_PERIOD * 1000) )
#define BLINK_ON_TIME_MS ( (uint32_t) (BLINK_ON_TIME * 1000) )

#if LETIMER_LOWEST_ENERGY_MODE == 3
  #define LETIMER_USES_ULFRCO 1
  #define LETIMER_CLK_FREQ  1000
#else
  #define LETIMER_CLK_FREQ  32768
#endif

void letimer_init(void);

#endif /* __LETIMER_H */
