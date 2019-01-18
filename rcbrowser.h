/*
 * rcbrowser.h
 *
 *  Created on: Dec 19, 2018
 *      Author: ominenko
 */

#ifndef RCBROWSER_H_
#define RCBROWSER_H_
#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>
#include <string>
#include "mongoose.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#ifndef _SIMULATION_
#include <wiringPi.h>
#include "pca9685.h"
#endif
#include "CDCmotor.h"
#include "pca9685Servo.h"
#include <map>
#include <unistd.h>
#include <libgen.h>
#include "CLI11.hpp"
#include "demonize.h"
#include "hc_sr04.h"
#include "CManipulator.h"
#include "CRadar.h"
#include "CHttpCmdHandler.h"
#include "MPU6050misc.h"
#include "CPower.h"


auto frontend_home = static_cast<string>("");
constexpr auto radar_dir_pin_pca = 14;
constexpr auto radar_trig_pin = 20;
constexpr auto radar_echo_pin = 21;

constexpr char *home_page = "/driver.html";

constexpr auto pca_pin_chasis_cameraY = 15;
constexpr auto pwm_chasis_camera_min = 350;
constexpr auto pwm_chasis_camera_max = 550;

constexpr auto pca_pin_chasis_motor_r_g = 2;
constexpr auto pca_pin_chasis_motor_r_p = 3;
constexpr auto pca_pin_chasis_motor_l_g = 0;
constexpr auto pca_pin_chasis_motor_l_p = 1;


#endif /* RCBROWSER_H_ */
