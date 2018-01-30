// File        : letimer.h
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LETIMER_H
#define __LETIMER_H

// Lowest energy mode the LE timer is allowed to use
// (e.g., when set to 2, the LE timer won't go into EM3)
#define LETIMER_LOWEST_ENERGY_MODE 2


#if LETIMER_LOWEST_ENERGY_MODE == 3
  #define LETIMER_USES_ULFRCO 1
  #define LETIMER_FREQ  1000
#else
  #define LETIMER_FREQ  32768
#endif

void letimer_init(void);

#endif /* __LETIMER_H */
