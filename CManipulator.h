/*
 * CManipulator.h
 *
 *  Created on: Oct 24, 2018
 *      Author: ominenko
 */

//https://github.com/lamjonah/ThisArm_Firmware/blob/master/ThisArm/CoordinateControl.h
#ifndef CMANIPULATOR_H_
#define CMANIPULATOR_H_
#include <stdint.h>
#include "pca9685Servo.h"

class CManipulator {
  pca9685_Servo servo_base;
  pca9685_Servo servo_shoulder;
  pca9685_Servo servo_elbow;

  int InverseKinematicsTransform(double R, double H, double &Shoulder, double &Elbow);
public:
  static constexpr auto BackArm_mm = 80;
  static constexpr auto ForeArm_mm = 80;
  static constexpr auto d_Min = 15;
  static constexpr auto d_Max = 155;
  static constexpr auto R_offset_mm = 7.5; // length in MM, that the Left and Right servo place outward than rotation centre
  CManipulator(uint8_t pin_base, uint8_t pin_shoulder, uint8_t pin_elbow);
  void init();
  void set_bse(int16_t base, int16_t shoulder, int16_t elbow);
  void set_xyz(int16_t _x, int16_t _y, int16_t _z);
  //void set_relative(int16_t _dx, int16_t _dy, int16_t _dz);
};

#endif /* CMANIPULATOR_H_ */
