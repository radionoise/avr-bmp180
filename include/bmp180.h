/*
 * BMP180 temperature and pressure sensor library for AVR.
 * Version 1.0.0
 */

#ifndef BMP180_BMP180_H
#define BMP180_BMP180_H

#include <stdint-gcc.h>
#include <stdlib.h>

#ifndef BMP180_ADDRESS
#define BMP180_ADDRESS 0x77
#endif

#define BMP180_OK 0
#define BMP180_ERROR_UT 1
#define BMP180_ERROR_UP 2

#define BMP180_OSS_ULTRA_LOW_POWER 0
#define BMP180_OSS_STANDARD 1
#define BMP180_OSS_HIGH_RESOLUTION 2
#define BMP180_OSS_ULTRA_HIGH_RESOLUTION 3

typedef struct Bmp180CalibrationData {
    int16_t ac1;
    int16_t ac2;
    int16_t ac3;
    uint16_t ac4;
    uint16_t ac5;
    uint16_t ac6;
    int16_t b1;
    int16_t b2;
    int16_t mb;
    int16_t mc;
    int16_t md;
} Bmp180CalibrationData;

typedef struct Bmp180Data {
    float temperatureC;
    long pressurePa;
} Bmp180Data;

/*
 * Reads sensor's calibration data. This need to be done once.
 */
uint8_t bmp180ReadCalibrationData(Bmp180CalibrationData *data);

/*
 * Reads sensor data (temperature and pressure).
 */
uint8_t bmp180ReadData(uint8_t oss, Bmp180Data *data, Bmp180CalibrationData *calibrationData);

/*
 * Converts Pa pressure to mmHg.
 */
float bmp180ConvertPressurePaToMmHg(long pressurePa);

/*
 * Calculates altitude based on pressure in Pa.
 */
int bmp180CalculateAltitudeMeters(long pressurePa);

#endif //BMP180_BMP180_H
