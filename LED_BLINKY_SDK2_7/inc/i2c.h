// File        : i2c.h
// Description : I2C peripheral
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#ifndef _I2C_H
#define _I2C_H

// NOTE: monitor i2C on expansion header:
//      exp-15 : SCL (PC10)
//      exp-16 : SDA (PC11)

// I2C bus connection
// From Starter Kit User Guide, pg 8-9, 18
//    SCL on PC10
//    SDA on PC11

// Pin routing
// See BGM121 datasheet pages 60-61, 64, and 67
//   I2C0_SCL on PC10 -> location 14
//   I2C0_SDA on PC11 -> location 16

#define I2C_READ_OP   1
#define I2C_WRITE_OP  0

#define I2C_SCL_PORT  gpioPortC
#define I2C_SCL_PIN   10
#define I2C_SCL_LOC   14

#define I2C_SDA_PORT  gpioPortC
#define I2C_SDA_PIN   11
#define I2C_SDA_LOC   16

void i2c_init(void);
void i2c_disable(void);

#endif /* _I2C_H */
