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
    if(auto bb = blackboard())
    {
      child->setBlackboard(bb);
    }
    children_.push_back(std::move(child));
  }

  void setBlackboard(std::shared_ptr<Blackboard> bb) override
  {
    ThreeNode::setBlackboard(std::move(bb));
    for(auto& child : children_)
    {
      child->setBlackboard(blackboard());
    }
  }

protected:
  void resetCurrentChild()
  {
    current_child_index_ = 0;
  }

  std::vector<std::shared_ptr<ThreeNode>> children_;
  size_t current_child_index_ = 0;
};
