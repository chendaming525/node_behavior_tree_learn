#pragma once

#include "mini_bt/node_config.hpp"
#include "mini_bt/ports.hpp"
#include "mini_bt/types.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class ThreeNode
{
public:
  explicit ThreeNode(std::string name, NodeConfig config = {})
    : m_name(std::move(name)), m_config(std::move(config))
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

  virtual PortsList ports() const
  {
    return {};
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
    m_config.blackboard = std::move(bb);
  }

  [[nodiscard]] std::shared_ptr<Blackboard> blackboard() const
  {
    return m_config.blackboard;
  }

  [[nodiscard]] const NodeConfig& config() const
  {
    return m_config;
  }

  NodeConfig& config()
  {
    return m_config;
  }

  template <typename T>
  void setOutput(const std::string& port_name, const T& value)
  {
    validatePort(port_name, PortDirection::OUTPUT);
    const auto it = m_config.output_ports.find(port_name);
    if(it == m_config.output_ports.end())
    {
      throw std::runtime_error(m_name + ": missing output remapping for port " + port_name);
    }
    if(!isBlackboardPointer(it->second))
    {
      throw std::runtime_error(
          m_name + ": output port " + port_name + " must remap to {blackboard_key}");
    }
    if(!m_config.blackboard)
    {
      throw std::runtime_error(m_name + ": setOutput needs a blackboard");
    }
    m_config.blackboard->set(stripBlackboardPointer(it->second), value);
  }

  template <typename T>
  [[nodiscard]] T getInput(const std::string& port_name) const
  {
    T out{};
    if(!getInput(port_name, out))
    {
      throw std::runtime_error(m_name + ": failed to read input port " + port_name);
    }
    return out;
  }

  template <typename T>
  [[nodiscard]] bool getInput(const std::string& port_name, T& out) const
  {
    validatePort(port_name, PortDirection::INPUT);
    const auto it = m_config.input_ports.find(port_name);
    if(it == m_config.input_ports.end())
    {
      return false;
    }
    try
    {
      if(isBlackboardPointer(it->second))
      {
        if(!m_config.blackboard)
        {
          return false;
        }
        out = m_config.blackboard->get<T>(stripBlackboardPointer(it->second));
      }
      else
      {
        out = convertFromString<T>(it->second);
      }
      return true;
    }
    catch(...)
    {
      return false;
    }
  }

private:
  void validatePort(const std::string& port_name, PortDirection direction) const
  {
    const PortsList manifest = ports();
    if(manifest.empty())
    {
      return;
    }

    const auto it = manifest.find(port_name);
    if(it == manifest.end())
    {
      throw std::runtime_error(m_name + ": unknown port " + port_name);
    }
    if(it->second.direction != direction)
    {
      throw std::runtime_error(m_name + ": port direction mismatch for " + port_name);
    }
  }

  std::string m_name;
  mini_bt::NodeStatus m_status = mini_bt::NodeStatus::IDLE;
  NodeConfig m_config;
};
