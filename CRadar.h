/*
 * CRadar.h
 *
 *  Created on: Oct 26, 2018
 *      Author: ominenko
 */

#ifndef CRADAR_H_
#define CRADAR_H_
#include <stdint.h>
#include "hc_sr04.h"
#include "pca9685Servo.h"
class CRadar {
  HC_SR04 hc_sr04;
  pca9685_Servo dir_servo;
public:
  CRadar(uint8_t _trig, uint8_t _echo, uint8_t _direction);
  void echo_handler() {
    hc_sr04.echo_handler();
  }
  void init(void (*pEchoHandler)(void));
};

#endif /* CRADAR_H_ */
