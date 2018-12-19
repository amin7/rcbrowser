/*
 * hc_sr04.h
 *
 *  Created on: Oct 17, 2018
 *      Author: ominenko
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_
#include <stdint.h>
#include <chrono>

//https://stackoverflow.com/questions/22580242/raspberrypi-g-ultrasonic-sensor-not-working
class HC_SR04 {
  const uint8_t pin_trig_;
  const uint8_t pin_echo_;
  std::chrono::microseconds stop_time;
  std::chrono::microseconds start_time;
  static void echo_handler_(HC_SR04 *);
public:
  static const auto MAX_DISTANCE = 4000;
  static const auto MEASURING_ANGLE = 5; //degre
  static const auto SOUND_SPEED = 343000; //mm/sec
  HC_SR04(uint8_t _trig, uint8_t _echo) :
      pin_trig_(_trig), pin_echo_(_echo) {
  }
  /***
   *  in pEchoHandler  echo_handler must be called;
   */

  void echo_handler();
  void init(void (*pEchoHandler)(void));
  /***
   * maxDistance im mm , to time out echo;
   * ret distance in mm, -1 - no echo
   */

  int32_t measure();
  };


#endif /* HC_SR04_H_ */
