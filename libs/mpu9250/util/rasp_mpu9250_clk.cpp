/******************************************************************************
arduino_mpu9250_clk.c - MPU-9250 Digital Motion Processor Arduino Library 
Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

This library implements motion processing functions of Invensense's MPU-9250.
It is based on their Emedded MotionDriver 6.12 library.
	https://www.invensense.com/developers/software-downloads/

Development environment specifics:
Arduino IDE 1.6.12
SparkFun 9DoF Razor IMU M0

Supported Platforms:
- ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
******************************************************************************/
#include "arduino_mpu9250_clk.h"
#include <chrono>
#include <ratio>
#include <thread>

int arduino_get_clock_ms(unsigned long *count)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch());
    *count = int_ms.count();
	return 0;
}

int arduino_delay_ms(unsigned long num_ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(num_ms));
	return 0;
}
