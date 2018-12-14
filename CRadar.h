/*
 * CRadar.h
 *
 *  Created on: Oct 26, 2018
 *      Author: ominenko
 */

#ifndef CRADAR_H_
#define CRADAR_H_
#include <stdint.h>
#include <atomic>
#include <thread>
#include "hc_sr04.h"
#include "pca9685Servo.h"
class CRadar {
  //HC_SR04 hc_sr04;
  pca9685_Servo dir_servo;
  void thread();
  std::atomic<bool> execute_ { false };
  std::thread thd_;
  const int16_t angle_min = -90;
  const int16_t angle_max = 90;
  int16_t angle = angle_min;
  bool angle_up = true;
public:
  CRadar(uint8_t _trig_pin, uint8_t _echo_pin, uint8_t _direction_pin);
//  void echo_handler() {
//    hc_sr04.echo_handler();
//  }
  void init(void (*pEchoHandler)(void));
  bool start();
  void stop();
  virtual ~CRadar() {
    stop();
  }
  ;
};

#endif /* CRADAR_H_ */
