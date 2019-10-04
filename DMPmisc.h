/*
 * MPU6050misc.h
 *
 *  Created on: Dec 28, 2018
 *      Author: ominenko
 */

#ifndef DMPMISC_H_
#define DMPMISC_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#if 0
class MPU6050_misc: private MPU6050 {
  int16_t ax, ay, az, gx, gy, gz;
  int16_t mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz;
  int16_t ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

  //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
  //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
  //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)
  static constexpr auto acel_deadzone = 8;
  static constexpr auto giro_deadzone = 1;
  static constexpr auto buffersize = 1000;
  void meansensors();
  void calibration();
  public:
  void calculate_offs();
};

class MPU6050_DMP: protected MPU6050 {
protected:
  bool inited_ = false;
  virtual void processDate(const uint8_t *buffer)=0;
public:
  bool isInited() const {
    return inited_;
  }
  bool init();
  void main();
};

class MPU6050_DMP_func: public MPU6050_DMP {
protected:
  std::mutex date_mx_;
  float euler_[3];         // [psi, theta, phi]    Euler angle container
  float yaw_pitch_roll_[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
  Quaternion q_;           // [w, x, y, z]         quaternion container
  VectorFloat accel_;
  VectorFloat speed_;
  VectorFloat position_;
  virtual void processDate(const uint8_t *buffer);
  float convert_accel(int16_t val) const;
  public:
  static constexpr auto GRAVITY = 9.807; //m/s*s
  void get(float *yaw_pitch_roll, VectorFloat &accel, Quaternion &q, VectorFloat &pos, VectorFloat &speed);
};
#endif

void dmp_main();
void dmp_dmp_test();
void dmp_calibrate();
#endif /* DMPMISC_H_ */
