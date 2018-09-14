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
public:
  enum {
    maxPWM = 0xfff
  };
  void init(int _fd, int _pinX, int _pinY, int _valX = 50, int _valY = 50);
  void set(int _valX, int _valY);
  void setOffset(int _valX, int _valY);
};

#endif /* SERVOCAMERA_H_ */
