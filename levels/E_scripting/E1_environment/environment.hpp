#pragma once

#include "mini_bt/blackboard.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class EnumsTable
{
public:
  // TODO(E1): Fill this when enums are actually needed.
  // For now, E1 only needs the type to exist inside Environment.
};

struct Environment
{
  std::shared_ptr<Blackboard> vars;
  std::shared_ptr<EnumsTable> enums;
};

Environment makeEnvironment();

