// File        : si7021.h
// Description : SI7021 I2C temperature/humidity sensor driver
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

// The enable port gates the sensor power input (VDD) as well as the I2C lines
// The pullups are removed from the MCU side of the bus when the sensor is disabled.

//
// sensor_enable gpio (PD9)

#ifndef _SI7021_H
#define _SI7021_H

#define SI7021_ENABLE_PORT  gpioPortD
#define SI7021_ENABLE_PIN   9

#define SI7021_I2C_ADDR   0x40

#define SI7021_USES_LPM   1

#define SI7021_CMD_MEASURE_TEMP_HOLD  0xE3  /* hold = clock stretch during measurement */
#define SI7021_CMD_READ_USER_REG      0xE7  /* reset value is 0011_1010 = 3A */
#define SI7021_CMD_WRITE_USER_REG     0xE6

void si7021_init(void);
void si7021_poweron(void);
void si7021_poweroff(void);
void si7021_request_temp();
void si7021_read_user_reg(void);


#endif /* _SI7021_H */
