// File        : sleep.c
// Description : Manage energy modes by sleeping as deeply as possible based on
//               peripheral and clock requirements
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include "em_emu.h"  // Use the EMU_EnterEMx routines for changing energy modes
#include "em_core.h"  // Atomic sections
#include "sleep.h"

// Define a counter for energy mode block requests.
// If an energy mode has block count > 0, it will not be entered during sleep.
// NB: sleep_blocks[0] exists to keep the indexing sane, but is never used
uint8_t sleep_blocks[ENERGY_MODE_MAX+1] = {0};


// Sleep in the deepest energy mode allowed.
// NB: the (restore = true) parameter in EM2 and EM3 is used  to restore HF
//     oscillators based on state prior to sleeping
void sleep(void) {

	int s = 0; // for debugger
	if ( sleep_blocks[EM1] ) {
		s = 0;
		// do nothing since we must stay in EM0
	} else if ( sleep_blocks[EM2] ) {
		s = 1;
		// Sleep CPU with high frequency clocks enabled,
		EMU_EnterEM1();
	} else if ( sleep_blocks[EM3] ) {
		s = 2;
		// Sleep CPU with only low frequency clocks enabled, ~3.3 uA
		EMU_EnterEM2(true);
	} else {
		s = 3;
		// Sleep at EM3 as the lowest, never go into EM4, ~2.8 uA
		EMU_EnterEM3(true);
	}

}

void block_sleep_mode(energy_mode_t em) {
	CORE_ATOMIC_SECTION( sleep_blocks[em]++; )
}

void unblock_sleep_mode(energy_mode_t em) {
	CORE_ATOMIC_SECTION(
		if(sleep_blocks[em]) {
			sleep_blocks[em]--;
		}
	)
}
