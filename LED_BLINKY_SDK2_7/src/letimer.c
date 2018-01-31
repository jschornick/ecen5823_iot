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

    // The LETIMER_USES_ULFRCO macro will be set if the LETIMER is allowed
	// to go into EM3. Only the ULFRCO clock source is available in this mode.
	// Otherwise, stick to using the LFXO for increased precision.
#if LETIMER_USES_ULFRCO
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    // enable ultra low freq oscillator
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
#else
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      // enable LFXO, block until stable
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   // LFA branch uses LFXO
#endif

	// Calculate the theoretical number of timer ticks required if no prescaler is used.
	// This may not be acheivable since the LETIMER is only a 16-bit counter.
	uint32_t period_ticks = (BLINK_PERIOD_MS * LETIMER_CLK_FREQ) / 1000;
	uint32_t on_time_ticks = (BLINK_ON_TIME_MS * LETIMER_CLK_FREQ) / 1000;

	// Default prescale of 1 does not device the clock.
	CMU_ClkDiv_TypeDef prescale = 1;

	// Calculate required timer prescaler. Double the prescaler until the equivalent number
	// of unscaled ticks fits in a 16-bit integer.
	while ( period_ticks > 0xffff ) {
		// We're still larger than the maximum timer period, double the divider...
		prescale *= 2;
		period_ticks /= 2;
		on_time_ticks /= 2;
	}

	// Enabled the prescaled timer clock
	CMU_ClockDivSet(cmuClock_LETIMER0, prescale);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// Set the period and on time using timer comparators
	LETIMER_CompareSet(LETIMER0, 0, period_ticks-1);  // COMP0, blink period
	LETIMER_CompareSet(LETIMER0, 1, on_time_ticks-1); // COMP1, on time

	// Timer configuration, repeat and reload using COMP0
	const LETIMER_Init_TypeDef config = {
	  .enable         = false,               // Don't start counting after init
	  .debugRun       = false,               // Counter runs during debug halt
	  .comp0Top       = true,                // Load COMP0 into CNT on underflow
	  .bufTop         = false,               // Don't load COMP1 into COMP0 when REP0 reaches 0
	  .out0Pol        = 0,                   // Idle value for output 0
	  .out1Pol        = 0,                   // Idle value for output 1
	  .ufoa0          = letimerUFOANone,     // No action on underflow
	  .ufoa1          = letimerUFOANone,     // No action on underflow
	  .repMode        = letimerRepeatFree    // Free run, repeat until stopped
	};
	LETIMER_Init(LETIMER0, &config);

	// Interrupt on comparators
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0| LETIMER_IEN_COMP1);

	// Don't drop below configured energy mode
	block_sleep_mode(LETIMER_LOWEST_ENERGY_MODE+1);

	// Allow peripheral to interrtup the CPU
	NVIC_EnableIRQ(LETIMER0_IRQn);

	// Go!
	LETIMER_Enable(LETIMER0, true);
}

// Interrupt service routine for LETIMER0
//
// This ISR is responsible for blinking the LED on a strict schedule.
void LETIMER0_IRQHandler(void) {
	uint32_t intr_flags;  // interrupt flags

	// Atomics are not strictly necessary here. The ISR executes on a
	// deterministic schedule in a fraction of the time between interrupts.
	//CORE_ATOMIC_IRQ_DISABLE();

	// Record the pending interrupt flags and clear them to prevent an immediate
	// retriggering of the interrupt.
	intr_flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, intr_flags);

	// The timer resets to COMP0, and thus it defines the period of the blink.
	if ( intr_flags & LETIMER_IF_COMP0 ) {
		led_off(LED0);
	}
	// COMP1 defines the on time of the LED. It will remain on until the timer
	// underflows and resets the count to COMP0.
	if ( intr_flags & LETIMER_IFC_COMP1 ) {
		led_on(LED0);
	}

	//CORE_ATOMIC_IRQ_ENABLE();
}
