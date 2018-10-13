/*
 * CDCmotor.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#ifndef CDCMOTOR_H_
#define CDCMOTOR_H_
#include <stdint.h>

class CDCmotor {
  int fd = -1;
  int pin0;
  int pin1;
  const bool inversion;
public:
  enum {
    maxPWM = 0xfff,
    startPWM = (0xfff / 10) // less power not enought to start
  };
  CDCmotor(bool _inv=false):inversion(_inv){};
  void init(int _fd, int _pin0, int _pin1);
  void set(int16_t power);
};

#endif /* CDCMOTOR_H_ */
