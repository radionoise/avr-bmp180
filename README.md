# BMP180 temperature/pressure sensor library for AVR microcontroller

### Overview
This library was tested on ATmega 16 @ 8 MHz.

### Usage
You have to read sensor's individual calibration data and pass it every time when fetching readings.

### Example
```
Bmp180CalibrationData calibrationData;
uint8_t result = mp180ReadCalibrationData(&calibrationData);

if (result == BMP180_OK) {
    Bmp180Data bmp180Data;
    result = bmp180ReadData(BMP180_OSS_STANDARD, &bmp180Data, &calibrationData);

    if (result == BMP180_OK) {
        float temperature = bmp180Data.temperatureC;
        long pressure = bmp180Data.pressurePa;
    }
}
```