#pragma once

#include "mini_bt/types.hpp"

#include <string>
#include <utility>

class ThreeNode
{
public:
  explicit ThreeNode(std::string name) : name_(std::move(name))
  {}

  virtual ~ThreeNode() = default;

  ThreeNode(const ThreeNode&) = delete;
  ThreeNode& operator=(const ThreeNode&) = delete;

  [[nodiscard]] const std::string& getName() const
  {
    return name_;
  }

  [[nodiscard]] mini_bt::NodeStatus curStatus() const
  {
    return status_;
  }

  void setStatus(mini_bt::NodeStatus status)
  {
    status_ = status;
  }

  void resetStatus()
  {
    status_ = mini_bt::NodeStatus::IDLE;
  }

  virtual void halt()
  {
    resetStatus();
  }

  mini_bt::NodeStatus executeTick()
  {
    const mini_bt::NodeStatus status = tick();
    setStatus(status);
    return status;
  }

protected:
  virtual mini_bt::NodeStatus tick() = 0;

private:
  std::string name_;
  mini_bt::NodeStatus status_ = mini_bt::NodeStatus::IDLE;
};
