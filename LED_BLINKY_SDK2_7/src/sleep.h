// File        : sleep.h
// Description : Manage energy modes by sleeping as deeply as possible based on
//               peripheral and clock requirements
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3, C99 standard

// The energy modes we can use for sleeping.
typedef enum {
	EM1 = 1,
	EM2,
	EM3,
	EM4
} energy_mode_t;

// The highest value energy_mode enum, used for defining energy mode arrays.
#define ENERGY_MODE_MAX EM4

// Function : sleep()
//
// Sleep the MCU in the lowest energy mode allowed based on the energy mode
// registrations. Use block_sleep_mode() and unblock_sleep_mode() to affect
// the behavior of this routine.
void sleep(void);

// Function : blockSleepMode()
//
// Inhibit the system from attempting to sleep in the energy mode identified by 'em'.
//
// Param : em - The energy mode to register as unavailable for sleep
void block_sleep_mode(energy_mode_t em);

// Function : unblockSleepMode()
//
// Unregister a single energy mode inhibition identified by 'em'. If more than one
// peripheral has registered a block, the mode will stay inhibited until all
// blocks are released.
//
// Param : em - The energy mode block to release
void unblock_sleep_mode(energy_mode_t em);
