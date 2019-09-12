#include <avr/io.h>
#include "i2c.h"

void i2cInit() {
    TWBR = TWBR_VALUE;
    TWSR = 0;
}

void i2cWait() {
    while (!(TWCR & (1 << TWINT)));
}

void i2cStart() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    i2cWait();
}

void i2cStop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

    while ((TWCR & (1 << TWSTO)));
}

void i2cSendData(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2cWait();
}

uint8_t i2cReadDataAck() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    i2cWait();

    return TWDR;
}

uint8_t i2cReadDataNotAck() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2cWait();

    return TWDR;
}

void i2cSendSlaveAddress(uint8_t address, uint8_t readWriteMode) {
    i2cSendData((address << 1) | readWriteMode);
}

uint8_t i2cGetStatus() {
    return TWSR;
}