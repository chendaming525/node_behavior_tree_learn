#include "mini_bt/environment.hpp"

#include <iostream>

int main()
{

  Environment env = makeEnvironment();
  env.vars->set("x", ScriptValue(42.0));
  double x = env.vars->get("x").asNumber();
  std::cout << "x = " << x << std::endl;

  env.vars->set("flag", ScriptValue(true));
  bool flag = env.vars->get("flag").asBool();
  std::cout << "flag = " << std::boolalpha << flag << std::endl;

  env.vars->set("message", ScriptValue("Hello, World!"));
  std::string message = env.vars->get("message").asString();
  std::cout << "message = " << message << std::endl;
  return 0;
}
