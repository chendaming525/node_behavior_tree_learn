#pragma once

#include <any>
#include <map>
#include <stdexcept>
#include <string>

class Blackboard
{
public:
  template <typename T>
  void set(const std::string& key, const T& value)
  {
    m_data[key] = value;
  }

  template <typename T>
  T get(const std::string& key) const
  {
    const auto it = m_data.find(key);
    if(it == m_data.end())
    {
      throw std::runtime_error("Key not found in blackboard: " + key);
    }
    return std::any_cast<T>(it->second);
  }

private:
  std::map<std::string, std::any> m_data;
};
