#pragma once

#include "mini_bt/blackboard.hpp"
#include "mini_bt/three_node.hpp"
#include "mini_bt/types.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

class Tree
{
public:
  Tree() = default;

  Tree(std::shared_ptr<ThreeNode> root, std::shared_ptr<Blackboard> blackboard)
    : root_(std::move(root)), blackboard_(std::move(blackboard))
  {
    if(!root_)
    {
      throw std::runtime_error("Tree needs a root node");
    }
    if(!blackboard_)
    {
      blackboard_ = std::make_shared<Blackboard>();
    }
    root_->setBlackboard(blackboard_);
  }

  [[nodiscard]] std::shared_ptr<ThreeNode> root() const
  {
    return root_;
  }

  [[nodiscard]] std::shared_ptr<Blackboard> blackboard() const
  {
    return blackboard_;
  }

  mini_bt::NodeStatus tickOnce()
  {
    if(!root_)
    {
      throw std::runtime_error("Tree has no root node");
    }
    return root_->executeTick();
  }

  mini_bt::NodeStatus tickWhileRunning()
  {
    mini_bt::NodeStatus status = mini_bt::NodeStatus::IDLE;
    do
    {
      status = tickOnce();
    } while(status == mini_bt::NodeStatus::RUNNING);
    return status;
  }

private:
  std::shared_ptr<ThreeNode> root_;
  std::shared_ptr<Blackboard> blackboard_;
};
