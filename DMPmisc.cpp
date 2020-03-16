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

    imu.setSampleRate(100); // Set sample rate to 100Hz

    // Use configureFifo to set which sensors should be stored
    // in the buffer.
    // Parameter to this function can be: INV_XYZ_GYRO,
    // INV_XYZ_ACCEL, INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
    imu.configureFifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    cout << "fifo" << imu.fifoAvailable() << std::endl;

    if (imu.fifoAvailable() >= 256)
    {
        // Then read while there is data in the FIFO
        while (imu.fifoAvailable() > 0)
        {
            // Call updateFifo to update ax, ay, az, gx, gy, and/or gz
            if (imu.updateFifo() == INV_SUCCESS)
        {
                printIMUData();
        }
        }
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
#if 1
    auto accelX = imu.calcAccel(imu.ax);
    auto accelY = imu.calcAccel(imu.ay);
    auto accelZ = imu.calcAccel(imu.az);
    auto gyroX = imu.calcGyro(imu.gx);
    auto gyroY = imu.calcGyro(imu.gy);
    auto gyroZ = imu.calcGyro(imu.gz);
    auto magX = imu.calcMag(imu.mx);
    auto magY = imu.calcMag(imu.my);
    auto magZ = imu.calcMag(imu.mz);
  

    //cout << "Accel: " << accelX << ", " << +accelY << ", " << accelZ << std::endl;
    cout << "Gyro: " << gyroX << ", " << +gyroY << ", " << gyroZ << std::endl;
//    cout << "Mag: " << magX << ", " << +magY << ", " << magZ << std::endl;
//    cout << "Time: " << imu.time << " ms" << std::endl;
#endif
#if 0
    auto q0 = imu.calcQuat(imu.qw);
    auto q1 = imu.calcQuat(imu.qx);
    auto q2 = imu.calcQuat(imu.qy);
    auto q3 = imu.calcQuat(imu.qz);

    cout << "Q: " << q0 << ", " << q1 << ", " << q2 << ", " << q3 << ", " << std::endl;
    cout << "R/P/Y: " << imu.roll << ", " << imu.pitch << ", " << imu.yaw << std::endl;
#endif
}
