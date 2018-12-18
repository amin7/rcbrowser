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
#include <mutex>
#include <chrono>
#include <map>
#include "hc_sr04.h"
#include "pca9685Servo.h"

using surround_t=std::map<int16_t, std::pair<std::chrono::milliseconds, int32_t>>;
class CRadar {
  HC_SR04 hc_sr04;
  void thread();
  std::atomic<bool> execute_ { false };
  std::thread thd_;
  const int16_t angle_min = -90;
  const int16_t angle_max = 90;
  const int16_t pwm_min = 110;
  const int16_t pwm_max = 510;
  int16_t angle = angle_min;
  bool angle_up = true;
  pca9685_Servo dir_servo;
  std::mutex map_mu_;
  //<angle<timestamp,distance>>
  surround_t surrond_;
public:
  CRadar(uint8_t _trig_pin, uint8_t _echo_pin, uint8_t _direction_pin);
  void echo_handler() {
    hc_sr04.echo_handler();
  }
  void init(void (*pEchoHandler)(void)) {
    hc_sr04.init(pEchoHandler);
  }

  bool start();
  void stop();
  surround_t getMap() {
    map_mu_.lock();
    auto surrount = surrond_;
    map_mu_.unlock();
    return surrount;
  }
  virtual ~CRadar() {
    stop();
  }
  const int16_t getAngleStep() const
  {
    return hc_sr04.MEASURING_ANGLE;
  }

  const int16_t getAngleMax() const
  {
    return angle_max;
  }

  const int16_t getAngleMin() const
  {
    return angle_min;
  }

  ;
};

#endif /* CRADAR_H_ */
