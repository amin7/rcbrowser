/*
 * ServoCamera.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ominenko
 */

#ifndef PCA9685SERVO_H_
#define PCA9685SERVO_H_
#include <stdint.h>
//http://en.wikipedia.org/wiki/Servo_control#Pulse_duration

class pca9685_Servo {
  const uint8_t pin;
  const int16_t minVal;
  const int16_t maxVal;
  const uint16_t minPulse;
  const uint16_t maxPulse;
public:
  enum {
    maxPWM = 0xfff+1
  };
  pca9685_Servo(uint8_t _pin);
  pca9685_Servo(uint8_t _pin, int16_t _minVal, int16_t _maxVal, uint16_t _minPulse, uint16_t _maxPulse);
  void init(uint8_t init_val = 50);
  void set(int16_t val);
  void set_PWM(uint16_t pulse) {
    set_PWM(pin, pulse);
  }
  static void set_PWM(uint8_t pin, uint16_t pulse);
};

#endif /* PCA9685SERVO_H_ */
