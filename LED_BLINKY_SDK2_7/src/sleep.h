// File        : sleep.h
// Description : Manage energy modes by sleeping as deeply as possible based on
//               peripheral and clock requirements
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3, C99 standard

#include <stdint.h>

typedef enum {
	EM1 = 1,
	EM2,
	EM3,
	EM4
} energy_mode_t;

#define ENERGY_MODE_MAX EM4

// The global sleep block counter, available for all to modify
extern uint8_t sleep_blocks[];

// Function : sleep()
//
// This function uses a global array
void sleep(void);


// Function : blockSleepMode()
void block_sleep_mode(energy_mode_t em);

// Function : unblockSleepMode()
void unblock_sleep_mode(energy_mode_t em);
