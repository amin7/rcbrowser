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
  const uint8_t pin_;
  const int16_t minVal;
  const int16_t maxVal;
  const uint16_t minPulse;
  const uint16_t maxPulse;
  int16_t val_;
  uint16_t val_to_pwm(int16_t val) const;
public:
  static constexpr auto maxPWM = 0xfff + 1;
  const uint16_t angle_time = .12 * 1000 / 60; // time for rotate on 1 degry SG90
  pca9685_Servo(uint8_t _pin);
  pca9685_Servo(uint8_t _pin, int16_t _minVal, int16_t _maxVal, uint16_t _minPulse, uint16_t _maxPulse);
  void init(int16_t init_val = 0);
  void setVal(int16_t val);
  int16_t getVal() const {
    return val_;
  }
  void set_PWM(uint16_t pulse) {
    set_PWM(pin_, pulse);
  }
  static void set_PWM(uint8_t pin, uint16_t pulse);
};

#endif /* PCA9685SERVO_H_ */
