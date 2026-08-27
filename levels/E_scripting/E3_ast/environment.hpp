#pragma once

#include "mini_bt/blackboard.hpp"

#include <memory>

class EnumsTable
{
};

struct Environment
{
  std::shared_ptr<Blackboard> vars;
  std::shared_ptr<EnumsTable> enums;
};

Environment makeEnvironment();
