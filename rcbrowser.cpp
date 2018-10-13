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

using namespace std;

typedef bool (*cmd_hander_t)(rapidjson::Document &);

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
CDCmotor motorL0;
CDCmotor motorR0(true);

const auto pin_chasis_cameraY = 15;
pca9685_Servo chasis_camer(pin_chasis_cameraY);

void call_from_thread() {
  cout << "thread function" << std::endl;
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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


static bool handle_chasiscamera(rapidjson::Document &d) {
  try {
  const int16_t Y = d["Y"].GetInt();
  std::cout << "DOM" << "Y" << Y << std::endl;

  chasis_camer.set(Y*65/100);
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


map<string, cmd_hander_t> cmd_map = {
    { "chasiscamera", handle_chasiscamera },
    { "wheels", handle_wheels }
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
      mg_http_serve_file(nc, hm, "./frontend/driver.html", mg_mk_str("text/html"), mg_mk_str(""));
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

}

int main() {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("Current working dir: %s\n", cwd);
  } else {
      perror("getcwd() error");
      return 1;
  }

  int fd = 0;
#ifndef _SIMULATION_
  wiringPiSetup();
  fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
  pca9685PWMReset(fd);
#endif
  init();
  motorR0.init(fd, 0, 1);
  motorL0.init(fd, 2, 3);
  chasis_camer.init();

  std::cout << "Number of threads = " << std::thread::hardware_concurrency() << std::endl;
  std::thread t1(call_from_thread);
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;

  const char *err_str;
  mg_mgr_init(&mgr, NULL);
  string front_path=cwd;
  front_path+="/frontend/";
  s_http_server_opts.document_root = front_path.c_str();
  //s_http_server_opts.url_rewrites = NULL;
  /* Set HTTP server options */
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.error_string = &err_str;
  nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
  if (nc == NULL) {
    fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
            *bind_opts.error_string);
    exit(1);
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.enable_directory_listing = "no";
  printf("Starting RESTful server on port %s, serving %s\n", s_http_port,
         s_http_server_opts.document_root);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
