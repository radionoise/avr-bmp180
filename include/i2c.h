/*
 * I2C helper library for AVR.
 * Version 1.0.0
 */

#ifndef I2C_I2C_H
#define I2C_I2C_H

#include <avr/io.h>

#ifndef F_I2C
#warning "F_I2C is not defined. Using default frequency: 100kHz"
#define F_I2C 100000UL
#endif

#define TWBR_VALUE (((F_CPU) / (F_I2C) - 16) / 2)

#if ((TWBR_VALUE > 255) || (TWBR_VALUE == 0))
#error "Invalid TWBR value"
#endif

#define I2C_WRITE 0
#define I2C_READ 1

// Common status codes
#define I2C_STATUS_START_TRANSMITTED 0x8
#define I2C_STATUS_REPEATED_START_TRANSMITTED 0x10
#define I2C_STATUS_ARBITRATION_LOST 0x38

// Master transmitter status codes
#define I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED 0x18
#define I2C_STATUS_ADDRESS_WRITE_ACK_NOT_RECEIVED 0x20
#define I2C_STATUS_DATA_WRITE_ACK_RECEIVED 0x28
#define I2C_STATUS_DATA_WRITE_ACK_NOT_RECEIVED 0x30

// Master receiver status codes
#define I2C_STATUS_ADDRESS_READ_ACK_SENT 0x40
#define I2C_STATUS_ADDRESS_READ_ACK_NOT_SENT 0x48
#define I2C_STATUS_DATA_READ_ACK_SENT 0x50
#define I2C_STATUS_DATA_READ_ACK_NOT_SENT 0x58

void i2cInit();
void i2cStart();
void i2cStop();
void i2cSendSlaveAddress(uint8_t address, uint8_t readWriteMode);
void i2cSendData(uint8_t data);
uint8_t i2cReadDataAck();
uint8_t i2cReadDataNotAck();
uint8_t i2cGetStatus();

#endif //I2C_I2C_H