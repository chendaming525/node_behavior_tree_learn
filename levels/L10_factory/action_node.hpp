#pragma once

#include "three_node.hpp"

class AlwaysSuccessNode : public ThreeNode
{
public:
  explicit AlwaysSuccessNode(const std::string& name, NodeConfig config = {})
    : ThreeNode(name, std::move(config))
  {}

  static PortsList providedPorts()
  {
    return {};
  }

  mini_bt::NodeStatus tick() override
  {
    return mini_bt::NodeStatus::SUCCESS;
  }
};

class AlwaysFailureNode : public ThreeNode
{
public:
  explicit AlwaysFailureNode(const std::string& name, NodeConfig config = {})
    : ThreeNode(name, std::move(config))
  {}

  static PortsList providedPorts()
  {
    return {};
  }

  mini_bt::NodeStatus tick() override
  {
    return mini_bt::NodeStatus::FAILURE;
  }
};
