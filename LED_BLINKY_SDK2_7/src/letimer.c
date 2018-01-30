// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include <stdint.h>
#include "em_cmu.h"
#include "em_core.h"  // atomics
#include "em_letimer.h"
#include "led.h"   // LED definitions and output control
#include "src/sleep.h"
#include "letimer.h"


void letimer_init(void) {

	// TODO: set oscillator based on config macro
	//   EM2 : LXFO   = 32678 Hz
	//   EM3 : ULFRCO = 10000 Hz
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);  // enable LFXO, block until stable
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);  // LFA branch uses LFXO

	// TODO: set divisor based on period
	CMU_ClockDivSet(cmuClock_LFA, 0);

	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// TODO: set based on timer sleep macro
	block_sleep_mode(LETIMER_LOWEST_ENERGY_MODE+1);


	// COMP0
	// COMP1
	// IEN
	// NVIC

	// LETIMER0->CMD = LETIMER_CMD_START

}

// Interrupt service routine for LETIMER0
void LETIMER0_IRQHandler(void) {
	uint32_t intr_flags;  // interrupt flags

	CORE_ATOMIC_IRQ_DISABLE();

	intr_flags = LETIMER_IntGet(LETIMER0); // record pending interrupts
	LETIMER_IntClear(LETIMER0, intr_flags);

	if ( intr_flags & LETIMER_IF_COMP0 ) {
		led_off(LED0);
	}
	if ( intr_flags & LETIMER_IFC_COMP1 ) {
		led_on(LED0);
	}

	CORE_ATOMIC_IRQ_ENABLE();
}
