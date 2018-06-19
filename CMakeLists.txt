cmake_minimum_required(VERSION 3.9)
project(avr-bmp180 C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_RANLIB avr-ranlib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(include)
add_library(${PROJECT_NAME} src/bmp180.c include/bmp180.h)