/*
 * ServoCamera.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#include "ServoCamera.h"
#ifndef _SIMULATION_
#include <wiringPi.h>
#include "pca9685.h"
#endif
#include <iostream>
#include <string>
#define _DMSG(arg) std::cout << __FILE__ << ":" << __LINE__ << "  DC[" << pinX << ":"<< pinY<<"] " <<arg<<std::endl

void ServoCamera::init(int _fd, int _pinX, int _pinY, int _valX, int _valY) {
  _DMSG("fd="<<fd);
  fd = _fd;
  pinX = _pinX;
  pinY = _pinY;
  set(_valX, _valY);
}

void ServoCamera::set(int _valX, int _valY) {
  _DMSG("_valX="<<fd<<",_valY="<<_valY);
  valX = _valX;
  valY = _valY;
}

void ServoCamera::setOffset(int _valX, int _valY) {
  _DMSG("_valX="<<fd<<",_valY="<<_valY);
  set(valX + _valX, valY + _valY);
}
