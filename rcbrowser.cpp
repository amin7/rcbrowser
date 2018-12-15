/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>
#include <string>
#include "rapidjson/reader.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
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

using namespace std;

typedef bool (*cmd_hander_t)(rapidjson::Document &);

static struct mg_serve_http_opts s_http_server_opts;
CDCmotor motorL0(2, 3);
CDCmotor motorR0(1, 0);
auto frontend_home = static_cast<string>("");
const auto radar_dir_pin_pca = 14;
const auto radar_trig_pin = 20;
const auto radar_echo_pin = 21;
//HC_SR04 ultrasonic0(28, 29); //wiringPI
HC_SR04 ultrasonic0 { 20, 21 }; //GPIO
CRadar radar { radar_trig_pin, radar_echo_pin, radar_dir_pin_pca };

void ultrasonic0_echo_handler() {
  ultrasonic0.echo_handler();
}

const char *home_page = "/driver.html";

const auto pin_chasis_cameraY = 15;
const auto pwm_chasis_camera_min = 350;
const auto pwm_chasis_camera_max = 550;
pca9685_Servo chasis_camer(pin_chasis_cameraY, 0, 100, pwm_chasis_camera_min, pwm_chasis_camera_max);

void call_from_thread() {
    int32_t prev=0;
  cout << "thread function" << endl;
  while (1) {
      const auto cur =ultrasonic0.measure();
      if(cur!=prev)
          cout << "ultrasonic=" << cur << endl;
      prev=cur;

  }
}

void print(mg_str str) {
  const char *t = str.p;
  size_t len = str.len;
  while (len--) {
    std::cout << *t;
    t++;
  }
  std::cout << std::endl;
}

bool handle_test(rapidjson::Document &d) {

  if (d.HasMember("pwm")) {
    const uint8_t pin = d["pwm"].GetInt();
    const uint16_t value = d["value"].GetInt();
    pca9685_Servo::set_PWM(pin, value);
    return true;
  }
  return false;
}



static bool handle_chasiscamera(rapidjson::Document &d) {
  try {
  const int16_t Y = d["Y"].GetInt();
  std::cout << "DOM" << "Y" << Y << std::endl;

    chasis_camer.set(Y);
  } catch (RAPIDJSON_ERROR_CHARTYPE err) {
  std::cout <<"error" << std::endl;
}
  return true;
}

bool handle_wheels(rapidjson::Document &d) {
  const int16_t wheel_L0 = d["wheel_L0"].GetInt();
  const int16_t wheel_R0 = d["wheel_R0"].GetInt();

  cout << "wheel=" << wheel_L0 << ":" << wheel_R0;
  cout << std::endl;
  motorL0.set(wheel_L0);
  motorR0.set(wheel_R0);
  return true;
}

const auto pin_manipulator_base = 4;
const auto pin_manipulator_l = 5;
const auto pin_manipulator_r = 6;

CManipulator manipulator(pin_manipulator_base, pin_manipulator_l, pin_manipulator_r);

bool handle_manipulator(rapidjson::Document &d) {
  const int16_t x = d["X"].GetInt();
  const int16_t y = d["Y"].GetInt();
  const int16_t z = d["Z"].GetInt();

  cout << "manipulator=" << x << ":" << y << ":" << z;
  cout << endl;
  manipulator.set_absolute(x, y, z);
  return true;
}

map<string, cmd_hander_t> cmd_map = {
    { "test", handle_test },
    { "chasiscamera", handle_chasiscamera },
    { "wheels", handle_wheels },
    { "manipulator", handle_manipulator }
};
enum {
  http_err_Ok = 200,
  http_err_BadRequest = 400,
  http_err_NotFount = 404,
  http_err_InternallError = 500,
  

};
void command_handler(struct mg_connection *nc, struct http_message *hm) {
  int status_code = http_err_BadRequest;
  std::cout << "command_handler" << std::endl;
  do {
    print(hm->body);
    std::string json;
    json.append(hm->body.p, hm->body.len);
    std::cout << "json=" << json << std::endl;
    rapidjson::Document d;
    if (d.Parse(json.c_str()).HasParseError()) {
      break;
    }
    if (!d.HasMember("cmd")) {
      break;
    }
    const char *cmd = d["cmd"].GetString();
    status_code = http_err_NotFount;

    auto it = cmd_map.find(cmd);
    if (it != cmd_map.end()) {
      status_code = http_err_Ok; //ok
      if (!(*it->second)(d)) {
        status_code = http_err_InternallError;
      }
    }
  } while (0);
  mg_send_response_line(nc, status_code, "");
  nc->flags |= MG_F_SEND_AND_CLOSE;

}
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
  case MG_EV_HTTP_REQUEST: {
    print(hm->uri);
    if (mg_vcmp(&hm->uri, "/") == 0) {
        mg_http_serve_file(nc, hm, frontend_home.c_str(), mg_mk_str("text/html"), mg_mk_str(""));
    } else
    if (mg_vcmp(&hm->uri, "/command") == 0) {
      command_handler(nc, hm);
    } else {
      mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
    }
  }
      break;
    default:
      break;
  }
}

//int main(int argc, char *argv[]) {
#define PIN_BASE 300
#define HERTZ 50


void init() {
#ifndef _SIMULATION_
  int fd = 0;
  //wiringPiSetup();
  wiringPiSetupGpio(); //use broadcom naming
  fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
  pca9685PWMReset(fd);
#endif
  motorR0.init();
  motorL0.init();
  chasis_camer.init();
  ultrasonic0.init(ultrasonic0_echo_handler);
  manipulator.init();
  radar.start();
}

int main(int argc, char *argv[]) {
  CLI::App app { "rc browser" };
  bool is_demon_mode;
  string frontend_folder = "";
  string http_port = "8000";
  app.add_flag("-d", is_demon_mode, "demon mode");
  app.add_option("-f", frontend_folder, "frontend_folder")->check(CLI::ExistingDirectory);
  app.add_option("-p", http_port, "http_port")->check(CLI::ExistingDirectory);

  CLI11_PARSE(app, argc, argv);

  if ("" == frontend_folder) { //use current dir
    char cwd[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", cwd, PATH_MAX);
    if (-1 == count) {
      perror("can't get readlink");
      return 1;
    }
    frontend_folder = dirname(cwd);
    frontend_folder += "/frontend";

  }
  cout << "is_demon_mode=" << is_demon_mode << endl;
  cout << "frontend_path=" << frontend_folder << endl;
  cout << "http_port=" << http_port << endl;
  cout << "home_page=" << home_page << endl;

  frontend_home = frontend_folder + home_page;
//--------------
  if (is_demon_mode) {
    daemonize();
  }

  init();

  std::cout << "Number of threads = " << std::thread::hardware_concurrency() << std::endl;
  std::thread t1(call_from_thread);
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;

  const char *err_str;
  mg_mgr_init(&mgr, NULL);

  s_http_server_opts.document_root = frontend_folder.c_str();
  //s_http_server_opts.url_rewrites = NULL;
  /* Set HTTP server options */
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.error_string = &err_str;
  nc = mg_bind_opt(&mgr, http_port.c_str(), ev_handler, bind_opts);
  if (nc == NULL) {
    fprintf(stderr, "Error starting server on port %s: %s\n", http_port.c_str(),
            *bind_opts.error_string);
    exit(1);
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.enable_directory_listing = "no";
  cout << "Starting RESTful server" << endl;
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
