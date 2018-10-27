/*
 * CRadar.cpp
 *
 *  Created on: Oct 26, 2018
 *      Author: ominenko
 */

#include "CRadar.h"

CRadar::CRadar(uint8_t _trig, uint8_t _echo, uint8_t _direction) :
    dir_servo(_direction, 0, 180, 103, 520) {


}

