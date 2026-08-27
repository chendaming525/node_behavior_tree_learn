#pragma once

#include "mini_bt/environment.hpp"
#include "mini_bt/script_value.hpp"

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

struct ExprBase
{
  virtual ~ExprBase() = default;
  virtual ScriptValue evaluate(Environment& env) const = 0;
};

using ExprPtr = std::shared_ptr<ExprBase>;

inline bool asBool(const ScriptValue& v)
{
  if (v.isBool())
  {
    return v.asBool();
  }
  if (v.isNumber())
  {
    return v.asNumber() != 0.0;
  }
  throw std::runtime_error("expected bool or number in logical operation");
}

inline bool compareEqual(const ScriptValue& left, const ScriptValue& right)
{
  if (left.isNumber() && right.isNumber())
  {
    return left.asNumber() == right.asNumber();
  }
  if (left.isString() && right.isString())
  {
    return left.asString() == right.asString();
  }
  throw std::runtime_error("cannot compare mixed types with ==");
}

inline bool compareLess(const ScriptValue& left, const ScriptValue& right)
{
  if (left.isNumber() && right.isNumber())
  {
    return left.asNumber() < right.asNumber();
  }
  if (left.isString() && right.isString())
  {
    return left.asString() < right.asString();
  }
  throw std::runtime_error("cannot compare mixed types with <");
}

struct ExprLiteral : ExprBase
{
  ScriptValue value;

  explicit ExprLiteral(ScriptValue v) : value(std::move(v)) {}

  ScriptValue evaluate(Environment&) const override
  {
    return value;
  }
};

struct ExprName : ExprBase
{
  std::string name;

  explicit ExprName(std::string n) : name(std::move(n)) {}

  ScriptValue evaluate(Environment& env) const override
  {
    return env.vars->get(name);
  }
};

enum class UnaryOp
{
  Negate,  // -x
  Not      // !x
};

struct ExprUnary : ExprBase
{
  UnaryOp op;
  ExprPtr rhs;

  ExprUnary(UnaryOp o, ExprPtr e) : op(o), rhs(std::move(e)) {}

  ScriptValue evaluate(Environment& env) const override
  {
    const ScriptValue v = rhs->evaluate(env);
    switch (op)
    {
      case UnaryOp::Negate:
        return ScriptValue(-v.asNumber());
      case UnaryOp::Not:
        return ScriptValue(!asBool(v));
      default:
        throw std::runtime_error("Invalid unary operator");
    }
  }
};

enum class BinaryOp
{
  Add,
  Sub,
  Mul,
  Div,
  Eq,
  Ne,
  Lt,
  Gt,
  Le,
  Ge,
  And,
  Or
};

struct ExprBinary : ExprBase
{
  ExprPtr lhs;
  BinaryOp op;
  ExprPtr rhs;

  ExprBinary(ExprPtr l, BinaryOp o, ExprPtr r)
    : lhs(std::move(l)), op(o), rhs(std::move(r))
  {}

  ScriptValue evaluate(Environment& env) const override
  {
    const ScriptValue left = lhs->evaluate(env);
    const ScriptValue right = rhs->evaluate(env);

    switch (op)
    {
      case BinaryOp::Add:
        return ScriptValue(left.asNumber() + right.asNumber());
      case BinaryOp::Sub:
        return ScriptValue(left.asNumber() - right.asNumber());
      case BinaryOp::Mul:
        return ScriptValue(left.asNumber() * right.asNumber());
      case BinaryOp::Div:
      {
        const double divisor = right.asNumber();
        if (std::fabs(divisor) < 1e-12)
        {
          throw std::runtime_error("division by zero");
        }
        return ScriptValue(left.asNumber() / divisor);
      }
      case BinaryOp::Eq:
        return ScriptValue(compareEqual(left, right));
      case BinaryOp::Ne:
        return ScriptValue(!compareEqual(left, right));
      case BinaryOp::Lt:
        return ScriptValue(compareLess(left, right));
      case BinaryOp::Gt:
        return ScriptValue(compareLess(right, left) && !compareEqual(left, right));
      case BinaryOp::Le:
        return ScriptValue(compareLess(left, right) || compareEqual(left, right));
      case BinaryOp::Ge:
        return ScriptValue(!compareLess(left, right));
      case BinaryOp::And:
        return ScriptValue(asBool(left) && asBool(right));
      case BinaryOp::Or:
        return ScriptValue(asBool(left) || asBool(right));
      default:
        throw std::runtime_error("Invalid binary operator");
    }
  }
};

inline ExprPtr lit(double x)
{
  return std::make_shared<ExprLiteral>(ScriptValue(x));
}

inline ExprPtr lit(bool b)
{
  return std::make_shared<ExprLiteral>(ScriptValue(b));
}

inline ExprPtr lit(const char* s)
{
  return std::make_shared<ExprLiteral>(ScriptValue(s));
}

inline ExprPtr name(std::string n)
{
  return std::make_shared<ExprName>(std::move(n));
}

inline ExprPtr unary(UnaryOp op, ExprPtr e)
{
  return std::make_shared<ExprUnary>(op, std::move(e));
}

inline ExprPtr binary(ExprPtr l, BinaryOp op, ExprPtr r)
{
  return std::make_shared<ExprBinary>(std::move(l), op, std::move(r));
}
