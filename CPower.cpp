/*
 * CPower.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: ominenko
 */
//https://github.com/Wyliodrin/wiringPi/blob/master/wiringPi/ads1115.c
#include "CPower.h"
#ifndef _SIMULATION_
#include <wiringPi.h>
#include <ads1115.h>
#endif

bool CPower::init() {
#ifndef _SIMULATION_
  if (false == ads1115Setup(BASE, 0x48)) {
    return false;
  }
  auto node = wiringPiFindNode(BASE + 0);
  node->data0 = CONFIG_PGA_6_144V;
#endif
  return true;
}
int16_t CPower::getAIN(const uint8_t pin) const {
#ifndef _SIMULATION_
  return static_cast<int32_t>(analogRead(BASE + pin)) * 6144 / 0x7fff;
#else
  return -1;
#endif
}

