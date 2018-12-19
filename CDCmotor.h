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
  const int pin0_;
  const int pin1_;
public:
  static constexpr auto maxPWM = 0xfff + 1;
  static constexpr auto startPWM = maxPWM / 10; // less power not enought to start
  CDCmotor(int _pin0, int _pin1) :
      pin0_(_pin0), pin1_(_pin1) {
  }
  ;
  void init();
  void set(int16_t power);
};

#endif /* CDCMOTOR_H_ */
