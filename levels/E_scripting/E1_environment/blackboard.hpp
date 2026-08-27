#pragma once

#include "mini_bt/script_value.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

class Blackboard
{
public:
  void set(const std::string &key, ScriptValue value)
  {
    values_[key] = std::move(value);
  }
  const ScriptValue &get(const std::string &key) const
  {
    auto it = values_.find(key);
    if (it == values_.end())
    {
      throw std::runtime_error("Key not found in blackboard");
    }
    return it->second;
  }
  bool contains(const std::string &key) const
  {
    return values_.find(key) != values_.end();
  }

private:
  std::unordered_map<std::string, ScriptValue> values_;
};
