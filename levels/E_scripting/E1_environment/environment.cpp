#include "mini_bt/environment.hpp"

#include <memory>

Environment makeEnvironment()
{
  Environment env;
  env.vars = std::make_shared<Blackboard>();
  env.enums = std::make_shared<EnumsTable>();
  return env;
}