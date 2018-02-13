// File        : si7021.c
// Description : SI7021 I2C temperature/humidity sensor driver
// Author      : Jeff Schornick
// Toolchain   : GNU ARM v4.9.3

#include <stdint.h>

#include "si7021.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "gpio.h"
#include "i2c.h"

uint16_t temperature;

void si7021_init(void)
{
	gpio_init();
	GPIO_PinModeSet(SI7021_ENABLE_PORT, SI7021_ENABLE_PIN, gpioModePushPull, 0);
}

void si7021_poweron(void)
{
	// pullups will be enabled when sensor is connected via sensor_enable
	// sensor_enable gpio (PD9)
	GPIO_PinOutSet(SI7021_ENABLE_PORT, SI7021_ENABLE_PIN);
}

void si7021_poweroff(void)
{
	GPIO_PinOutClear(SI7021_ENABLE_PORT, SI7021_ENABLE_PIN);
}

I2C_TransferSeq_TypeDef i2c_seq;

void si7021_read_user_reg()
{
	uint8_t reg_data;

	// prime TX buf, wont' send until START
	I2C0->TXDATA = (SI7021_I2C_ADDR<<1) | I2C_WRITE_OP;
	I2C0->IFC = I2C_IF_ACK;
	I2C0->CMD = I2C_CMD_START;
	// wait for ACK
	// TODO: i2c_wait_ack_nack();
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;  // sleep here?
	I2C0->IFC = I2C_IF_ACK;

	// request register read
	I2C0->TXDATA = (SI7021_CMD_READ_USER_REG);
	// wait for ACK
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;
	I2C0->IFC = I2C_IF_ACK;

	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_I2C_ADDR<<1) | I2C_READ_OP;
	// wait for ACK
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;

	while ( !(I2C0->IF & I2C_IF_RXDATAV) ) {} ;
	reg_data = I2C0->RXDATA;  // auto-clears RXDATAV

	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;

	if( reg_data > 0 ) {
	}
}


void si7021_request_temp()
{
	// prime TX buf, wont' send until START
	I2C0->TXDATA = (SI7021_I2C_ADDR<<1) | I2C_WRITE_OP;
	I2C0->IFC = I2C_IF_ACK;
	I2C0->CMD = I2C_CMD_START;
	// wait for ACK
	// TODO: i2c_wait_ack_nack();
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;  // sleep here?
	I2C0->IFC = I2C_IF_ACK;

	//request temp measurement
	I2C0->TXDATA = (SI7021_CMD_MEASURE_TEMP_HOLD);
	// wait for ACK
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;
	I2C0->IFC = I2C_IF_ACK;

	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_I2C_ADDR<<1) | I2C_READ_OP;  // prime for read
	// wait for ACK
	while ( !(I2C0->IF & I2C_IF_ACK) ) {} ;

	I2C_IntEnable(I2C0, I2C_IEN_RXDATAV);
	// ~7ms wait during clock stretch, let interrupt handle it
	return;

}

uint16_t si7021_read_temp()
{
	uint8_t temp_msb;
	uint8_t temp_lsb;

	while ( !(I2C0->IF & I2C_IF_RXDATAV) ) {} ;
	temp_msb = I2C0->RXDATA;  // auto-clears RXDATAV

	// ack reception of first byte
	I2C0->CMD = I2C_CMD_ACK;

	while ( !(I2C0->IF & I2C_IF_RXDATAV) ) {} ;
	temp_lsb = I2C0->RXDATA;  // auto-clears RXDATAV

	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;

	temperature = (temp_msb << 8) + temp_lsb;

	return temperature;

}


