// File        : scheduler.c
// Description : Simple event-based scheduler
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include <stdint.h>

// Global variable to read/write events between ISRs and main loop
uint32_t event_flags = 0;

