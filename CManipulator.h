/*
 * CManipulator.h
 *
 *  Created on: Oct 24, 2018
 *      Author: ominenko
 */

#ifndef CMANIPULATOR_H_
#define CMANIPULATOR_H_
#include <stdint.h>
#include "pca9685Servo.h"

class CManipulator {
  int16_t curX, curY, curZ;
  pca9685_Servo servo_base;
  pca9685_Servo servo_l;
  pca9685_Servo servo_r;
public:
  CManipulator(uint8_t pin_base, uint8_t pin_l, uint8_t pin_r);
  void init();
  void set_absolute(int16_t _x, int16_t _y, int16_t _z);
  void set_relative(int16_t _dx, int16_t _dy, int16_t _dz);
};

#endif /* CMANIPULATOR_H_ */
