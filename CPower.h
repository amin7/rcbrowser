/*
 * CPower.h
 *
 *  Created on: Jan 18, 2019
 *      Author: ominenko
 */

#ifndef CPOWER_H_
#define CPOWER_H_
#include <stdint.h>

class CPower {
  static constexpr auto BASE = 2222;
  static constexpr auto CONFIG_PGA_6_144V = 0x0000;
  static constexpr auto inA0 = 0;
  static constexpr auto inA1 = 1;
  static constexpr auto inA2 = 2;
  static constexpr auto inA3 = 3;
public:
  bool init();
  int16_t getAIN(const uint8_t pin) const; //in mV
  int16_t getVBAT() const {
        return getAIN(inA3) * 2; //Divider 10k+10k
    
  }
  int16_t get5V() const {
    return getAIN(inA0);
  }
};

#endif /* CPOWER_H_ */
