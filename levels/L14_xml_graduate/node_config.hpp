#pragma once

#include "mini_bt/blackboard.hpp"

#include <map>
#include <memory>
#include <string>

struct NodeConfig
{
  std::shared_ptr<Blackboard> blackboard;
  std::map<std::string, std::string> input_ports;
  std::map<std::string, std::string> output_ports;
};
