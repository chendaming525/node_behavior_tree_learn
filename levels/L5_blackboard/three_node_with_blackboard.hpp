#pragma once

#include "mini_bt/blackboard.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class ThreeNode
{
public:
  explicit ThreeNode(std::string name) : m_name(std::move(name))
  {}

  virtual ~ThreeNode() = default;

  ThreeNode(const ThreeNode&) = delete;
  ThreeNode& operator=(const ThreeNode&) = delete;

  [[nodiscard]] const std::string& getName() const
  {
    return m_name;
  }

  virtual mini_bt::NodeStatus tick()
  {
    return mini_bt::NodeStatus::SUCCESS;
  }

  virtual mini_bt::NodeStatus executeTick()
  {
    const mini_bt::NodeStatus status = tick();
    setStatus(status);
    return status;
  }

  [[nodiscard]] mini_bt::NodeStatus curStatus() const
  {
    return m_status;
  }

  void setStatus(mini_bt::NodeStatus status)
  {
    m_status = status;
  }

  /** Like BT::resetStatus */
  void resetStatus()
  {
    m_status = mini_bt::NodeStatus::IDLE;
  }

  virtual void halt()
  {
    resetStatus();
  }

  virtual void setBlackboard(std::shared_ptr<Blackboard> bb)
  {
    m_blackboard = std::move(bb);
  }

  [[nodiscard]] std::shared_ptr<Blackboard> blackboard() const
  {
    return m_blackboard;
  }

  template <typename T>
  void setOutput(const std::string& key, const T& value)
  {
    if(!m_blackboard)
    {
      throw std::runtime_error(m_name + ": setOutput needs a blackboard");
    }
    m_blackboard->set(key, value);
  }

  template <typename T>
  [[nodiscard]] T getInput(const std::string& key) const
  {
    if(!m_blackboard)
    {
      throw std::runtime_error(m_name + ": getInput needs a blackboard");
    }
    return m_blackboard->get<T>(key);
  }

  template <typename T>
  [[nodiscard]] bool getInput(const std::string& key, T& out) const
  {
    if(!m_blackboard)
    {
      return false;
    }
    try
    {
      out = m_blackboard->get<T>(key);
      return true;
    }
    catch(...)
    {
      return false;
    }
  }

private:
  std::string m_name;
  mini_bt::NodeStatus m_status = mini_bt::NodeStatus::IDLE;
  std::shared_ptr<Blackboard> m_blackboard;
};
