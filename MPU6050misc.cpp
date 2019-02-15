/*
 * MPU6050misc.cpp
 *
 *  Created on: Dec 28, 2018
 *      Author: ominenko
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include "MPU6050misc.h"
using namespace std;

void MPU6050_setoffs(MPU6050 &mpu) {
  mpu.setXAccelOffset(-2466);
  mpu.setYAccelOffset(-447);
  mpu.setZAccelOffset(1965);
  mpu.setXGyroOffset(-6);
  mpu.setYGyroOffset(63);
  mpu.setZGyroOffset(8);
}


void MPU6050_main() {
  cout << "MPU6050 3-axis acceleromter example program" << endl;
  MPU6050 accelgyro;
  accelgyro.setup();
  accelgyro.initialize();

  MPU6050_setoffs(accelgyro);
  cout << "ID=" << static_cast<int>(accelgyro.getDeviceID()) << endl;
  if (accelgyro.testConnection())
    cout << "MPU6050 connection test successful" << endl;
  else {
    cerr << "MPU6050 connection test failed! something maybe wrong, continuing anyway though ..." << endl;
    return;
  }

  cout << endl;
  cout << "  ax \t ay \t az \t gx \t gy \t gz" << endl;


  while (true) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
  }
}

void MPU6050_misc::meansensors() {
  int64_t buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;
  auto i = buffersize;

  while (i--) {
    // read raw accel/gyro measurements from device
    getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    buff_ax += ax;
    buff_ay += ay;
    buff_az += az;
    buff_gx += gx;
    buff_gy += gy;
    buff_gz += gz;
    this_thread::sleep_for(chrono::milliseconds(5));
  }

  mean_ax = buff_ax / buffersize;
  mean_ay = buff_ay / buffersize;
  mean_az = buff_az / buffersize;
  mean_gx = buff_gx / buffersize;
  mean_gy = buff_gy / buffersize;
  mean_gz = buff_gz / buffersize;

  cout << "mean_ax=" << mean_ax << "\t mean_ay=" << mean_ay << "\t mean_az=" << mean_az
      << "\t mean_gx=" << mean_gx << "\t mean_gy=" << mean_gy << "\t mean_gz=" << mean_gz << endl;
}

void MPU6050_misc::calibration() {
  ax_offset = -mean_ax / 8;
  ay_offset = -mean_ay / 8;
  az_offset = (16384 - mean_az) / 8;

  gx_offset = -mean_gx / 4;
  gy_offset = -mean_gy / 4;
  gz_offset = -mean_gz / 4;
  while (1) {
    int ready = 0;
    setXAccelOffset(ax_offset);
    setYAccelOffset(ay_offset);
    setZAccelOffset(az_offset);

    setXGyroOffset(gx_offset);
    setYGyroOffset(gy_offset);
    setZGyroOffset(gz_offset);

    meansensors();
    cout << "..." << endl;

    if (abs(mean_ax) <= acel_deadzone)
      ready++;
    else
      ax_offset = ax_offset - mean_ax / acel_deadzone;

    if (abs(mean_ay) <= acel_deadzone)
      ready++;
    else
      ay_offset = ay_offset - mean_ay / acel_deadzone;

    if (abs(16384 - mean_az) <= acel_deadzone)
      ready++;
    else
      az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

    if (abs(mean_gx) <= giro_deadzone)
      ready++;
    else
      gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

    if (abs(mean_gy) <= giro_deadzone)
      ready++;
    else
      gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

    if (abs(mean_gz) <= giro_deadzone)
      ready++;
    else
      gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

    if (ready == 6)
      break;
  }
}

void MPU6050_misc::calculate_offs() {
  cout << "MPU6050 calibration" << endl;
  setup();
  initialize();
  cout << "\nYour MPU6050 should be placed in horizontal position, with package letters facing up. \nDon't touch it until you see a finish message." << endl;
  if (testConnection())
    cout << "MPU6050 connection test successful" << endl;
  else {
    cerr << "MPU6050 connection test failed! something maybe wrong, continuing anyway though ..." << endl;
    return;
  }
  cout << "initial val" << endl;
  cout << "AX=" << getXAccelOffset() << "\t AY=" << getYAccelOffset() << "\t AZ=" << getZAccelOffset() << endl;
  cout << "GX=" << getXGyroOffset() << "\t GY=" << getYGyroOffset() << "\t GZ=" << getZGyroOffset() << endl;
  cout << "wait.." << endl;
  this_thread::sleep_for(chrono::seconds(15));
  cout << "reset offsets" << endl;
  setXAccelOffset(0);
  setYAccelOffset(0);
  setZAccelOffset(0);
  setXGyroOffset(0);
  setYGyroOffset(0);
  setZGyroOffset(0);
  cout << "MPU6050 meansensors" << endl;
  meansensors();
  cout << "MPU6050 Calculating offsets..." << endl;
  calibration();
  meansensors();
  cout << "\nFINISHED!" << endl;
  cout << "\nSensor readings with offsets:" << endl;

  cout << "Your offsets:" << endl;

  cout << "ax_offset=" << ax_offset << endl;
  cout << "ay_offset=" << ay_offset << endl;
  cout << "az_offset=" << az_offset << endl;

  cout << "gx_offset=" << gx_offset << endl;
  cout << "gy_offset=" << gy_offset << endl;
  cout << "gz_offset=" << gz_offset << endl;

  cout << "Check that your sensor readings are close to 0 0 16384 0 0 0" << endl;
}

void MPU6050_calibrate() {
  MPU6050_misc misc;
  misc.calculate_offs();
}

bool MPU6050_DMP::init() {
  setup();
  initialize();
  if (dmpInitialize()) {
    return false;;
  }
  MPU6050_setoffs(*this);
  inited_ = true;
  return true;
}

void MPU6050_DMP::main() {
  if (!inited_) {
    cerr << "dmp not started" << endl;
    return;
  }
  cout << "Enabling DMP..." << endl;
  setDMPEnabled(true);
  // get expected DMP packet size for later comparison
  const auto packetSize = dmpGetFIFOPacketSize();
  cout << "FIFO packetSize=" << packetSize << endl;
  while (1) {
    //wait data
    this_thread::sleep_for(chrono::milliseconds(1));
    auto fifoCount = getFIFOCount();
    auto mpuIntStatus = getIntStatus();
    //cout << "FIFO fifoCount!" << fifoCount << endl;

    if (mpuIntStatus & (1 << MPU6050_INTERRUPT_FIFO_OFLOW_BIT) || fifoCount >= 1024) {
      // reset so we can continue cleanly
      resetFIFO();
      cerr << "FIFO overflow!";
      cerr << "fifoCount=" << fifoCount << endl;
      continue;
    }
    while (fifoCount >= packetSize) {
      uint8_t fifoBuffer[packetSize];
      getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
      processDate(fifoBuffer);
    }
  }
}

class MPU6050_DMP_test: public MPU6050_DMP {
  void processDate(const uint8_t *fifoBuffer) {
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float euler[3];         // [psi, theta, phi]    Euler angle container
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

#if 0// OUTPUT_READABLE_QUATERNION

    // display quaternion values in easy matrix form: w x y z
    dmpGetQuaternion(&q, fifoBuffer);
    cout << "quat\t" << q.w << "\t" << q.x << "\t" << q.y << "\t" << q.z << endl;
#endif

#if 0//  OUTPUT_READABLE_EULER
    // display Euler angles in degrees
    dmpGetQuaternion(&q, fifoBuffer);
    dmpGetEuler(euler, &q);
    cout << "euler\t" << euler[0] * 180 / M_PI << "\t" << euler[1] * 180 / M_PI << "\t" << euler[2] * 180 / M_PI << endl;
#endif

#if 0 // OUTPUT_READABLE_YAWPITCHROLL
    // display Euler angles in degrees
    dmpGetQuaternion(&q, fifoBuffer);
    dmpGetGravity(&gravity, &q);
    dmpGetYawPitchRoll(ypr, &q, &gravity);
    cout << "ypr\t" << ypr[0] * 180 / M_PI
    << "\t" << ypr[1] * 180 / M_PI
    << "\t" << ypr[2] * 180 / M_PI << endl;
#endif

#if 1 // OUTPUT_READABLE_REALACCEL
    // display real acceleration, adjusted to remove gravity
    dmpGetQuaternion(&q, fifoBuffer);
    dmpGetAccel(&aa, fifoBuffer);
    dmpGetGravity(&gravity, &q);
    dmpGetLinearAccel(&aaReal, &aa, &gravity);
    cout << "areal\t" << aaReal.x << "\t" << aaReal.y << "\t" << aaReal.z << endl;
#endif

#if 0// OUTPUT_READABLE_WORLDACCEL
    // display initial world-frame acceleration, adjusted to remove gravity
    // and rotated based on known orientation from quaternion
    dmpGetQuaternion(&q, fifoBuffer);
    dmpGetAccel(&aa, fifoBuffer);
    dmpGetGravity(&gravity, &q);
    dmpGetLinearAccel(&aaReal, &aa, &gravity);
    dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
    cout << "aworld\t" << aaWorld.x << "\t" << aaWorld.y << "\t" << aaWorld.z << endl;
#endif

#if 1// OUTPUT_READABLE_WORLDACCEL
    // display initial world-frame acceleration, adjusted to remove gravity
    // and rotated based on known orientation from quaternion
    dmpGetQuaternion(&q, fifoBuffer);
    dmpGetAccel(&aa, fifoBuffer);
    dmpGetGravity(&gravity, &q);
    dmpGetLinearAccel(&aaReal, &aa, &gravity);
    dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
    cout << "aworld\t" << aaWorld.x << "\t" << aaWorld.y << "\t" << aaWorld.z << endl;
#endif
  }

};
void MPU6050_dmp_test() {
  MPU6050_DMP_test DMP_test;
  if (DMP_test.init()) {
    DMP_test.main();
  }
}
inline int16_t toDegry(float in) {
  return in * 180 / M_PI;
}

void MPU6050_DMP_func::processDate(const uint8_t *buffer) {
  if (nullptr == buffer) {
    return;
  }
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector

  //  OUTPUT_READABLE_EULER
  // display Euler angles in degrees
  std::lock_guard<std::mutex> lock(date_mx_);
  dmpGetQuaternion(&q_, buffer);
  dmpGetEuler(euler_, &q_);
  dmpGetGravity(&gravity, &q_);
  dmpGetYawPitchRoll(yaw_pitch_roll_, &q_, &gravity);
  dmpGetAccel(&aa, buffer);
  dmpGetLinearAccel(&aaReal, &aa, &gravity);
  dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q_);

//  cout << "X" << toDegry(q.x) << " Y" << toDegry(q.y) << " Z" << toDegry(q.z) << " W" << q.w << " ";
//  cout << "eX" << toDegry(euler[0]) << " Y" << toDegry(euler[1]) << " Z" << toDegry(euler[2]);
//  cout << endl;

}

void MPU6050_DMP_func::get(int16_t *yaw_pitch_roll, int16_t *aworld, Quaternion &q) {
  std::lock_guard<std::mutex> lock(date_mx_);
  if (yaw_pitch_roll) {
    yaw_pitch_roll[0] = yaw_pitch_roll_[0];
    yaw_pitch_roll[1] = yaw_pitch_roll_[1];
    yaw_pitch_roll[2] = yaw_pitch_roll_[2];
  }
  if (aworld) {
    aworld[0] = aaWorld.x;
    aworld[1] = aaWorld.y;
    aworld[2] = aaWorld.z;
  }
  q = q_;
}

