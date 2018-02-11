// File        : letimer.h
// Description : Low-energy timer (LETIMER) initialization and ISR
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef __LETIMER_H
#define __LETIMER_H

// Lowest energy mode the LE timer is allowed to use
// (e.g., when set to 2, the LE timer won't go into EM3)
#define LETIMER_LOWEST_ENERGY_MODE 3

#define LETIMER_MAX_TICKS   0xffff   // 16-bit timer

// Longest timer period we are allowed to schedule.
// Used to calculate the prescale value of the sleep timer.
#define LETIMER_MAX_PERIOD_MS    2000    // milliseconds

#if LETIMER_LOWEST_ENERGY_MODE == 3
  #define LETIMER_USES_ULFRCO 1
  #define LETIMER_CLK_FREQ  1000
#else
  #define LETIMER_CLK_FREQ  32768
#endif

#define LETIMER_MS_TO_TICKS_UNSCALED(ms) ((ms * LETIMER_CLK_FREQ) / 1000)

#define SOFT_TIMER_MAX 4

typedef enum {
	TIMER_OFF = 0,
	TIMER_ONCE,
	TIMER_REPEAT
} timer_mode_t;

typedef struct {
	timer_mode_t mode;
	uint16_t ticks;       // length of timer
	uint16_t expire_tick; // tick when timer goes off
	uint8_t event_flag;
} soft_timer_t;

extern soft_timer_t soft_timers[];


void letimer_init(void);
void update_timers(void);
void set_timer(uint8_t num, uint16_t ticks, uint8_t mode, uint32_t flag);

#endif /* __LETIMER_H */
