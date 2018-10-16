/*
 * ServoCamera.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#ifndef PCA9685SERVO_H_
#define PCA9685SERVO_H_
#include <stdint.h>
class pca9685_Servo {
  const uint8_t pin;
  const int minPulse=0*(50.0f*maxPWM/1000);
  const int maxPulse=2.5*(50.0f*maxPWM/1000);
  static int calcTicks(float impulseMs, int hertz);
public:
  enum {
    maxPWM = 0xfff+1
  };
  pca9685_Servo(uint8_t _pin) :
      pin(_pin) {
  }
  ;
  void init(uint8_t init_val = 50);
  void set(uint8_t val);
};

#endif /* PCA9685SERVO_H_ */