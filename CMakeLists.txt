cmake_minimum_required(VERSION 3.9)
project(avr-bmp180 C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_RANLIB avr-ranlib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include(ExternalProject)
ExternalProject_Add(
        avr-i2c
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/i2c
        GIT_REPOSITORY https://github.com/radionoise/avr-i2c.git
#        GIT_TAG v1.0.0
        CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        INSTALL_COMMAND ""
)
include_directories(libs/i2c/include)

include_directories(include)
add_library(${PROJECT_NAME} src/bmp180.c include/bmp180.h)
add_dependencies(${PROJECT_NAME} avr-i2c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libs/i2c/lib/libavr-i2c.a)