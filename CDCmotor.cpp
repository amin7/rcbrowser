/*
 * CDCmotor.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#include "CDCmotor.h"
#include <stdlib.h>
#ifndef _SIMULATION_
#include <wiringPi.h>
#include "pca9685.h"
#endif
#include <iostream>
#include <string>

using namespace std;
void CDCmotor::init() {
  set(0);
}
void CDCmotor::set(int16_t power) {
  cout << __FILE__ << ":" << __LINE__ << "  DC[" << pin0_ << ":" << pin1_ << "]=" << power << endl;

  auto pwm_power0 = abs(power) * maxPWM / 100;
  auto pwm_power1 = decltype(pwm_power0) { 0 };
  if (maxPWM < pwm_power0) {
    pwm_power0 = maxPWM;
  }
  if (startPWM > pwm_power0) {
    pwm_power0 = 0;
  }
  if (0 > power) {
    pwm_power1 = pwm_power0;
    pwm_power0 = 0;
  }

#ifndef _SIMULATION_
  pwmWrite(300 + pin0_, pwm_power0);
  pwmWrite(300 + pin1_, pwm_power1);
#endif

}
