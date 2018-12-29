/*
 * MPU6050misc.h
 *
 *  Created on: Dec 28, 2018
 *      Author: ominenko
 */

#ifndef MPU6050MISC_H_
#define MPU6050MISC_H_
#include "libs/MPU6050.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
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
  bool init();
  void main();
};

class MPU6050_DMP_func: public MPU6050_DMP {
protected:
  std::mutex date_mx_;
  std::array<int16_t, 3> yaw_pitch_roll_;
  virtual void processDate(const uint8_t *buffer);
  public:
  void get_yaw_pitch_roll(int16_t *yaw_pitch_roll);
};

void MPU6050_main();
void MPU6050_dmp_test();
void MPU6050_calibrate();
#endif /* MPU6050MISC_H_ */
