/*
 * MPU6050misc.cpp
 *
 *  Created on: Dec 28, 2018
 *      Author: ominenko
 */
#include "DMPmisc.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <array>
using namespace std;
#include "SparkFunMPU9250-DMP.h"


MPU9250_DMP imu;
void printIMUData(void);
void dmp_main()
{
    if (imu.begin() != INV_SUCCESS)
    {
        while (1)
        {
            cout << "Unable to communicate with MPU-9250";
            cout << "Check connections, and try again." << std::endl;
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
  }

    // Use setSensors to turn on or off MPU-9250 sensors.
    // Any of the following defines can be combined:
    // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
    // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
    // Enable all sensors:
    imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

    // Use setGyroFSR() and setAccelFSR() to configure the
    // gyroscope and accelerometer full scale ranges.
    // Gyro options are +/- 250, 500, 1000, or 2000 dps
    imu.setGyroFSR(2000); // Set gyro to 2000 dps
    // Accel options are +/- 2, 4, 8, or 16 g
    imu.setAccelFSR(2); // Set accel to +/-2g
    // Note: the MPU-9250's magnetometer FSR is set at 
    // +/- 4912 uT (micro-tesla's)

    // setLPF() can be used to set the digital low-pass filter
    // of the accelerometer and gyroscope.
    // Can be any of the following: 188, 98, 42, 20, 10, 5
    // (values are in Hz).
    imu.setLPF(5); // Set LPF corner frequency to 5Hz

    // The sample rate of the accel/gyro can be set using
    // setSampleRate. Acceptable values range from 4Hz to 1kHz
    imu.setSampleRate(10); // Set sample rate to 10Hz

    // Likewise, the compass (magnetometer) sample rate can be
    // set using the setCompassSampleRate() function.
    // This value can range between: 1-100Hz
    imu.setCompassSampleRate(10); // Set mag rate to 10Hz

    // dataReady() checks to see if new accel/gyro data
    // is available. It will return a boolean true or false
    // (New magnetometer data cannot be checked, as the library
    //  runs that sensor in single-conversion mode.)
    while (1)

        if (imu.dataReady())
    {
        // Call update() to update the imu objects sensor data.
        // You can specify which sensors to update by combining
        // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
        // UPDATE_TEMPERATURE.
        // (The update function defaults to accel, gyro, compass,
        //  so you don't have to specify these values.)
        imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
        printIMUData();
  }
    cout << "done" << std::endl;
}

void printIMUData(void)
{
    // After calling update() the ax, ay, az, gx, gy, gz, mx,
    // my, mz, time, and/or temerature class variables are all
    // updated. Access them by placing the object. in front:

    // Use the calcAccel, calcGyro, and calcMag functions to
    // convert the raw sensor readings (signed 16-bit values)
    // to their respective units.
    float accelX = imu.calcAccel(imu.ax);
    float accelY = imu.calcAccel(imu.ay);
    float accelZ = imu.calcAccel(imu.az);
    float gyroX = imu.calcGyro(imu.gx);
    float gyroY = imu.calcGyro(imu.gy);
    float gyroZ = imu.calcGyro(imu.gz);
    float magX = imu.calcMag(imu.mx);
    float magY = imu.calcMag(imu.my);
    float magZ = imu.calcMag(imu.mz);
  

    cout << "Accel: " << accelX << ", " << +accelY << ", " << accelZ << std::endl;
    cout << "Gyro: " << gyroX << ", " << +gyroY << ", " << gyroZ << std::endl;
    cout << "Mag: " << magX << ", " << +magY << ", " << magZ << std::endl;
    cout << "Time: " << imu.time << " ms" << std::endl;
}
