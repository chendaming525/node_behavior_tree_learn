#pragma once

#include "three_node.hpp"

#include <memory>
#include <utility>
#include <vector>

class ControlNode : public ThreeNode
{
public:
  explicit ControlNode(const std::string& name, NodeConfig config = {})
    : ThreeNode(name, std::move(config))
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

  void resetChildren()
  {
    for(auto& child : children_)
    {
      if(child->curStatus() == mini_bt::NodeStatus::RUNNING)
      {
        child->halt();
      }
      else
      {
        child->setStatus(mini_bt::NodeStatus::IDLE);
      }
    }
    current_child_index_ = 0;
  }

  void halt() override
  {
    resetChildren();
    ThreeNode::halt();
  }

protected:
  std::vector<std::shared_ptr<ThreeNode>> children_;
  size_t current_child_index_ = 0;
};
