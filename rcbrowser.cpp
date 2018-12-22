/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */
#include "rcbrowser.h"
using namespace std;

static struct mg_serve_http_opts s_http_server_opts;

CRadar radar { radar_trig_pin, radar_echo_pin, radar_dir_pin_pca };

void ultrasonic0_echo_handler() {
  radar.echo_handler();
}

pca9685_Servo chasis_camer(pca_pin_chasis_cameraY, 0, 100, pwm_chasis_camera_min, pwm_chasis_camera_max);
CHttpCmdHandler http_cmd_handler;

void print(mg_str str) {
  const char *t = str.p;
  size_t len = str.len;
  while (len--) {
    cout << *t;
    t++;
  }
  cout << endl;
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
  auto &allocator = reply.GetAllocator();
  reply.AddMember("vbat", 8.5, allocator);
  return true;
}

static bool handle_chasiscamera(const rapidjson::Document &d, rapidjson::Document &reply) {
  if (d.HasMember("Y")) {
    const auto y = d["Y"].GetInt();
    chasis_camer.setVal(y);
  }
  auto &allocator = reply.GetAllocator();
  reply.AddMember("Y", chasis_camer.getVal(), allocator);
  return true;
}

static bool handle_chasisradar(const rapidjson::Document &d, rapidjson::Document &reply) {
  auto lastUpd = static_cast<int64_t>(0);
  auto sendDelta = false;
  if (d.HasMember("timestamp")) {
    lastUpd = d["timestamp"].GetInt64();
  }
  auto &allocator = reply.GetAllocator();
  if (0 == lastUpd) { //send setting info
    reply.AddMember("AngleMin", radar.getAngleMin(), allocator);
    reply.AddMember("AngleMax", radar.getAngleMax(), allocator);
    reply.AddMember("AngleStep", radar.getAngleStep(), allocator);
  }
  rapidjson::Value values(rapidjson::kArrayType);
  //<angle<timestamp,distance>>
  const auto &maps = radar.getMap();
  for (const auto &element : maps) {
    const auto timestamp = element.second.first.count();
    if (lastUpd <= timestamp) {
      lastUpd = timestamp;
    } else {
      if (sendDelta) {
        continue;
      }
    }
    rapidjson::Value val(rapidjson::kObjectType);
    val.AddMember("angl", element.first, allocator);
    val.AddMember("time", timestamp, allocator);
    val.AddMember("dist", element.second.second, allocator);
    values.PushBack(val, allocator);
  }
  reply.AddMember("radar", values, allocator);
  return true;
}

CDCmotor motorL0(pca_pin_chasis_motor_l_g, pca_pin_chasis_motor_l_p);
CDCmotor motorR0(pca_pin_chasis_motor_r_p, pca_pin_chasis_motor_r_g);
bool handle_wheels(const rapidjson::Document &d, rapidjson::Document &reply) {
  const int16_t wheel_L0 = d["wheel_L0"].GetInt();
  const int16_t wheel_R0 = d["wheel_R0"].GetInt();

  cout << "wheel=" << wheel_L0 << ":" << wheel_R0;
  cout << endl;
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
    string json;
    json.append(hm->body.p, hm->body.len);
    cout << "json=" << json << endl;
    if (json.length()) {
      if (part_cmd.Parse(json.c_str()).HasParseError()) {
        break;
      }
    } else {
      part_cmd.SetObject(); //dummy object
    }
    status_code = http_err_InternallError;
    if (handler(part_cmd, part_reply)) {
      status_code = http_err_Ok; //ok
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      part_reply.Accept(writer);
      c_reply = buffer.GetString();
      cout << "reply=" << c_reply << endl;
      const auto reply_sz = strlen(c_reply);
      mg_send_head(nc, status_code, reply_sz, nullptr);
      mg_send(nc, c_reply, reply_sz);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      return;
    }
  } while (0);

  mg_send_response_line(nc, status_code, nullptr);
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


void MPU6050_dmp() {
  cout << "MPU6050_dmp" << endl;
  MPU6050 mpu;
  mpu.setup();
  mpu.initialize();
  const auto dmpInitializeState = mpu.dmpInitialize();
  if (dmpInitializeState) {
    cout << "dmp init err=" << static_cast<int>(dmpInitializeState) << endl;
    return; //eeror;
  }
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  mpu.setDMPEnabled(true);

}

void MPU6050_main() {
  cout << "MPU6050 3-axis acceleromter example program" << endl;
  MPU6050 accelgyro;
  accelgyro.setup();

  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  cout << "ID=" << static_cast<int>(accelgyro.getDeviceID()) << endl;
  if (accelgyro.testConnection())
    cout << "MPU6050 connection test successful" << endl;
  else {
    cerr << "MPU6050 connection test failed! something maybe wrong, continuing anyway though ..." << endl;
    return;
  }
  accelgyro.initialize();
  // use the code below to change accel/gyro offset values
  /*
   printf("Updating internal sensor offsets...\n");
   // -76  -2359 1688  0 0 0
   printf("%i \t %i \t %i \t %i \t %i \t %i\n",
   accelgyro.getXAccelOffset(),
   accelgyro.getYAccelOffset(),
   accelgyro.getZAccelOffset(),
   accelgyro.getXGyroOffset(),
   accelgyro.getYGyroOffset(),
   accelgyro.getZGyroOffset());
   accelgyro.setXGyroOffset(220);
   accelgyro.setYGyroOffset(76);
   accelgyro.setZGyroOffset(-85);
   printf("%i \t %i \t %i \t %i \t %i \t %i\n",
   accelgyro.getXAccelOffset(),
   accelgyro.getYAccelOffset(),
   accelgyro.getZAccelOffset(),
   accelgyro.getXGyroOffset(),
   accelgyro.getYGyroOffset(),
   accelgyro.getZGyroOffset());
   */

  cout << endl;
  cout << "  ax \t ay \t az \t gx \t gy \t gz" << endl;

  while (true) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
  }
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

  cout << "Number of threads = " << thread::hardware_concurrency() << endl;
  auto gyro_thread = std::thread(MPU6050_dmp);

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
