#pragma once

#include <stdexcept>
#include <string>
#include <variant>

class ScriptValue
{
public:
  using Storage = std::variant<double, bool, std::string>;

  ScriptValue() = default;
  ScriptValue(double value);
  ScriptValue(bool value);
  ScriptValue(std::string value);
  ScriptValue(const char* value);

  bool isNumber() const;
  bool isBool() const;
  bool isString() const;

  double asNumber() const;
  bool asBool() const;
  const std::string& asString() const;

private:
  static std::runtime_error typeError(const std::string& expected,
                                      const std::string& actual);
  std::string typeName() const;

  Storage value_{0.0};
};

