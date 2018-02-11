// File        : letimer.c
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include <stdint.h>
#include "em_cmu.h"
#include "em_core.h"  // atomics
#include "em_letimer.h"
#include "led.h"   // LED definitions and output control
#include "sleepmodes.h"
#include "letimer.h"

#include "scheduler.h"

soft_timer_t soft_timers[SOFT_TIMER_MAX];
uint8_t running_timer = 0;

// set the lowest prescaler value that can count to ticks
CMU_ClkDiv_TypeDef letimer_calc_prescaler( uint32_t ticks )
{
	// Default prescale of 1 does not device the clock.
	CMU_ClkDiv_TypeDef prescale = 1;

	// Calculate required timer prescaler. Double the prescaler until the equivalent number
	// of unscaled ticks fits in a 16-bit integer.
	while ( ticks > LETIMER_MAX_TICKS ) {
		// We're still larger than the maximum timer period, double the divider...
		prescale *= 2;
		ticks /= 2;
	}
	return prescale;
}

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
	// This may not be achievable since the LETIMER is only a 16-bit counter.
	uint32_t unscaled_ticks = LETIMER_MS_TO_TICKS_UNSCALED( LETIMER_MAX_PERIOD_MS );
	uint16_t period_ticks;
	CMU_ClkDiv_TypeDef prescale;

	// Enabled the prescaled timer clock
	prescale = letimer_calc_prescaler( unscaled_ticks );
	CMU_ClockDivSet(cmuClock_LETIMER0, prescale);

	period_ticks = unscaled_ticks / prescale;

	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	// Set the primary period and on time using timer comparators
	LETIMER_CompareSet(LETIMER0, 0, period_ticks-1);  // COMP0, defines main wakeup loop period

	// Timer configuration, repeat and reload using COMP0
	const LETIMER_Init_TypeDef config = {
	  .enable         = false,               // Don't start counting after init
	  .debugRun       = false,               // Counter runs during debug halt
	  .comp0Top       = false,               // Don't load COMP0 into CNT on underflow
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

	// Don't drop below configured energy mode
	block_sleep_mode(LETIMER_LOWEST_ENERGY_MODE+1);

	// Allow peripheral to interrtup the CPU
	NVIC_EnableIRQ(LETIMER0_IRQn);

	// Go!
	LETIMER_Enable(LETIMER0, true);
}


void set_timer(uint8_t num, uint16_t ms, uint8_t mode, uint32_t flag)
{
	// atomic?
	uint16_t ticks = LETIMER_MS_TO_TICKS_UNSCALED(ms) / CMU_ClockDivGet(cmuClock_LETIMER0);
	soft_timers[num] = (soft_timer_t) {
			  .mode = mode,
			  .ticks = ticks,
			  .expire_tick = LETIMER0->CNT - ticks,  // 16-bit wrap
			  .event_flag = flag
	};
	update_timers();
}

void update_timers(void)
{
	uint16_t now = LETIMER0->CNT;
	soft_timer_t *timer;
	uint16_t least_ticks;
	uint16_t diff_ticks;
	uint16_t new_expire;
	uint16_t found = 0;
	for( uint8_t idx=0; idx<SOFT_TIMER_MAX; idx++) {
		timer = &soft_timers[idx];
		if ( timer->mode == TIMER_OFF ) { continue; }
		diff_ticks = now - timer->expire_tick;  // should wrap if expire < now
		if (!found ) {
			least_ticks = diff_ticks;
			new_expire = timer->expire_tick;
			found = 1;
		} else {
			if (diff_ticks < least_ticks) {
				least_ticks = diff_ticks;
				new_expire = timer->expire_tick;
			}
		}
	}
	if(found) {
		LETIMER_CompareSet(LETIMER0, 0, new_expire );
		LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0);

	} else {
		LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP0);
	}
}

// Interrupt service routine for LETIMER0
//
// This ISR is responsible for blinking the LED on a strict schedule.
void LETIMER0_IRQHandler(void)
{
	uint32_t intr_flags;  // interrupt flags

	// Atomics are not strictly necessary here. The ISR executes on a
	// deterministic schedule in a fraction of the time between interrupts.
	//CORE_ATOMIC_IRQ_DISABLE();

	//uint16_t now = LETIMER0->CNT;

	// Record the pending interrupt flags and clear them to prevent an immediate
	// retriggering of the interrupt.
	intr_flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, intr_flags);

	if ( intr_flags & LETIMER_IF_COMP0 ) {

		soft_timer_t *timer;

		// handle expiring timers
		for( uint8_t idx=0; idx<SOFT_TIMER_MAX; idx++) {
			timer = &soft_timers[idx];
			if( timer->expire_tick == LETIMER0->COMP0 ) {
				switch ( timer->mode ) {
					case TIMER_ONCE:
						timer->mode = TIMER_OFF;
						event_flags |= timer->event_flag;
						break;
					case TIMER_REPEAT:
						timer->expire_tick = LETIMER0->COMP0 - timer->ticks;  // should wrap at 16 bits?
						event_flags |= timer->event_flag;
						break;
					case TIMER_OFF:
						break;
				}
			}
		}

		update_timers();
	}

	//CORE_ATOMIC_IRQ_ENABLE();
}
