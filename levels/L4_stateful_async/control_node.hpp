#pragma once

#include "mini_bt/three_node.hpp"

#include <memory>
#include <utility>
#include <vector>

class ControlNode : public ThreeNode
{
public:
  explicit ControlNode(const std::string& name) : ThreeNode(name)
  {}

  void addChild(std::shared_ptr<ThreeNode> child)
  {
    children_.push_back(std::move(child));
  }

protected:
  void resetCurrentChild()
  {
    current_child_index_ = 0;
  }

  std::vector<std::shared_ptr<ThreeNode>> children_;
  size_t current_child_index_ = 0;
};
