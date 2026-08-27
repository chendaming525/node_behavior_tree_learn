#include "mini_bt/script_value.hpp"

ScriptValue::ScriptValue(double value) : value_(value) {}
ScriptValue::ScriptValue(bool value) : value_(value) {}
ScriptValue::ScriptValue(std::string value) : value_(std::move(value)) {}
ScriptValue::ScriptValue(const char* value) : value_(std::string(value)) {}

bool ScriptValue::isNumber() const
{
  return std::holds_alternative<double>(value_);
}
bool ScriptValue::isBool() const
{
  return std::holds_alternative<bool>(value_);
}
bool ScriptValue::isString() const
{
  return std::holds_alternative<std::string>(value_);
}

double ScriptValue::asNumber() const
{
  if (!isNumber())
  {
    throw typeError("number", typeName());
  }
  return std::get<double>(value_);
}
bool ScriptValue::asBool() const
{
  if (!isBool())
  {
    throw typeError("bool", typeName());
  }
  return std::get<bool>(value_);
}
const std::string& ScriptValue::asString() const
{
  if (!isString())
  {
    throw typeError("string", typeName());
  }
  return std::get<std::string>(value_);
}

std::string ScriptValue::typeName() const
{
    if (isNumber())
        return "number";
    if (isBool())
        return "bool";
    if (isString())
        return "string";
    return "unknown";
}

std::runtime_error ScriptValue::typeError(const std::string &expected,
                                          const std::string &actual)
{
    return std::runtime_error("ScriptValue type error: expected " + expected +
                              ", got " + actual);
}