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
#include "ServoCamera.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
CDCmotor motorL0, motorL1;
CDCmotor motorR0, motorR1;
ServoCamera servoCamera;

void call_from_thread() {
  std::cout << "thread function" << std::endl;
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

static void handle_camera(struct mg_connection *nc, struct http_message *hm) {
  std::cout << "handle_camera"
      "" << std::endl;
#ifndef _SIMULATION_
  pinMode(0, LOW);
#endif
  print(hm->body);
  std::string json;
  json.append(hm->body.p, hm->body.len);
  std::cout << "json" << json << std::endl;
  rapidjson::Document d;
  d.Parse(json.c_str());
  const int16_t deltaX=d["deltaX"].GetInt();
  const int16_t deltaY=d["deltaY"].GetInt();
  std::cout << "DOM" << "deltaX" << deltaX << "deltaY" <<deltaY << std::endl;

  mg_send_response_line(nc, 200, "");
  nc->flags |= MG_F_SEND_AND_CLOSE;

  servoCamera.setOffset(deltaX, deltaY);
}

static void handle_wheels(struct mg_connection *nc, struct http_message *hm) {
  std::cout << "handle_wheels" << std::endl;
  print(hm->body);
  std::string json;
  json.append(hm->body.p, hm->body.len);
  std::cout << "json" << json << std::endl;
  rapidjson::Document d;
  d.Parse(json.c_str());
//  /* Send headers */
//  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//
//  /* Compute the result and send it back as a JSON object */
//  auto result = 0;
//  mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
//  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
  const int16_t wheel_L0 = d["wheel_L0"].GetInt();
  const int16_t wheel_L1 = d["wheel_L1"].GetInt();
  const int16_t wheel_R0 = d["wheel_R0"].GetInt();
  const int16_t wheel_R1 = d["wheel_R1"].GetInt();
  mg_send_response_line(nc, 200, "");
  nc->flags |= MG_F_SEND_AND_CLOSE;


  std::cout << "DOM";
  std::cout << "wheel_L=" << wheel_L0 << ":" << wheel_L1;
  std::cout << "wheel_R=" << wheel_R0 << ":" << wheel_R1;
  std::cout << std::endl;
  motorL0.set(wheel_L0);
  motorL1.set(wheel_L1);
  motorR0.set(wheel_R0);
  motorR1.set(wheel_R1);
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
  case MG_EV_HTTP_REQUEST: {
    print(hm->uri);
    if (mg_vcmp(&hm->uri, "/") == 0) {
      mg_http_serve_file(nc, hm, "./frontend/rcbrowser.html", mg_mk_str("text/html"), mg_mk_str(""));
    } else {
      if (mg_vcmp(&hm->uri, "/camera") == 0) {
        handle_camera(nc, hm); /* Handle RESTful call */
      }
      if (mg_vcmp(&hm->uri, "/wheels") == 0) {
        handle_wheels(nc, hm); /* Handle RESTful call */
      } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
        char buf[100] = {0};
        memcpy(buf, hm->body.p,
               sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
      } else {
        mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      }
    }
      break;
    default:
      break;
  }
}
}

//int main(int argc, char *argv[]) {
#define PIN_BASE 300
#define HERTZ 50

int main() {
  int fd = 0;
#ifndef _SIMULATION_
  wiringPiSetup();
  fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
  pca9685PWMReset(fd);
#endif
  motorL0.init(fd, 0, 1);
  motorL1.init(fd, 2, 3);
  motorR0.init(fd, 4, 5);
  motorR1.init(fd, 6, 7);
  servoCamera.init(fd, 14, 15);

  std::cout << "Number of threads = " << std::thread::hardware_concurrency() << std::endl;
  std::thread t1(call_from_thread);
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;

  const char *err_str;
  mg_mgr_init(&mgr, NULL);
  s_http_server_opts.document_root = "./frontend/";
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
