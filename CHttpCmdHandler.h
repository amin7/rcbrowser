/*
 * CHttpCmdHandler.h
 *
 *  Created on: Dec 17, 2018
 *      Author: ominenko
 */

#ifndef CHTTPCMDHANDLER_H_
#define CHTTPCMDHANDLER_H_
#include <iostream>
#include <stdint.h>
#include <map>
#include <string>
#include <functional>
#include "mongoose.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
using namespace std;
class CHttpCmdHandler {
public:
  using cmd_hander_t=std::function<bool(const rapidjson::Document &,rapidjson::Document &)>;
  bool add(const string &cmd, const cmd_hander_t &handler);
  const cmd_hander_t* get_cmd_handler(const string &cmd) const;
  const cmd_hander_t* get_cmd_handler(const struct mg_str &uri) const;
  private:
  map<string, cmd_hander_t> cmd_;
};

#endif /* CHTTPCMDHANDLER_H_ */
