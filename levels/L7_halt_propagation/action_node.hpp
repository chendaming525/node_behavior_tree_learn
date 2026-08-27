#pragma once

#include "mini_bt/three_node.hpp"

class AlwaysSuccessNode : public ThreeNode
{
public:
  explicit AlwaysSuccessNode(const std::string& name) : ThreeNode(name)
  {}

  mini_bt::NodeStatus tick() override
  {
    return mini_bt::NodeStatus::SUCCESS;
  }
};

class AlwaysFailureNode : public ThreeNode
{
public:
  explicit AlwaysFailureNode(const std::string& name) : ThreeNode(name)
  {}

  mini_bt::NodeStatus tick() override
  {
    return mini_bt::NodeStatus::FAILURE;
  }
};
