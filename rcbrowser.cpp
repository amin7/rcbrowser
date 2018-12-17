/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

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


static struct mg_serve_http_opts s_http_server_opts;
CDCmotor motorL0(2, 3);
CDCmotor motorR0(1, 0);
auto frontend_home = static_cast<string>("");
const auto radar_dir_pin_pca = 14;
const auto radar_trig_pin = 20;
const auto radar_echo_pin = 21;

CRadar radar { radar_trig_pin, radar_echo_pin, radar_dir_pin_pca };

void ultrasonic0_echo_handler() {
  radar.echo_handler();
}

const char *home_page = "/driver.html";

const auto pin_chasis_cameraY = 15;
const auto pwm_chasis_camera_min = 350;
const auto pwm_chasis_camera_max = 550;
pca9685_Servo chasis_camer(pin_chasis_cameraY, 0, 100, pwm_chasis_camera_min, pwm_chasis_camera_max);
CHttpCmdHandler http_cmd_handler;

void print(mg_str str) {
  const char *t = str.p;
  size_t len = str.len;
  while (len--) {
    std::cout << *t;
    t++;
  }
  std::cout << std::endl;
}


bool handle_test(const rapidjson::Document &d, rapidjson::Document &reply) {

  if (d.HasMember("pwm")) {
    const uint8_t pin = d["pwm"].GetInt();
    const uint16_t value = d["value"].GetInt();
    pca9685_Servo::set_PWM(pin, value);
    return true;
  }
  return false;
}

bool handle_status(const rapidjson::Document &d, rapidjson::Document &reply) {
  return true;
}

static bool handle_chasiscamera(const rapidjson::Document &d, rapidjson::Document &reply) {
  try {
  const int16_t Y = d["Y"].GetInt();
  std::cout << "DOM" << "Y" << Y << std::endl;

    chasis_camer.set(Y);
  } catch (RAPIDJSON_ERROR_CHARTYPE err) {
  std::cout <<"error" << std::endl;
}
  return true;
}

static bool handle_chasisradar(const rapidjson::Document &d, rapidjson::Document &reply) {
  auto &allocator = reply.GetAllocator();
  rapidjson::Value values(rapidjson::kArrayType);
  //<angle<timestamp,distance>>
  const auto &maps = radar.getMap();
  for (const auto &element : maps) {
    rapidjson::Value val(rapidjson::kObjectType);
    val.AddMember("angle", element.first, allocator);
    val.AddMember("timestamp", element.second.first.count(), allocator);
    val.AddMember("distance", element.second.second, allocator);
    values.PushBack(val, allocator);
  }
  reply.AddMember("radar", values, allocator);
  return true;
}

bool handle_wheels(const rapidjson::Document &d, rapidjson::Document &reply) {
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

bool handle_manipulator(const rapidjson::Document &d, rapidjson::Document &reply) {
  const int16_t x = d["X"].GetInt();
  const int16_t y = d["Y"].GetInt();
  const int16_t z = d["Z"].GetInt();

  cout << "manipulator=" << x << ":" << y << ":" << z;
  cout << endl;
  manipulator.set_absolute(x, y, z);
  return true;
}

enum {
  http_err_Ok = 200,
  http_err_BadRequest = 400,
  http_err_NotFount = 404,
  http_err_InternallError = 500
};


void command_handler(struct mg_connection *nc, struct http_message *hm, const CHttpCmdHandler::cmd_hander_t &handler) {
  int status_code = http_err_BadRequest;
  const char *c_reply = "";
  rapidjson::Document part_cmd;
  rapidjson::Document part_reply;
  part_reply.SetObject();
  do {
    print(hm->body);
    std::string json;
    json.append(hm->body.p, hm->body.len);
    std::cout << "json=" << json << std::endl;

    if (part_cmd.Parse(json.c_str()).HasParseError()) {
      break;
    }
    status_code = http_err_InternallError;
    if (handler(part_cmd, part_reply)) {
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      part_reply.Accept(writer);
      c_reply = buffer.GetString();
      std::cout << "reply=" << c_reply << std::endl;
      status_code = http_err_Ok; //ok
    }
  } while (0);

  mg_send_response_line(nc, status_code, c_reply);
  nc->flags |= MG_F_SEND_AND_CLOSE;

}
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
  case MG_EV_HTTP_REQUEST: {
    print(hm->uri);
    if (mg_vcmp(&hm->uri, "/") == 0) {
        mg_http_serve_file(nc, hm, frontend_home.c_str(), mg_mk_str("text/html"), mg_mk_str(""));
        break;
      }
      auto handler = http_cmd_handler.get_cmd_handler(hm->uri);
      if (handler) {
        command_handler(nc, hm, *handler);
        break;
      }
      /* Serve static content */
      mg_serve_http(nc, hm, s_http_server_opts);
    }
      break;
    default:
      break;
  }
}

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
  radar.init(ultrasonic0_echo_handler);
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

  http_cmd_handler.add("/test", handle_test);
  http_cmd_handler.add("/chasiscamera", handle_chasiscamera);
  http_cmd_handler.add("/wheels", handle_wheels);
  http_cmd_handler.add("/manipulator", handle_manipulator);
  http_cmd_handler.add("/chasisradar", handle_chasisradar);
  http_cmd_handler.add("/status", handle_status);
//--------------
  if (is_demon_mode) {
    daemonize();
  }

  init();

  std::cout << "Number of threads = " << std::thread::hardware_concurrency() << std::endl;
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
