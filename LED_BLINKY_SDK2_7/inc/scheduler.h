// File        : scheduler.h
// Description : Simple scheduler
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#define EVENT_SAMPLE_WAKEUP 0x1
#define EVENT_SENSOR_READY  0x2
#define EVENT_I2C_MSG       0x4

#define CHECK_EVENT(evt) (event_flags & (evt))
#define CLEAR_EVENT(evt) (event_flags &= ~(evt))

extern uint32_t event_flags;

#endif /* _SCHEDULER_H */
