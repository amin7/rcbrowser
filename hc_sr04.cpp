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
#include<errno.h>
#include <thread>

using namespace std;

void HC_SR04::init() {
#ifndef _SIMULATION_
//  pinMode(trig, OUTPUT);
//  pinMode(echo, INPUT);
//  pullUpDnControl(echo, PUD_DOWN);
//
//  if (wiringPiISR(echo, INT_EDGE_RISING, &myInterrupt) < 0) {
//    cerr << "interrupt error [" << strerror(errno) << "]:" << errno << endl;
//    return;
//  }
#endif
}
int32_t HC_SR04::measure(int32_t maxDistance) {
  stop_time = chrono::milliseconds(0);
  start_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

#ifndef _SIMULATION_
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
#endif
  this_thread::sleep_for(chrono::milliseconds(0));
  if (stop_time == chrono::milliseconds(0)) { //no responce
    return -1;
  }

  return (stop_time - start_time).count() / 58;
}

//eof
