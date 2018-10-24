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
std::chrono::microseconds stop_time;
void begin_interrupt() {
  if (stop_time == chrono::microseconds(0)) { //1st time
    stop_time = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch());
    }
}

void HC_SR04::init() {
#ifndef _SIMULATION_
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pullUpDnControl(echo, PUD_DOWN);
  digitalWrite(trig, 0);

  if (wiringPiISR(echo, INT_EDGE_RISING, &begin_interrupt) < 0) {
    cerr << "interrupt error [" << strerror(errno) << "]:" << errno << endl;
    return;
  }
  
#endif
  sound_speed = soundspeed();
}
float HC_SR04::soundspeed(float temperature, float hum) {
  return 331400 + 606 * temperature + 12.4 * hum;
}

int32_t HC_SR04::measure() {
#ifndef _SIMULATION_
  digitalWrite(trig, 0);
  this_thread::sleep_for(chrono::microseconds(2));
  digitalWrite(trig, 1);
  this_thread::sleep_for(chrono::microseconds(10));
  digitalWrite(trig, 0);

  stop_time = chrono::microseconds(0);
  start_time = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch());

  //Wait for echo start
//          while(digitalRead(echo) == LOW);
//
//          //Wait for echo end
//          while(digitalRead(echo) == HIGH);
#endif
  const int32_t timeout = (MAX_DISTANCE * 1000 * 2 / sound_speed);
  this_thread::sleep_for(chrono::milliseconds(timeout));
  if (stop_time == chrono::microseconds(0)) { //no responce
    return -1;
  }
  return (stop_time - start_time).count()*sound_speed / 2/1000000;
}

//eof
