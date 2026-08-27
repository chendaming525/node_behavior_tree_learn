#pragma once

#include "mini_bt/three_node.hpp"

#include <memory>
#include <utility>

class DecoratorBase : public ThreeNode
{
public:
  DecoratorBase(const std::string& name, std::shared_ptr<ThreeNode> child)
    : ThreeNode(name), child_(std::move(child))
  {}

protected:
  void resetChild()
  {
    if(child_)
    {
      child_->setStatus(mini_bt::NodeStatus::IDLE);
    }
  }

  std::shared_ptr<ThreeNode> child_;
};
