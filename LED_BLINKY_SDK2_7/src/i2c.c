// File        : i2c.c
// Description : I2C peripheral enable/disable routines
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include <stdint.h>

#include "em_core.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"

#include "gpio.h"
#include "i2c.h"
#include "scheduler.h"  // event_flags
#include "si7021.h"

void i2c_init(void)
{
	// Enable the high speed peripheral clock tree and the clock of the actual I2C peripheral
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C0, true);

	// GPIOs are used to drive the I2C lines, make sure they are ready to use.
	gpio_init();

	// See i2c.h for port/pin definitions
	// gpioModeWiredAnd == open drain (0 will pull to GND, requires external pull-up)
	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAnd, 1);

	// GPIOs are configured, and pullups from sensor are connected
	//   send I2C slave reset pattern on SCL
	for (uint8_t cnt = 0; cnt < 9; cnt++) {
	    GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
	    GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
	}

	I2C0->ROUTEPEN  = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
	I2C0->ROUTELOC0 = (I2C_SDA_LOC << _I2C_ROUTELOC0_SDALOC_SHIFT)
	                  | (I2C_SCL_LOC << _I2C_ROUTELOC0_SCLLOC_SHIFT);

	// use the emlib init routine with default config
	const I2C_Init_TypeDef config = I2C_INIT_DEFAULT;
	I2C_Init(I2C0, &config);

	// The bus will be in the "busy" state when coming out of reset
	// Use the abort command to reset the state machine
	if (I2C0->STATE & I2C_STATE_BUSY) {
		I2C0->CMD = I2C_CMD_ABORT;
	}

	// Start with interrupts disabled, we'll enable RXDATAV on demand
	I2C0->IEN = 0;

	// Allow peripheral to interrupt the CPU
	NVIC_ClearPendingIRQ(I2C0_IRQn);
	NVIC_EnableIRQ(I2C0_IRQn);
}

void i2c_disable(void)
{
	// OK to disable I2C lines since the entire bus will be powered down
	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeDisabled, 0);
	// I2C is the only one using the HF periph clock, so disable it too
	CMU_ClockEnable(cmuClock_HFPER, false);
	CMU_ClockEnable(cmuClock_I2C0, false);
}

void I2C0_IRQHandler(void) {

	// disable nested interrupts instead?
	CORE_ATOMIC_IRQ_DISABLE();

	uint32_t intr_flags;  // interrupt flags

	// Record the pending interrupt flags and clear them to prevent an immediate
	// retriggering of the interrupt.
	intr_flags = I2C_IntGet(I2C0);

	// RXDATAV interrupt:
	// See reference manual page 470
	//	 Receive Data Valid Interrupt Flag
	//   Set when data is available in the receive buffer. Cleared automatically when the receive buffer is read.
	if ( intr_flags & I2C_IF_RXDATAV ) {
		// Disable RX interrupt so we don't rerun the ISR until reading response in main loop
		I2C_IntDisable(I2C0, I2C_IEN_RXDATAV);
		event_flags |= EVENT_I2C_MSG;
	}

	CORE_ATOMIC_IRQ_ENABLE();
}
