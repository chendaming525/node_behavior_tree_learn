#pragma once

#include <any>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class Blackboard
{
public:
  Blackboard() = default;

  Blackboard(std::shared_ptr<Blackboard> parent, std::map<std::string, std::string> remapping)
    : parent_(std::move(parent)), remapping_(std::move(remapping))
  {}

  template <typename T>
  void set(const std::string& key, const T& value)
  {
    const auto remap = remapping_.find(key);
    if(remap != remapping_.end())
    {
      if(!parent_)
      {
        throw std::runtime_error("Blackboard remapping has no parent for key: " + key);
      }
      parent_->set(remap->second, value);
      return;
    }
    m_data[key] = value;
  }

  template <typename T>
  T get(const std::string& key) const
  {
    const auto it = m_data.find(key);
    if(it != m_data.end())
    {
      return std::any_cast<T>(it->second);
    }

    const auto remap = remapping_.find(key);
    if(remap != remapping_.end())
    {
      if(!parent_)
      {
        throw std::runtime_error("Blackboard remapping has no parent for key: " + key);
      }
      return parent_->get<T>(remap->second);
    }

    throw std::runtime_error("Key not found in blackboard: " + key);
  }

private:
  std::map<std::string, std::any> m_data;
  std::shared_ptr<Blackboard> parent_;
  std::map<std::string, std::string> remapping_;
};
