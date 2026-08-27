#pragma once

#include "mini_bt/decorator.hpp"

#include <utility>

class RetryNode : public DecoratorBase
{
public:
  RetryNode(const std::string& name, std::shared_ptr<ThreeNode> child, int times)
    : DecoratorBase(name, std::move(child)), m_times(times)
  {}

  mini_bt::NodeStatus tick() override
  {
    const mini_bt::NodeStatus status = child_->executeTick();

    if(status == mini_bt::NodeStatus::RUNNING)
    {
      return mini_bt::NodeStatus::RUNNING;
    }
    if(status == mini_bt::NodeStatus::SUCCESS)
    {
      attempt_ = 0;
      resetChild();
      return mini_bt::NodeStatus::SUCCESS;
    }

    // FAILURE
    ++attempt_;
    if(attempt_ < m_times)
    {
      resetChild();
      return mini_bt::NodeStatus::RUNNING;
    }

    attempt_ = 0;
    resetChild();
    return mini_bt::NodeStatus::FAILURE;
  }

private:
  int m_times = 1;
  int attempt_ = 0;
};
