#pragma once

// =============================================================================
// NodeConfig — 来自 L9，L10 直接复用，本关不重新发明。
//
// 本关本地副本（与 L9 同构）。只使用本目录头文件。
//
// L10 里它出现在哪：
//   instantiateNode(registration_id, name, config)
//   以及 NodeBuilder(name, config) -> node
// =============================================================================

#include "blackboard.hpp"

#include <map>
#include <memory>
#include <string>

// 每个节点实例的“接线图”：
// - blackboard     : 数据存放处
// - input_ports    : 口名 -> "hello" | "{key}"
// - output_ports   : 口名 -> "{key}"
struct NodeConfig
{
  std::shared_ptr<Blackboard> blackboard;
  std::map<std::string, std::string> input_ports;
  std::map<std::string, std::string> output_ports;
};
