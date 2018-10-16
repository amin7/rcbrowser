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
  const int pin0;
  const int pin1;
public:
  enum {
    maxPWM = 0xfff,
    startPWM = (0xfff / 10) // less power not enought to start
  };
  CDCmotor(int _pin0, int _pin1) :
      pin0(_pin0), pin1(pin1) {
  }
  ;
  void init();
  void set(int16_t power);
};

#endif /* CDCMOTOR_H_ */
