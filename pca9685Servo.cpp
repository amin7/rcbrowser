/*
 * ServoCamera.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#include "pca9685Servo.h"
#ifndef _SIMULATION_
#include <wiringPi.h>
#include "pca9685.h"
#endif
#include <iostream>
#include <string>
#define _DMSG(arg) std::cout << __FILE__ << ":" << __LINE__ <<"("<<__FUNCTION__<< ")  DC[" << pin <<"] " <<arg<<std::endl

void pca9685_Servo::init(uint8_t init_val) {
  set(init_val);
}

//http://en.wikipedia.org/wiki/Servo_control#Pulse_duration
int pca9685_Servo::calcTicks(float impulseMs, int hertz)
{
    float cycleMs = 1000.0f / hertz;
    return (int)(maxPWM * impulseMs / cycleMs + 0.5f);
}


void pca9685_Servo::set(uint8_t val) {
  if (val > 100) {
    val = 100;
  }
  //_DMSG("val="<<std::string(val));
#ifndef _SIMULATION_
  pwmWrite(300 + pin, minPulse + (maxPulse - minPulse) * val / 100);
#endif
}





