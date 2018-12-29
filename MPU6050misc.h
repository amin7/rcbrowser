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

void MPU6050_setoffs(MPU6050 &);
void MPU6050_main();
void MPU6050_dmp();
void MPU6050_calibrate();
#endif /* MPU6050MISC_H_ */
