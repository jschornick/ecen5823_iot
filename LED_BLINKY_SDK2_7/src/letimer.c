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

#if LETIMER_USES_ULFRCO
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    // enable ultra low freq oscillator
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
#else
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      // enable LFXO, block until stable
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   // LFA branch uses LFXO
#endif

	// comp0 = ((period  * freq)/1000) / div
	// comp1 = ((on_time * freq)/1000) / div

	// div = 2^prescale

	uint32_t period_ticks = (BLINK_PERIOD_MS * LETIMER_CLK_FREQ) / 1000;
	uint32_t on_time_ticks = (BLINK_ON_TIME_MS * LETIMER_CLK_FREQ) / 1000;

	CMU_ClkDiv_TypeDef prescale = 1;

	// calculate timer prescaler
	while ( period_ticks > 0xffff ) {
		// we're larger than the maximum timer period, so up the dividers
		period_ticks /= 2;
		on_time_ticks /= 2;
		prescale *= 2;
	}

	// TODO: set divisor based on period
	//CMU_ClockDivSet(cmuClock_LFA, prescale);
	CMU_ClockDivSet(cmuClock_LETIMER0, prescale);


	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// TODO: set based on configured frequency, target period, and target on time
	//LETIMER0->COMP0 = 0xdead;
	LETIMER_CompareSet(LETIMER0, 0, period_ticks);  // COMP0, defines period of timer
	LETIMER_CompareSet(LETIMER0, 1, on_time_ticks);  //

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

	// apply the configuration to LETIMER0
	LETIMER_Init(LETIMER0, &config);


	// IEN
	LETIMER_IntClear(LETIMER0, LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0| LETIMER_IEN_COMP1);

	block_sleep_mode(LETIMER_LOWEST_ENERGY_MODE+1);

	// NVIC
	NVIC_EnableIRQ(LETIMER0_IRQn);

	LETIMER_Enable(LETIMER0, true);  // emlib command that calls: LETIMER0->CMD = LETIMER_CMD_START

}

// Interrupt service routine for LETIMER0
void LETIMER0_IRQHandler(void) {
	uint32_t intr_flags;  // interrupt flags

	// Not strictly necessary since we execute in a fraction of the time between interrupts
	//CORE_ATOMIC_IRQ_DISABLE();

	intr_flags = LETIMER_IntGet(LETIMER0); // record pending interrupts
	LETIMER_IntClear(LETIMER0, intr_flags);

	if ( intr_flags & LETIMER_IF_COMP0 ) {
		led_off(LED0);
	}
	if ( intr_flags & LETIMER_IFC_COMP1 ) {
		led_on(LED0);
	}

	//CORE_ATOMIC_IRQ_ENABLE();
}
