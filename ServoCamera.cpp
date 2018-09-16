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
#define _DMSG(arg) std::cout << __FILE__ << ":" << __LINE__ <<"("<<__FUNCTION__<< ")  DC[" << pinX << ":"<< pinY<<"] " <<arg<<std::endl

void ServoCamera::init(int _fd, int _pinX, int _pinY, int _valX, int _valY) {
  _DMSG("fd="<<fd);
  fd = _fd;
  pinX = _pinX;
  pinY = _pinY;
  set(_valX, _valY);
}
void toRange(int &Value){
    if(100<Value){
        Value=100;
        return;
    }
    if(0>Value){
        Value=0;
        return;
    }
}
//http://en.wikipedia.org/wiki/Servo_control#Pulse_duration
int ServoCamera::calcTicks(float impulseMs, int hertz)
{
    float cycleMs = 1000.0f / hertz;
    return (int)(maxPWM * impulseMs / cycleMs + 0.5f);
}


void ServoCamera::set(int _valX, int _valY) {
    toRange(_valX);
    toRange(_valY);
  _DMSG("_valX="<<_valY<<",_valY="<<_valY);
  valX = _valX;
  valY = _valY;
  serve(valX,valY);
}

void ServoCamera::serve(int _valX, int _valY) {
    toRange(_valX);
    toRange(_valY);
  _DMSG("_valX="<<_valX<<",_valY="<<_valY);
  pwmWrite (300+pinX, minPulse+minPulse*_valX/100);
  pwmWrite (300+pinY, minPulse+minPulse*_valY/100);
}

void ServoCamera::setOffset(int _valX, int _valY) {
  _DMSG("_valX="<<fd<<",_valY="<<_valY);
  valX += _valX;
  valY += _valY;
  set(valX , valY);
}

void ServoCamera::serveOffset(int _valX, int _valY) {
  _DMSG("_valX="<<fd<<",_valY="<<_valY);
  serve(valX + _valX, valY + _valY);
}
