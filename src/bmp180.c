#include <util/delay.h>
#include "bmp180.h"
#include "i2c.h"

Bmp180CalibrationData *bmp180ReadCalibrationData() {
    Bmp180CalibrationData *data = malloc(sizeof(Bmp180CalibrationData));

    i2cInit();
    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        free(data);
        return NULL;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_WRITE);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED) {
        free(data);
        return NULL;
    }

    i2cSendData(0xAA);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        free(data);
        return NULL;
    }

    i2cStop();

    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        free(data);
        return NULL;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_READ);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_READ_ACK_SENT) {
        free(data);
        return NULL;
    }

    for (int reg = 0xAA; reg <= 0xBE; reg += 2) {
        uint16_t bits = 0;
        uint8_t msb = i2cReadDataAck();
        if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_SENT) {
            free(data);
            return NULL;
        }

        uint8_t lsb;
        if (reg == 0xBE) {
            lsb = i2cReadDataNotAck();
            if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_NOT_SENT) {
                free(data);
                return NULL;
            }
        } else {
            lsb = i2cReadDataAck();
            if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_SENT) {
                free(data);
                return NULL;
            }
        }

        bits |= (msb << 8) | lsb;

        switch (reg) {
            case 0xAA:
                data->ac1 = bits;
                break;
            case 0xAC:
                data->ac2 = bits;
                break;
            case 0xAE:
                data->ac3 = bits;
                break;
            case 0xB0:
                data->ac4 = bits;
                break;
            case 0xB2:
                data->ac5 = bits;
                break;
            case 0xB4:
                data->ac6 = bits;
                break;
            case 0xB6:
                data->b1 = bits;
                break;
            case 0xB8:
                data->b2 = bits;
                break;
            case 0xBA:
                data->mb = bits;
                break;
            case 0xBC:
                data->mc = bits;
                break;
            case 0xBE:
                data->md = bits;
                break;
        }
    }

    i2cStop();

    return data;
}

uint16_t bmp180ReadUncompensatedTemperature() {
    i2cInit();
    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_WRITE);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0xF4);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0x2E);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();
    _delay_us(4500);

    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_WRITE);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0xF6);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();
    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_READ);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_READ_ACK_SENT) {
        return BMP180_ERROR_RESULT;
    }

    uint8_t msb = i2cReadDataAck();
    if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_SENT) {
        return BMP180_ERROR_RESULT;
    }

    uint8_t lsb = i2cReadDataNotAck();
    if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_NOT_SENT) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();

    uint16_t result = 0;
    result |= (msb << 8) | lsb;

    return result;
}

float bmp180CalculateTemperature(uint16_t uncompensatedTemperature, int32_t *b5, Bmp180CalibrationData *calibrationData) {
    int32_t x1 = ((uncompensatedTemperature - calibrationData->ac6) * (int32_t) calibrationData->ac5) >> 15;
    int32_t x2 = ((int32_t) calibrationData->mc << 11) / (x1 + calibrationData->md);
    *b5 = x1 + x2;
    float temperature = (int32_t) ((*b5 + 8) >> 4);
    temperature /= (float) 10;

    return temperature;
}

uint32_t bmp180ReadUncompensatedPressure(uint8_t oss) {
    i2cInit();
    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_WRITE);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0xF4);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0x34 | (oss << 6));
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();

    switch (oss) {
        case BMP180_OSS_ULTRA_LOW_POWER:
            _delay_us(4500);
            break;
        case BMP180_OSS_STANDARD:
            _delay_us(7500);
            break;
        case BMP180_OSS_HIGH_RESOLUTION:
            _delay_us(13500);
            break;
        case BMP180_OSS_ULTRA_HIGH_RESOLUTION:
            _delay_us(25500);
            break;
    }

    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_WRITE);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendData(0xF6);
    if (i2cGetStatus() != I2C_STATUS_DATA_WRITE_ACK_RECEIVED) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();
    i2cStart();
    if (i2cGetStatus() != I2C_STATUS_START_TRANSMITTED) {
        return BMP180_ERROR_RESULT;
    }

    i2cSendSlaveAddress(BMP180_ADDRESS, I2C_READ);
    if (i2cGetStatus() != I2C_STATUS_ADDRESS_READ_ACK_SENT) {
        return BMP180_ERROR_RESULT;
    }

    uint8_t msb = i2cReadDataAck();
    if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_SENT) {
        return BMP180_ERROR_RESULT;
    }

    uint8_t lsb = i2cReadDataAck();
    if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_SENT) {
        return BMP180_ERROR_RESULT;
    }

    uint8_t xlsb = i2cReadDataNotAck();
    if (i2cGetStatus() != I2C_STATUS_DATA_READ_ACK_NOT_SENT) {
        return BMP180_ERROR_RESULT;
    }

    i2cStop();

    uint32_t result = 0;
    result |= (msb << 8) | lsb;
    result <<= 8;
    result |= xlsb;

    result >>= (8 - oss);

    return result;
}

long bmp180CalculatePressure(uint32_t uncompensatedPressure, int32_t *b5, uint8_t oss, Bmp180CalibrationData *calibrationData) {
    int32_t b6 = *b5 - 4000;
    int32_t x1 = ((int32_t) calibrationData->b2 * (b6 * b6 >> 12)) >> 11;
    int32_t x2 = (int32_t) calibrationData->ac2 * b6 >> 11;
    int32_t x3 = x1 + x2;
    int32_t b3 = ((((int32_t) calibrationData->ac1 * 4 + x3) << oss) + 2) >> 2;
    x1 = (int32_t) calibrationData->ac3 * b6 >> 13;
    x2 = ((int32_t) calibrationData->b1 * (b6 * b6 >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    uint32_t b4 = (uint32_t) calibrationData->ac4 * (x3 + 32768) >> 15;
    uint32_t b7 = ((uncompensatedPressure - b3) * (50000 >> oss));

    long p;
    if (b7 < 0x80000000) {
        p = ((b7 * 2) / b4);
    } else {
        p = ((b7 / b4) * 2);
    }

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p = p + ((x1 + x2 + 3791) >> 4);

    return p;
}

Bmp180Data *bmp180ReadData(uint8_t oss, Bmp180CalibrationData *calibrationData) {
    uint16_t ut = bmp180ReadUncompensatedTemperature();
    uint32_t up = bmp180ReadUncompensatedPressure(oss);

    Bmp180Data *data = malloc(sizeof(Bmp180Data));
    int32_t b5 = 0;

    if (ut != BMP180_ERROR_RESULT && ut != 0) {
        float temperature = bmp180CalculateTemperature(ut, &b5, calibrationData);
        data->temperatureC = temperature;
    } else {
        data->temperatureC = BMP180_ERROR_RESULT;
    }

    if (up != BMP180_ERROR_RESULT && up != 0 && ut != 0) {
        long pressure = bmp180CalculatePressure(up, &b5, oss, calibrationData);
        data->pressurePa = pressure;
    } else {
        data->pressurePa = BMP180_ERROR_RESULT;
    }

    return data;
}

int bmp180ConvertPressurePaToMmHg(long pressurePa) {
    return (int) (pressurePa * 0.00750062);
}