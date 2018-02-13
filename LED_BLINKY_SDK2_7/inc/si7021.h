// File        : si7021.h
// Description : SI7021 I2C temperature/humidity sensor driver
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

// The enable port gates the sensor power input (VDD) as well as the I2C lines
// The pullups are removed from the MCU side of the bus when the sensor is disabled.

#ifndef _SI7021_H
#define _SI7021_H

#include <stdint.h>

// Si7021 SENSOR_ENABLE pin connected to PD9 on MCU
#define SI7021_ENABLE_PORT  gpioPortD
#define SI7021_ENABLE_PIN   9

#define SI7021_I2C_ADDR   0x40

#define SI7021_CMD_MEASURE_TEMP_HOLD  0xE3  /* hold = clock stretch during measurement */
#define SI7021_CMD_READ_USER_REG      0xE7  /* reset value is 0011_1010 = 3A */
#define SI7021_CMD_WRITE_USER_REG     0xE6

#define SI7021_WARMUP_MS   80

// Function: si7102_init
//
// One time setup for the Si7021 temperature sensor.
// Does not power on the device.
void si7021_init(void);

// Function: si7021_poweron
//
// Enables the Si7021 via the SENSOR_ENABLE pin
// A warmup period is required after poweron.
// Should be called after init or poweroff
void si7021_poweron(void);

// Function: si7021 poweroff
//
// Disables the Si7021 via the SENSOR_ENABLE pin.
// Reenable via a call to si7021_poweron().
void si7021_poweroff(void);

// Function: si7021_request_temp
//
// Initiates a temperature measurement over I2C
// After this function returns, the I2C bus will be in clock stretch mode until
// the conversion completes and the sensor allows SCL to return high (~7 ms).
void si7021_request_temp();


// Function: si7021_read_user_req
//
// Reads the si7021 user register via I2C
void si7021_read_user_reg(void);

// Function: si7021_read_temp
//
// Reads the result of the previously initiated temperature measurement.
// Should be called after calling si7021_request_temp().
// Returns the unconverted 16-bit tempcode.
uint16_t si7021_read_temp(void);


#endif /* _SI7021_H */
