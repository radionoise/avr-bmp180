#ifndef WEATHER_STATION_BMP180_H
#define WEATHER_STATION_BMP180_H

#include <stdint-gcc.h>
#include <stdlib.h>

#ifndef BMP180_ADDRESS
#define BMP180_ADDRESS 0x77
#endif

#define BMP180_ERROR_RESULT 0xFFFF

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

Bmp180CalibrationData *bmp180ReadCalibrationData();
Bmp180Data *bmp180ReadData(uint8_t oss, Bmp180CalibrationData *calibrationData);
float bmp180ConvertPressurePaToMmHg(long pressurePa);
int bmp180CalculateAltitudeMeters(long pressurePa);

#endif //WEATHER_STATION_BMP180_H
