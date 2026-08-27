#pragma once

// =============================================================================
// bt_factory.hpp — L10：字符串 ID -> 节点实例
// =============================================================================

#include "node_config.hpp"
#include "ports.hpp"
#include "three_node.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

using NodeBuilder = std::function<std::shared_ptr<ThreeNode>(
    const std::string&, const NodeConfig&)>;

class NodeFactory
{
public:
  template <typename T>
  void registerNodeType(const std::string& registration_id)
  {
    if(m_node_builders.count(registration_id) != 0)
    {
      throw std::runtime_error("Duplicate node registration: " + registration_id);
    }
    m_node_builders[registration_id] = [](const std::string& name, const NodeConfig& config) {
      return std::make_shared<T>(name, config);
    };
    ports_list[registration_id] = T::providedPorts();
  }

  std::shared_ptr<ThreeNode> instantiateNode(
      const std::string& registration_id, const std::string& name, const NodeConfig& config)
  {
    auto it = m_node_builders.find(registration_id);
    if(it == m_node_builders.end())
    {
      throw std::runtime_error("Unknown node registration: " + registration_id);
    }
    const NodeBuilder& builder = it->second;
    return builder(name, config);
  }

  PortsList getPortsList(const std::string& registration_id) const
  {
    auto it = ports_list.find(registration_id);
    if(it == ports_list.end())
    {
      throw std::runtime_error("Unknown node registration: " + registration_id);
    }
    return it->second;
  }

private:
  std::unordered_map<std::string, NodeBuilder> m_node_builders;
  std::unordered_map<std::string, PortsList> ports_list;
};
