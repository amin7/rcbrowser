/*
 * CDCmotor.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#include "CDCmotor.h"
#ifndef _SIMULATION_
#include <wiringPi.h>
#include "pca9685.h"
#endif
#include <iostream>
#include <string>
#define _DMSG(arg) std::cout << __FILE__ << ":" << __LINE__ << "  DC[" << pin0 << ":"<< pin1<<"] " <<arg<<std::endl


void CDCmotor::init(int _fd, int _pin0, int _pin1) {
  _DMSG("fd="<<_fd);
  fd = _fd;
  pin0 = _pin0;
  pin1 = _pin1;
  set(0);
}
void CDCmotor::set(int16_t power) {
  if (-1 == fd) {
    _DMSG("not inited");
    return;
  }
  _DMSG("power=" << power);

  auto zeroPin = pin0;
  auto powerPin = pin1;
  if (0 > power) {
    zeroPin = pin1;
    powerPin = pin0;
    power = -power;
  }
  if (100 < power) {
    power = 100;
  }
  power = power * maxPWM / 100;
  if (startPWM > power) {
    _DMSG("less than start " << startPWM);
    power = 0;
  }
#ifndef _SIMULATION_
  pwmWrite(300+ zeroPin,  0);
  pwmWrite(300+ powerPin, power);
#endif
}
