/*
 * CHttpCmdHandler.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: ominenko
 */

#include "CHttpCmdHandler.h"

bool CHttpCmdHandler::add(const string &cmd, const cmd_hander_t &handler) {
  auto it = cmd_.find(cmd);
  if (it != cmd_.end()) {
    return false; //already present
  }
  cmd_[cmd] = handler;
}

const CHttpCmdHandler::cmd_hander_t* CHttpCmdHandler::get_cmd_handler(const string &cmd) const {
  auto it = cmd_.find(cmd);
  if (it != cmd_.end()) {
    return &(it->second);
  }
return nullptr;
}

const CHttpCmdHandler::cmd_hander_t* CHttpCmdHandler::get_cmd_handler(const struct mg_str &uri) const {
  for (const auto &it : cmd_) {
    if (mg_vcmp(&uri, it.first.c_str()) == 0) {
      return &(it.second);
    }
  }
  return nullptr;
}

