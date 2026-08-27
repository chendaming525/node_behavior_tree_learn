#pragma once

#include "control_node.hpp"

class SequenceNode : public ControlNode
{
public:
  explicit SequenceNode(const std::string& name, NodeConfig config = {})
    : ControlNode(name, std::move(config))
  {}

  static PortsList providedPorts()
  {
    return {};
  }

  mini_bt::NodeStatus tick() override
  {
    while(current_child_index_ < children_.size())
    {
      const mini_bt::NodeStatus status = children_[current_child_index_]->executeTick();
      if(status == mini_bt::NodeStatus::RUNNING)
      {
        return mini_bt::NodeStatus::RUNNING;
      }
      if(status == mini_bt::NodeStatus::FAILURE)
      {
        resetChildren();
        return mini_bt::NodeStatus::FAILURE;
      }
      ++current_child_index_;
    }
    resetChildren();
    return mini_bt::NodeStatus::SUCCESS;
  }
};
