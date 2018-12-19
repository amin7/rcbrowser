/*
 * CManipulator.cpp
 *
 *  Created on: Oct 24, 2018
 *      Author: ominenko
 */

#include "CManipulator.h"

CManipulator::CManipulator(uint8_t pin_base, uint8_t pin_l, uint8_t pin_r) :
    curX(0), curY(0), curZ(0),
        servo_base(pin_base), servo_l(pin_l), servo_r(pin_r) {

}
void CManipulator::init() {
  servo_base.init();
  servo_l.init();
  servo_r.init();
}

void CManipulator::set_absolute(int16_t _x, int16_t _y, int16_t _z) {
  servo_base.setVal(_x);
  servo_l.setVal(_y);
  servo_r.setVal(_z);
}

