#pragma once

#include "mini_bt/stateful_action.hpp"

#include <iostream>

class SleepAction : public StatefulAction
{
public:
  explicit SleepAction(const std::string& name, NodeConfig config = {}, int ticks_needed = 5)
    : StatefulAction(name, std::move(config)), ticks_needed_(ticks_needed)
  {}

  mini_bt::NodeStatus onStart() override
  {
    count_tick = 0;
    std::cout << "[" << getName() << "] sleep start (need " << ticks_needed_ << " ticks)\n";
    return mini_bt::NodeStatus::RUNNING;
  }

  mini_bt::NodeStatus onRunning() override
  {
    ++count_tick;
    std::cout << "[" << getName() << "] sleep tick " << count_tick << "/" << ticks_needed_
              << "\n";
    if(count_tick >= ticks_needed_)
    {
      count_tick = 0;
      return mini_bt::NodeStatus::SUCCESS;
    }
    return mini_bt::NodeStatus::RUNNING;
  }

  void onHalted() override
  {
    count_tick = 0;
    std::cout << "[" << getName() << "] onHalted, counter reset\n";
  }

private:
  int ticks_needed_ = 5;
  int count_tick = 0;
};
