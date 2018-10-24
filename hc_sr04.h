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
  const uint8_t trig;
  const uint8_t echo;
  std::chrono::microseconds start_time;
  float sound_speed;
public:
  enum {
    MAX_DISTANCE = 4000,
    MEASURING_ANGLE = 15 //degre
  };
  HC_SR04(uint8_t _trig, uint8_t _echo) :
      trig(_trig), echo(_echo) {
  }

  void init();
  /*
   *  result im mm/sec
   */
  static float soundspeed(float temperature = 20, float hum = 50);
  /***
   * maxDistance im mm , to time out echo;
   * ret distance in mm, -1 - no echo
   */

  int32_t measure();
  };


#endif /* HC_SR04_H_ */
