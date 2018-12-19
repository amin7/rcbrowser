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


auto frontend_home = static_cast<string>("");
const auto radar_dir_pin_pca = 14;
const auto radar_trig_pin = 20;
const auto radar_echo_pin = 21;

constexpr char *home_page = "/driver.html";

constexpr auto pin_chasis_cameraY = 15;
constexpr auto pwm_chasis_camera_min = 350;
constexpr auto pwm_chasis_camera_max = 550;


#endif /* RCBROWSER_H_ */
