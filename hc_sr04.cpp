/*
 * hc_sr04.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ominenko
 */
#include "hc_sr04.h"
#ifndef _SIMULATION_
#include <wiringPi.h>
#endif
#include<iostream>
#include <string.h>
#include<errno.h>
#include <thread>

using namespace std;

void HC_SR04::echo_handler() {
  if (stop_time == chrono::microseconds(0)) { //1st time
    stop_time = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch());
    }
}

void HC_SR04::init(void (*pEchoHandler)(void)) {
#ifndef _SIMULATION_
  pinMode(pin_trig_, OUTPUT);
  pinMode(pin_echo_, INPUT);
  pullUpDnControl(pin_echo_, PUD_DOWN);
  digitalWrite(pin_trig_, 0);

  if (wiringPiISR(pin_echo_, INT_EDGE_FALLING, pEchoHandler) < 0) {
    cerr << "interrupt error [" << strerror(errno) << "]:" << errno << endl;
    return;
  }
  

#endif
}

int32_t HC_SR04::measure() {
#ifndef _SIMULATION_
  digitalWrite(pin_trig_, 0);
  this_thread::sleep_for(chrono::microseconds(2));
  digitalWrite(pin_trig_, 1);
  this_thread::sleep_for(chrono::microseconds(10));
  digitalWrite(pin_trig_, 0);

  stop_time = chrono::microseconds(0);
  start_time = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch());

  //Wait for echo start

#endif
  const int32_t timeout = (MAX_DISTANCE * 1000 * 3 / SOUND_SPEED);
  this_thread::sleep_for(chrono::milliseconds(timeout));
  if (stop_time == chrono::microseconds(0)) { //no responce
    return -1;
  }
  return (stop_time - start_time).count() * SOUND_SPEED / 2 / 1000000 - 100;
}

//eof
