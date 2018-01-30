// File        : letimer.h
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LETIMER_H
#define __LETIMER_H

// Lowest energy mode the LE timer is allowed to use
// (e.g., when set to EM2, the LE timer won't go into EM3)
#define LETIMER_LOWEST_ENERGY_MODE EM2


#if LETIMER_LOWEST_ENERGY_MODE == EM3
#define LETIMER_OSC ULFRCO
#else
#define LETIMER_OSC LFXO
#endif

void letimer_init(void);

#endif /* __LETIMER_H */
