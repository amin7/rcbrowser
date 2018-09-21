/*
 * ServoCamera.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#ifndef SERVOCAMERA_H_
#define SERVOCAMERA_H_
#include <stdint.h>
class ServoCamera {
  int fd = -1;
  int pinX;
  int pinY;
  int valX;
  int valY;
  const int minPulse=maxPWM*1/(1000.0f / 50);
  const int maxPulse=maxPWM*2/(1000.0f / 50);
public:
  enum {
    maxPWM = 0xfff+1
  };
  void init(int _fd, int _pinX, int _pinY, int _valX = 50, int _valY = 50);
  void set(int _valX, int _valY);
  void setOffset(int _valX, int _valY);
  void serve(int _valX, int _valY);
  static int calcTicks(float impulseMs, int hertz);
};

#endif /* SERVOCAMERA_H_ */
